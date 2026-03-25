#Requires -Version 5.1
<#
.SYNOPSIS
    运行 clang-tidy 静态分析

.DESCRIPTION
    此脚本用于在 CMake 项目上运行 clang-tidy 静态分析。
    需要预先生成 compile_commands.json 文件。

.PARAMETER BuildDir
    构建目录路径（包含 compile_commands.json）

.PARAMETER SourceDir
    源代码目录（默认当前目录）

.PARAMETER Fix
    自动修复发现的问题

.PARAMETER WarningsAsErrors
    将警告视为错误

.PARAMETER OutputFile
    输出报告文件路径

.PARAMETER Verbose
    显示详细输出

.EXAMPLE
    ./run-clang-tidy.ps1 -BuildDir build

.EXAMPLE
    ./run-clang-tidy.ps1 -Fix -WarningsAsErrors
#>

param(
    [string]$BuildDir = "build",
    [string]$SourceDir = "",
    [switch]$Fix,
    [switch]$WarningsAsErrors,
    [string]$OutputFile = "",
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

# 检查 clang-tidy 是否安装
function Test-ClangTidyAvailable {
    try {
        $version = clang-tidy --version 2>&1 | Select-Object -First 1
        Write-Info "检测到 $version"
        return $true
    }
    catch {
        Write-Error "错误：未找到 clang-tidy"
        Write-Info "安装方法："
        Write-Info "  Windows: choco install llvm 或 从 https://releases.llvm.org/ 下载"
        Write-Info "  Linux:   apt install clang-tidy 或 yum install clang-tools-extra"
        Write-Info "  macOS:   brew install llvm"
        return $false
    }
}

# 查找 compile_commands.json
function Find-CompileCommands {
    param([string]$BuildDir)
    
    $compileCommands = Join-Path $BuildDir "compile_commands.json"
    
    if (Test-Path $compileCommands) {
        return $compileCommands
    }
    
    Write-Error "错误：未找到 compile_commands.json"
    Write-Info "请先运行 CMake 配置并启用 CMAKE_EXPORT_COMPILE_COMMANDS"
    Write-Info "示例：cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
    return $null
}

# 获取所有源文件
function Get-SourceFiles {
    param([string]$SourceDir)
    
    if (-not $SourceDir) {
        $SourceDir = $PWD.Path
    }
    
    $extensions = @("*.cpp", "*.cxx", "*.cc", "*.c")
    $files = @()
    
    foreach ($ext in $extensions) {
        $files += Get-ChildItem -Path $SourceDir -Filter $ext -Recurse -File | 
                   Where-Object { $_.FullName -notmatch "build|cmake-build|CMakeFiles" } |
                   Select-Object -ExpandProperty FullName
    }
    
    return $files
}

# 运行 clang-tidy
function Invoke-ClangTidy {
    param(
        [string[]]$Files,
        [string]$CompileCommands,
        [switch]$Fix,
        [switch]$WarningsAsErrors,
        [string]$OutputFile,
        [switch]$Verbose
    )
    
    $arguments = @("-p", (Split-Path $CompileCommands))
    
    if ($Fix) {
        $arguments += "-fix"
        $arguments += "-format"
    }
    
    if ($WarningsAsErrors) {
        $arguments += "--warnings-as-errors=*"
    }
    
    $totalFiles = $Files.Count
    $currentFile = 0
    $errorCount = 0
    $warningCount = 0
    
    Write-Info "开始分析 $totalFiles 个文件..."
    Write-Info ""
    
    foreach ($file in $Files) {
        $currentFile++
        $relativePath = $file.Replace("$($PWD.Path)\", "")
        
        Write-Host "[$currentFile/$totalFiles] " -NoNewline -ForegroundColor Gray
        Write-Host $relativePath
        
        $fileArgs = $arguments + $file
        
        if ($Verbose) {
            Write-Info "  执行: clang-tidy $($fileArgs -join ' ')"
        }
        
        $output = & clang-tidy @fileArgs 2>&1
        $exitCode = $LASTEXITCODE
        
        if ($output) {
            # 分析输出
            $hasErrors = $output -match "error:"
            $hasWarnings = $output -match "warning:"
            
            if ($hasErrors) {
                $errorCount++
                Write-Error "  发现错误"
            }
            
            if ($hasWarnings) {
                $warningCount++
                Write-Warning "  发现警告"
            }
            
            # 显示输出
            $output | ForEach-Object {
                if ($_ -match "error:") {
                    Write-ColorOutput "    $_" "Red"
                } elseif ($_ -match "warning:") {
                    Write-ColorOutput "    $_" "Yellow"
                } else {
                    Write-Host "    $_" -ForegroundColor Gray
                }
            }
            
            # 写入输出文件
            if ($OutputFile) {
                $output | Out-File -FilePath $OutputFile -Append -Encoding UTF8
            }
        }
    }
    
    Write-Info ""
    Write-Info "=========================================="
    Write-Info "分析完成"
    Write-Info "=========================================="
    Write-Info "总文件数: $totalFiles"
    
    if ($errorCount -gt 0) {
        Write-Error "错误数: $errorCount"
    } else {
        Write-Success "错误数: 0"
    }
    
    if ($warningCount -gt 0) {
        Write-Warning "警告数: $warningCount"
    } else {
        Write-Success "警告数: 0"
    }
    
    return @{
        TotalFiles = $totalFiles
        Errors = $errorCount
        Warnings = $warningCount
    }
}

# 主函数
function Main {
    Write-Info "=========================================="
    Write-Info "     clang-tidy 静态分析脚本"
    Write-Info "=========================================="
    
    # 检查 clang-tidy
    if (-not (Test-ClangTidyAvailable)) {
        exit 1
    }
    
    # 查找 compile_commands.json
    $compileCommands = Find-CompileCommands -BuildDir $BuildDir
    if (-not $compileCommands) {
        exit 1
    }
    Write-Info "使用编译数据库: $compileCommands"
    
    # 检查 .clang-tidy 配置
    $clangTidyConfig = Join-Path $PWD.Path ".clang-tidy"
    if (Test-Path $clangTidyConfig) {
        Write-Info "使用配置文件: $clangTidyConfig"
    } else {
        Write-Warning "未找到 .clang-tidy 配置文件，将使用默认检查规则"
    }
    
    # 获取源文件
    $sourceFiles = Get-SourceFiles -SourceDir $SourceDir
    if ($sourceFiles.Count -eq 0) {
        Write-Error "错误：未找到源文件"
        exit 1
    }
    
    # 准备输出文件
    if ($OutputFile) {
        # 清空或创建输出文件
        "" | Out-File -FilePath $OutputFile -Encoding UTF8
        Write-Info "报告将保存到: $OutputFile"
    }
    
    # 运行分析
    $result = Invoke-ClangTidy -Files $sourceFiles -CompileCommands $compileCommands `
               -Fix:$Fix -WarningsAsErrors:$WarningsAsErrors -OutputFile $OutputFile -Verbose:$Verbose
    
    # 返回退出码
    if ($Fix) {
        Write-Info ""
        Write-Info "已自动修复部分问题，请检查代码变更"
    }
    
    if ($result.Errors -gt 0) {
        exit 1
    }
    
    exit 0
}

Main
