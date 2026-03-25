#Requires -Version 5.1
<#
.SYNOPSIS
    运行 CTest 测试

.DESCRIPTION
    此脚本用于运行 CMake 项目的 CTest 测试套件。
    支持并行测试、标签过滤和输出格式化。

.PARAMETER BuildDir
    构建目录路径（默认：build）

.PARAMETER BuildType
    构建类型（Debug、Release）

.PARAMETER Parallel
    并行测试数量（默认自动检测）

.PARAMETER Label
    测试标签过滤

.PARAMETER ExcludeLabel
    排除的测试标签

.PARAMETER OutputOnFailure
    测试失败时显示输出

.PARAMETER Verbose
    显示详细输出

.PARAMETER Coverage
    是否收集代码覆盖率

.EXAMPLE
    ./run-tests-ctest.ps1 -Parallel 4

.EXAMPLE
    ./run-tests-ctest.ps1 -Label "unit" -OutputOnFailure
#>

param(
    [string]$BuildDir = "build",
    [ValidateSet("Debug", "Release", "RelWithDebInfo", "MinSizeRel")]
    [string]$BuildType = "Debug",
    [int]$Parallel = 0,
    [string]$Label = "",
    [string]$ExcludeLabel = "",
    [switch]$OutputOnFailure,
    [switch]$Verbose,
    [switch]$Coverage
)

$ErrorActionPreference = "Stop"

# 颜色输出函数
function Write-ColorOutput {
    param([string]$Message, [string]$Color = "White")
    Write-Host $Message -ForegroundColor $Color
}

function Write-Success { Write-ColorOutput $args "Green" }
function Write-Error   { Write-ColorOutput $args "Red" }
function Write-Info    { Write-ColorOutput $args "Cyan" }
function Write-Warning { Write-ColorOutput $args "Yellow" }

# 检查 CTest 是否可用
function Test-CTestAvailable {
    try {
        $version = ctest --version 2>&1
        Write-Info "检测到 $version"
        return $true
    }
    catch {
        Write-Error "错误：未找到 CTest"
        return $false
    }
}

# 检查构建目录
function Test-BuildDirectory {
    param([string]$BuildDir)
    
    if (-not (Test-Path $BuildDir)) {
        Write-Error "错误：构建目录不存在: $BuildDir"
        Write-Info "请先运行 CMake 配置和构建"
        return $false
    }
    
    return $true
}

# 构建测试（如果需要）
function Build-Tests {
    param(
        [string]$BuildDir,
        [string]$BuildType
    )
    
    Write-Info "构建测试..."
    
    $arguments = @(
        "--build", $BuildDir,
        "--config", $BuildType,
        "--target", "all"  # Windows MSBuild 使用 ALL_BUILD
    )
    
    # 尝试构建测试目标
    & cmake @arguments
    
    if ($LASTEXITCODE -ne 0) {
        throw "测试构建失败"
    }
    
    Write-Success "测试构建完成"
}

# 运行 CTest
function Invoke-CTest {
    param(
        [string]$BuildDir,
        [string]$BuildType,
        [int]$Parallel,
        [string]$Label,
        [string]$ExcludeLabel,
        [switch]$OutputOnFailure,
        [switch]$Verbose
    )
    
    Write-Info "运行测试..."
    
    Push-Location $BuildDir
    
    try {
        $arguments = @(
            "-C", $BuildType,
            "--output-on-failure"
        )
        
        # 并行测试
        if ($Parallel -gt 0) {
            $arguments += "-j", $Parallel
        } else {
            # 自动检测 CPU 数量
            $cpuCount = (Get-CimInstance Win32_ComputerSystem).NumberOfLogicalProcessors
            if (-not $cpuCount) {
                $cpuCount = 4
            }
            $arguments += "-j", $cpuCount
        }
        
        # 标签过滤
        if ($Label) {
            $arguments += "-L", $Label
        }
        
        if ($ExcludeLabel) {
            $arguments += "-LE", $ExcludeLabel
        }
        
        # 详细输出
        if ($Verbose) {
            $arguments += "-V"
        }
        
        # 输出格式
        $arguments += "-T", "Test"
        $arguments += "--output-junit", "junit-results.xml"
        
        Write-Info "执行: ctest $($arguments -join ' ')"
        Write-Info ""
        
        & ctest @arguments
        
        $exitCode = $LASTEXITCODE
        
        Pop-Location
        return $exitCode
    }
    catch {
        Pop-Location
        throw $_
    }
}

# 收集覆盖率
function Get-CoverageReport {
    param([string]$BuildDir)
    
    Write-Info "收集代码覆盖率..."
    
    # 检查 gcov 是否可用
    $gcov = Get-Command gcov -ErrorAction SilentlyContinue
    if (-not $gcov) {
        Write-Warning "未找到 gcov，跳过覆盖率收集"
        return
    }
    
    # 检查 lcov 是否可用
    $lcov = Get-Command lcov -ErrorAction SilentlyContinue
    if ($lcov) {
        Write-Info "使用 lcov 生成覆盖率报告..."
        
        & lcov --capture --directory $BuildDir --output-file coverage.info
        & lcov --remove coverage.info '/usr/*' --output-file coverage.info
        & lcov --list coverage.info
        
        Write-Success "覆盖率报告已保存到: coverage.info"
        
        # 检查 genhtml
        $genhtml = Get-Command genhtml -ErrorAction SilentlyContinue
        if ($genhtml) {
            & genhtml coverage.info --output-directory coverage_html
            Write-Success "HTML 报告已生成到: coverage_html/"
        }
    }
}

# 解析测试结果
function Show-TestSummary {
    param([string]$BuildDir)
    
    $testFile = Join-Path $BuildDir "Testing/Temporary/LastTests.log"
    
    if (Test-Path $testFile) {
        $content = Get-Content $testFile -Raw
        
        # 统计
        $passed = ([regex]::Matches($content, "Passed")).Count
        $failed = ([regex]::Matches($content, "Failed")).Count
        
        Write-Info "=========================================="
        Write-Info "测试摘要"
        Write-Info "=========================================="
        
        if ($failed -eq 0) {
            Write-Success "所有测试通过！"
        } else {
            Write-Error "有 $failed 个测试失败"
        }
    }
}

# 主函数
function Main {
    Write-Info "=========================================="
    Write-Info "     CTest 测试运行脚本"
    Write-Info "=========================================="
    
    # 检查 CTest
    if (-not (Test-CTestAvailable)) {
        exit 1
    }
    
    # 检查构建目录
    if (-not (Test-BuildDirectory -BuildDir $BuildDir)) {
        exit 1
    }
    
    try {
        # 构建测试
        Build-Tests -BuildDir $BuildDir -BuildType $BuildType
        
        # 运行测试
        $exitCode = Invoke-CTest -BuildDir $BuildDir -BuildType $BuildType `
                     -Parallel $Parallel -Label $Label -ExcludeLabel $ExcludeLabel `
                     -OutputOnFailure:$OutputOnFailure -Verbose:$Verbose
        
        # 显示摘要
        Show-TestSummary -BuildDir $BuildDir
        
        # 覆盖率
        if ($Coverage) {
            Get-CoverageReport -BuildDir $BuildDir
        }
        
        Write-Info "=========================================="
        if ($exitCode -eq 0) {
            Write-Success "测试执行成功！"
        } else {
            Write-Error "测试执行失败，退出码: $exitCode"
        }
        Write-Info "=========================================="
        
        exit $exitCode
    }
    catch {
        Write-Error "错误: $_"
        exit 1
    }
}

Main
