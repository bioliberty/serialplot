@echo off
REM Run SerialPlot with correct Qt DLLs

set QT_DIR=C:\Qt\6.10.1\mingw_64
set MINGW_DIR=C:\Qt\Tools\mingw1310_64

REM Set PATH to use ONLY Qt and MinGW from the correct location
REM This prevents loading DLLs from Anaconda or other Qt installations
set PATH=%MINGW_DIR%\bin;%QT_DIR%\bin;C:\Windows\System32;C:\Windows

if exist build\serialplot_biolib.exe (
    echo Starting SerialPlot...
    cd build
    serialplot_biolib.exe
) else (
    echo ERROR: serialplot_biolib.exe not found!
    echo Please run build.bat first.
    pause
)