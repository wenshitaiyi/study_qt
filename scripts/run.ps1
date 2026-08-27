<#
.SYNOPSIS
    Run a built Qt demo executable.
.PARAMETER Target
    Name of the target executable (e.g. demo_console).
.PARAMETER Config
    Configuration: Debug (default) or Release.
.PARAMETER Args
    Arguments to pass to the target executable.
#>

[CmdletBinding()]
param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Target,

    [Parameter(Position = 1)]
    [ValidateSet("Debug", "Release")]
    [string]$Config = "Debug",

    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]]$Args
)

$RootDir = Split-Path -Parent $PSScriptRoot
$QtBin = "D:\app\qt\qt-5.15.2\5.15.2\msvc2019_64\bin"

# Set runtime PATH
if (Test-Path $QtBin) {
    $env:PATH = "$QtBin;$env:PATH"
}

$ExePath = Join-Path $RootDir "app\$($Config.ToLower())\$Target.exe"

if (-not (Test-Path $ExePath)) {
    Write-Warning "Executable not found at '$ExePath'. Attempting to build it first..."
    & (Join-Path $PSScriptRoot "build.ps1") -Target $Target -Config $Config
}

if (Test-Path $ExePath) {
    Write-Host "Running: $ExePath $Args" -ForegroundColor Cyan
    & $ExePath @Args
} else {
    Write-Error "Failed to locate executable: $ExePath"
}
