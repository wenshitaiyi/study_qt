@echo off
setlocal enabledelayedexpansion

:: Usage:
::   build.bat                 (Build all targets, Debug)
::   build.bat demo_console    (Build specific target, Debug)
::   build.bat demo_console Release (Build specific target, Release)
::   build.bat clean           (Clean build folder)

set TARGET=%~1
set CONFIG=%~2
if "%CONFIG%"=="" set CONFIG=Debug

if /i "%TARGET%"=="clean" (
    powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0build.ps1" -Clean
    goto :eof
)

if "%TARGET%"=="" (
    powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0build.ps1" -Config %CONFIG%
) else (
    powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0build.ps1" -Target %TARGET% -Config %CONFIG%
)
