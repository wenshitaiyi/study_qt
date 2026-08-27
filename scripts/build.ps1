<#
.SYNOPSIS
    Qt Study Project Build Script (PowerShell)
.DESCRIPTION
    Builds the whole project or a specified demo target using CMake and MSVC.
.PARAMETER Target
    Specific demo target to build (e.g. demo_console). Default is empty (build all).
.PARAMETER Config
    Build configuration: Debug (default) or Release.
.PARAMETER Clean
    If specified, deletes the build directory before configuring.
.EXAMPLE
    .\scripts\build.ps1
    .\scripts\build.ps1 -Target demo_console
    .\scripts\build.ps1 -Config Release
    .\scripts\build.ps1 -Clean
#>

[CmdletBinding()]
param(
    [Parameter(Position = 0)]
    [string]$Target = "",

    [Parameter(Position = 1)]
    [ValidateSet("Debug", "Release")]
    [string]$Config = "Debug",

    [switch]$Clean
)

$ErrorActionPreference = "Stop"

# Host environment paths
$CMakeExe = "D:\app\CMake\bin\cmake.exe"
$QtPrefix = "D:\app\qt\qt-5.15.2\5.15.2\msvc2019_64"

# Fallback to PATH if fixed path not found
if (-not (Test-Path $CMakeExe)) {
    $CMakeCmd = Get-Command cmake.exe -ErrorAction SilentlyContinue
    if ($CMakeCmd) {
        $CMakeExe = $CMakeCmd.Source
    } else {
        Write-Error "CMake executable not found at '$CMakeExe' or in system PATH."
    }
}

# Resolve project root
$RootDir = Split-Path -Parent $PSScriptRoot
$BuildDir = Join-Path $RootDir "build"

# Clean if requested
if ($Clean) {
    if (Test-Path $BuildDir) {
        Write-Host "Cleaning build directory: $BuildDir..." -ForegroundColor Yellow
        Remove-Item -Recurse -Force $BuildDir
    }
}

# Normalize PATH environment for MSVC if needed
[System.Environment]::SetEnvironmentVariable('Path', $null, 'Process')
[System.Environment]::SetEnvironmentVariable('PATH', $env:Path, 'Process')

# Configure if build directory doesn't have CMakeCache.txt
$CMakeCache = Join-Path $BuildDir "CMakeCache.txt"
if (-not (Test-Path $CMakeCache)) {
    Write-Host "=== Configuring Project with CMake ===" -ForegroundColor Cyan
    $ConfigArgs = @(
        "-S", $RootDir,
        "-B", $BuildDir,
        "-A", "x64",
        "-T", "v143"
    )
    if (Test-Path $QtPrefix) {
        $ConfigArgs += "-DCMAKE_PREFIX_PATH=$QtPrefix"
    }

    & $CMakeExe @ConfigArgs
    if ($LASTEXITCODE -ne 0) {
        Write-Error "CMake configuration failed with exit code $LASTEXITCODE."
    }
}

# Build target
Write-Host "=== Building Target: $(if ($Target) { $Target } else { 'ALL_BUILD' }) [$Config] ===" -ForegroundColor Cyan

$BuildArgs = @(
    "--build", $BuildDir,
    "--config", $Config
)

if ($Target -ne "") {
    $BuildArgs += @("--target", $Target)
}

& $CMakeExe @BuildArgs
if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed with exit code $LASTEXITCODE."
}

Write-Host "=== Build Completed Successfully! ===" -ForegroundColor Green
$OutDir = Join-Path $RootDir "app\$($Config.ToLower())"
Write-Host "Output Directory: $OutDir" -ForegroundColor Gray
