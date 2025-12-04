@echo off
REM SerialPlot Build Script for Windows
REM This script builds SerialPlot using Qt 6.10.1 and MinGW

echo ========================================
echo Building SerialPlot
echo ========================================

REM Set Qt and MinGW paths
set QT_DIR=C:\Qt\6.10.1\mingw_64
set MINGW_DIR=C:\Qt\Tools\mingw1310_64
set PATH=%MINGW_DIR%\bin;%QT_DIR%\bin;%PATH%

REM Create build directory if it doesn't exist
if not exist build mkdir build
cd build

REM Configure with CMake
echo.
echo [1/3] Configuring with CMake...
cmake .. -DCMAKE_PREFIX_PATH="%QT_DIR%" -G "MinGW Makefiles" -DPROGRAM_NAME="serialplot_biolib"
if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake configuration failed!
    pause
    exit /b 1
)

REM Build
echo.
echo [2/3] Building...
mingw32-make -j%NUMBER_OF_PROCESSORS%
if %ERRORLEVEL% neq 0 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

REM Deploy Qt DLLs
echo.
echo [3/4] Deploying Qt dependencies...
windeployqt serialplot_biolib.exe --no-translations
if %ERRORLEVEL% neq 0 (
    echo WARNING: windeployqt failed, but executable may still work
)

REM Create desktop shortcut
echo.
echo [4/5] Creating desktop shortcut...
powershell -Command "$WS = New-Object -ComObject WScript.Shell; $SC = $WS.CreateShortcut([Environment]::GetFolderPath('Desktop') + '\SerialPlot (MM DEV).lnk'); $SC.TargetPath = '%CD%\serialplot_biolib.exe'; $SC.WorkingDirectory = '%CD%'; $SC.Description = 'SerialPlot MM Dev Version'; $SC.Save()"
if %ERRORLEVEL% equ 0 echo Desktop shortcut created successfully

REM Success
echo.
echo [5/5] Build completed successfully!
echo.
echo Executable location: %CD%\serialplot_biolib.exe
echo.
echo To run: serialplot.exe or use run.bat
echo To install system-wide: mingw32-make install (requires admin)
echo.
pause
