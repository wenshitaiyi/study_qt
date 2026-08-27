@echo off
setlocal enabledelayedexpansion

:: Usage:
::   run.bat demo_console
::   run.bat demo_console Debug

set TARGET=%~1
set CONFIG=%~2
if "%CONFIG%"=="" set CONFIG=Debug

if "%TARGET%"=="" (
    echo Usage: run.bat ^<TargetName^> [Debug^|Release]
    exit /b 1
)

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0run.ps1" -Target %TARGET% -Config %CONFIG%
