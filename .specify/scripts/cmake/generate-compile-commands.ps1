#Requires -Version 5.1
<#
.SYNOPSIS
    生成 compile_commands.json 文件

.DESCRIPTION
    此脚本用于为 CMake 项目生成 compile_commands.json 文件，
    该文件用于支持 clangd、clang-tidy 等工具的代码智能功能。

.PARAMETER BuildDir
    构建目录路径（默认：build）

.PARAMETER BuildType
    构建类型（默认：Debug）

.PARAMETER Generator
    CMake 生成器（可选，默认自动检测）

.PARAMETER Clean
    是否在配置前清理构建目录

.PARAMETER CopyToRoot
    是否将文件复制到项目根目录

.PARAMETER Verbose
    显示详细输出

.EXAMPLE
    ./generate-compile-commands.ps1

.EXAMPLE
    ./generate-compile-commands.ps1 -CopyToRoot

.EXAMPLE
    ./generate-compile-commands.ps1 -BuildDir cmake-build-debug -BuildType Debug
#>

param(
    [string]$BuildDir = "build",
    [ValidateSet("Debug", "Release", "RelWithDebInfo", "MinSizeRel")]
    [string]$BuildType = "Debug",
    [string]$Generator = "",
    [switch]$Clean,
    [switch]$CopyToRoot,
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

# 检查 CMake
function Test-CMakeAvailable {
    try {
        $version = cmake --version 2>&1 | Select-Object -First 1
        Write-Info "检测到 $version"
        return $true
    }
    catch {
        Write-Error "错误：未找到 CMake"
        return $false
    }
}

# 检测生成器
function Get-BestGenerator {
    if ($Generator) {
        return $Generator
    }
    
    if ($IsWindows -or ($env:OS -eq "Windows_NT")) {
        # 检查 Ninja
        $ninja = Get-Command ninja -ErrorAction SilentlyContinue
        if ($ninja) {
            return "Ninja"
        }
        
        # 使用 VS
        return "Visual Studio 17 2022"
    }
    
    # Linux/macOS
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

# 配置 CMake
function Invoke-CMakeConfigure {
    param(
        [string]$SourceDir,
        [string]$BuildDir,
        [string]$BuildType,
        [string]$Generator
    )
    
    Write-Info "配置 CMake 项目..."
    Write-Info "  源目录: $SourceDir"
    Write-Info "  构建目录: $BuildDir"
    Write-Info "  构建类型: $BuildType"
    Write-Info "  生成器: $Generator"
    
    # 确保构建目录存在
    if (-not (Test-Path $BuildDir)) {
        New-Item -Path $BuildDir -ItemType Directory -Force | Out-Null
    }
    
    $arguments = @(
        "-S", $SourceDir,
        "-B", $BuildDir,
        "-G", $Generator,
        "-DCMAKE_BUILD_TYPE=$BuildType",
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
    )
    
    # Windows MSVC 特定设置
    if ($Generator -like "Visual Studio*") {
        $arguments += "-A", "x64"
    }
    
    if ($Verbose) {
        Write-Info "执行: cmake $($arguments -join ' ')"
    }
    
    & cmake @arguments
    
    if ($LASTEXITCODE -ne 0) {
        throw "CMake 配置失败"
    }
    
    Write-Success "CMake 配置成功"
}

# 查找 compile_commands.json
function Find-CompileCommands {
    param([string]$BuildDir)
    
    $compileCommands = Join-Path $BuildDir "compile_commands.json"
    
    if (Test-Path $compileCommands) {
        Write-Success "找到 compile_commands.json: $compileCommands"
        return $compileCommands
    }
    
    # 某些生成器可能在子目录中
    $subDir = Join-Path $BuildDir "CMakeFiles"
    $subFile = Join-Path $subDir "compile_commands.json"
    
    if (Test-Path $subFile) {
        Write-Success "找到 compile_commands.json: $subFile"
        return $subFile
    }
    
    Write-Error "错误：未找到 compile_commands.json"
    return $null
}

# 复制到项目根目录
function Copy-CompileCommandsToRoot {
    param(
        [string]$SourceFile,
        [string]$ProjectRoot
    )
    
    $destFile = Join-Path $ProjectRoot "compile_commands.json"
    
    Copy-Item -Path $SourceFile -Destination $destFile -Force
    
    Write-Success "已复制到: $destFile"
}

# 验证 compile_commands.json
function Test-CompileCommands {
    param([string]$FilePath)
    
    Write-Info "验证 compile_commands.json..."
    
    try {
        $content = Get-Content $FilePath -Raw | ConvertFrom-Json
        
        $count = @($content).Count
        Write-Info "  编译单元数: $count"
        
        # 提取一些统计信息
        $compilers = @($content | ForEach-Object { $_.command.Split()[0] } | Select-Object -Unique)
        Write-Info "  编译器: $($compilers -join ', ')"
        
        $files = @($content | ForEach-Object { $_.file })
        $cppCount = ($files | Where-Object { $_ -match '\.(cpp|cxx|cc)$' }).Count
        $cCount = ($files | Where-Object { $_ -match '\.c$' }).Count
        
        Write-Info "  C++ 文件: $cppCount"
        Write-Info "  C 文件: $cCount"
        
        Write-Success "文件格式验证通过"
        return $true
    }
    catch {
        Write-Error "文件格式验证失败: $_"
        return $false
    }
}

# 创建 .clangd 配置（如果不存在）
function New-ClangdConfig {
    param([string]$ProjectRoot)
    
    $clangdConfig = Join-Path $ProjectRoot ".clangd"
    
    if (Test-Path $clangdConfig) {
        Write-Info "已存在 .clangd 配置文件"
        return
    }
    
    $configContent = @"
CompileFlags:
  CompilationDatabase: .

Diagnostics:
  UnusedIncludes: Strict
  MissingIncludes: Strict

InlayHints:
  Enabled: Yes
  ParameterNames: Yes
  DeducedTypes: Yes
  Designators: Yes

Hover:
  ShowAKA: Yes
"@
    
    $configContent | Out-File -FilePath $clangdConfig -Encoding UTF8
    Write-Success "已创建 .clangd 配置文件"
}

# 主函数
function Main {
    Write-Info "=========================================="
    Write-Info "     compile_commands.json 生成脚本"
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
    
    # 检测生成器
    $selectedGenerator = Get-BestGenerator
    
    try {
        # 配置
        Invoke-CMakeConfigure -SourceDir $sourceDir -BuildDir $buildPath `
            -BuildType $BuildType -Generator $selectedGenerator
        
        # 查找生成的文件
        $compileCommands = Find-CompileCommands -BuildDir $buildPath
        if (-not $compileCommands) {
            exit 1
        }
        
        # 验证
        if (-not (Test-CompileCommands -FilePath $compileCommands)) {
            exit 1
        }
        
        # 复制到根目录
        if ($CopyToRoot) {
            Copy-CompileCommandsToRoot -SourceFile $compileCommands -ProjectRoot $sourceDir
        }
        
        # 创建 .clangd 配置
        New-ClangdConfig -ProjectRoot $sourceDir
        
        Write-Info "=========================================="
        Write-Success "compile_commands.json 生成成功！"
        Write-Info "=========================================="
        Write-Info ""
        Write-Info "后续步骤："
        Write-Info "  1. 配置编辑器使用 compile_commands.json"
        Write-Info "  2. VS Code: 安装 clangd 扩展"
        Write-Info "  3. 运行 clang-tidy: .specify/scripts/cmake/run-clang-tidy.ps1"
        
        exit 0
    }
    catch {
        Write-Error "错误: $_"
        exit 1
    }
}

Main
