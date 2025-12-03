@echo off
REM Clean build artifacts

echo Cleaning build directory...
if exist build (
    cd build
    del /s /q * >nul 2>&1
    cd ..
    rmdir /s /q build
    echo Build directory cleaned.
) else (
    echo Build directory does not exist.
)
pause
