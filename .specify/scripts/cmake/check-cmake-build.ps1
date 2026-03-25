#Requires -Version 5.1
<#
.SYNOPSIS
    检查 CMake 构建是否成功

.DESCRIPTION
    此脚本用于验证 CMake 项目的配置和构建是否成功。
    支持多种构建类型和生成器。

.PARAMETER BuildDir
    构建目录路径（默认：build）

.PARAMETER BuildType
    构建类型（Debug、Release、RelWithDebInfo、MinSizeRel）

.PARAMETER Generator
    CMake 生成器（默认自动检测）

.PARAMETER Clean
    是否在构建前清理构建目录

.PARAMETER Verbose
    显示详细输出

.EXAMPLE
    ./check-cmake-build.ps1 -BuildType Release

.EXAMPLE
    ./check-cmake-build.ps1 -Clean -Verbose
#>

param(
    [string]$BuildDir = "build",
    [ValidateSet("Debug", "Release", "RelWithDebInfo", "MinSizeRel")]
    [string]$BuildType = "Debug",
    [string]$Generator = "",
    [switch]$Clean,
    [switch]$Verbose
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

# 检查 CMake 是否安装
function Test-CMakeAvailable {
    try {
        $cmakeVersion = cmake --version 2>&1 | Select-Object -First 1
        Write-Info "检测到 $cmakeVersion"
        return $true
    }
    catch {
        Write-Error "错误：未找到 CMake，请先安装 CMake"
        return $false
    }
}

# 检测最佳生成器
function Get-BestGenerator {
    if ($Generator) {
        return $Generator
    }
    
    # Windows 上优先使用 Visual Studio
    if ($IsWindows -or ($env:OS -eq "Windows_NT")) {
        # 检查 Visual Studio 是否安装
        $vsWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
        if (Test-Path $vsWhere) {
            $vsPath = & $vsWhere -latest -property installationPath 2>$null
            if ($vsPath) {
                # VS 2022
                if (Test-Path "$vsPath\MSBuild\Current\Bin\MSBuild.exe") {
                    return "Visual Studio 17 2022"
                }
            }
        }
        
        # 检查 Ninja
        $ninja = Get-Command ninja -ErrorAction SilentlyContinue
        if ($ninja) {
            return "Ninja"
        }
        
        return "Visual Studio 17 2022"
    }
    
    # Linux/macOS 默认使用 Ninja 或 Unix Makefiles
    $ninja = Get-Command ninja -ErrorAction SilentlyContinue
    if ($ninja) {
        return "Ninja"
    }
    
    return "Unix Makefiles"
}

# 清理构建目录
function Clear-BuildDirectory {
    param([string]$Path)
    
    if (Test-Path $Path) {
        Write-Info "清理构建目录: $Path"
        Remove-Item -Path $Path -Recurse -Force
    }
}

# 配置项目
function Invoke-CMakeConfigure {
    param(
        [string]$SourceDir,
        [string]$BuildDir,
        [string]$BuildType,
        [string]$Generator
    )
    
    Write-Info "配置项目..."
    Write-Info "  源目录: $SourceDir"
    Write-Info "  构建目录: $BuildDir"
    Write-Info "  构建类型: $BuildType"
    Write-Info "  生成器: $Generator"
    
    $arguments = @(
        "-S", $SourceDir,
        "-B", $BuildDir,
        "-G", $Generator,
        "-DCMAKE_BUILD_TYPE=$BuildType",
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
    )
    
    if ($Verbose) {
        $arguments += "-DCMAKE_VERBOSE_MAKEFILE=ON"
    }
    
    if ($Verbose) {
        Write-Info "执行: cmake $($arguments -join ' ')"
    }
    
    & cmake @arguments
    
    if ($LASTEXITCODE -ne 0) {
        throw "CMake 配置失败，退出码: $LASTEXITCODE"
    }
    
    Write-Success "配置成功"
}

# 构建项目
function Invoke-CMakeBuild {
    param(
        [string]$BuildDir,
        [string]$BuildType
    )
    
    Write-Info "构建项目..."
    
    $arguments = @(
        "--build", $BuildDir,
        "--config", $BuildType
    )
    
    # 并行构建
    if ($IsWindows -or ($env:OS -eq "Windows_NT")) {
        # MSBuild 自动并行
    } else {
        $cpuCount = (Get-CimInstance Win32_ComputerSystem).NumberOfLogicalProcessors
        if (-not $cpuCount) {
            $cpuCount = 4
        }
        $arguments += "-j$cpuCount"
    }
    
    if ($Verbose) {
        Write-Info "执行: cmake $($arguments -join ' ')"
    }
    
    & cmake @arguments
    
    if ($LASTEXITCODE -ne 0) {
        throw "构建失败，退出码: $LASTEXITCODE"
    }
    
    Write-Success "构建成功"
}

# 主函数
function Main {
    Write-Info "=========================================="
    Write-Info "     CMake 构建检查脚本"
    Write-Info "=========================================="
    
    # 检查 CMake
    if (-not (Test-CMakeAvailable)) {
        exit 1
    }
    
    $sourceDir = $PWD.Path
    $buildPath = Join-Path $sourceDir $BuildDir
    
    # 清理
    if ($Clean) {
        Clear-BuildDirectory -Path $buildPath
    }
    
    # 确保构建目录存在
    if (-not (Test-Path $buildPath)) {
        New-Item -Path $buildPath -ItemType Directory -Force | Out-Null
    }
    
    # 检测生成器
    $selectedGenerator = Get-BestGenerator
    
    try {
        # 配置
        Invoke-CMakeConfigure -SourceDir $sourceDir -BuildDir $buildPath -BuildType $BuildType -Generator $selectedGenerator
        
        # 构建
        Invoke-CMakeBuild -BuildDir $buildPath -BuildType $BuildType
        
        Write-Info "=========================================="
        Write-Success "所有检查通过！构建成功完成。"
        Write-Info "=========================================="
        
        # 返回构建目录路径（供其他脚本使用）
        Write-Output $buildPath
        
        exit 0
    }
    catch {
        Write-Error "错误: $_"
        Write-Info "=========================================="
        Write-Error "构建检查失败"
        Write-Info "=========================================="
        exit 1
    }
}

Main
