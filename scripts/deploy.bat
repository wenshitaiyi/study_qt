@echo off
setlocal enabledelayedexpansion

:: Usage:
::   deploy.bat demo_widget
::   deploy.bat demo_widget Release

set TARGET=%~1
set CONFIG=%~2
if "%CONFIG%"=="" set CONFIG=Debug

if "%TARGET%"=="" (
    echo Usage: deploy.bat ^<TargetName^> [Debug^|Release]
    exit /b 1
)

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0deploy.ps1" -Target %TARGET% -Config %CONFIG%
