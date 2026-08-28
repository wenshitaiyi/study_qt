<#
.SYNOPSIS
    Deploy Qt runtime dependencies for a built executable using windeployqt.
.DESCRIPTION
    Uses windeployqt to automatically copy required Qt runtime DLLs, plugins (platforms, styles, etc.)
    and configuration files to the executable's output directory.
.PARAMETER Target
    Name of the target executable (e.g. demo_console, demo_widget).
.PARAMETER Config
    Configuration: Debug (default) or Release.
.EXAMPLE
    .\scripts\deploy.ps1 demo_widget
    .\scripts\deploy.ps1 demo_widget Release
#>

[CmdletBinding()]
param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Target,

    [Parameter(Position = 1)]
    [ValidateSet("Debug", "Release")]
    [string]$Config = "Debug"
)

$ErrorActionPreference = "Stop"

$RootDir = Split-Path -Parent $PSScriptRoot
$QtBin = "D:\app\qt\qt-5.15.2\5.15.2\msvc2019_64\bin"
$WinDeployQt = Join-Path $QtBin "windeployqt.exe"

if (-not (Test-Path $WinDeployQt)) {
    $DeployCmd = Get-Command windeployqt.exe -ErrorAction SilentlyContinue
    if ($DeployCmd) {
        $WinDeployQt = $DeployCmd.Source
    } else {
        Write-Error "windeployqt.exe not found at '$WinDeployQt' or in system PATH."
    }
}

$ExePath = Join-Path $RootDir "app\$($Config.ToLower())\$Target.exe"

if (-not (Test-Path $ExePath)) {
    Write-Warning "Executable not found at '$ExePath'. Building target first..."
    & (Join-Path $PSScriptRoot "build.ps1") -Target $Target -Config $Config
}

if (-not (Test-Path $ExePath)) {
    Write-Error "Cannot deploy: Executable not found at $ExePath"
}

Write-Host "=== Deploying Qt Runtime with windeployqt ===" -ForegroundColor Cyan
Write-Host "Target: $ExePath" -ForegroundColor Gray

# Set PATH so windeployqt can resolve dependent tools
$env:PATH = "$QtBin;$env:PATH"

$DeployArgs = @(
    $ExePath,
    "--no-translations",
    "--compiler-runtime"
)

if ($Config -eq "Debug") {
    $DeployArgs += "--debug"
} else {
    $DeployArgs += "--release"
}

& $WinDeployQt @DeployArgs
if ($LASTEXITCODE -ne 0) {
    Write-Error "windeployqt failed with exit code $LASTEXITCODE."
}

Write-Host "=== Deployment Completed Successfully! ===" -ForegroundColor Green
Write-Host "All necessary Qt DLLs and plugins have been deployed to: $(Split-Path -Parent $ExePath)" -ForegroundColor Gray
