# SerialPlot Build Script for Windows (PowerShell)
# This script builds SerialPlot using Qt 6.10.1 and MinGW

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Building SerialPlot" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

# Set Qt and MinGW paths
$QT_DIR = "C:\Qt\6.10.1\mingw_64"
$MINGW_DIR = "C:\Qt\Tools\mingw1310_64"
$env:PATH = "$MINGW_DIR\bin;$QT_DIR\bin;$env:PATH"

# Create build directory if it doesn't exist
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}
Set-Location build

# Configure with CMake
Write-Host ""
Write-Host "[1/3] Configuring with CMake..." -ForegroundColor Yellow
cmake .. -DCMAKE_PREFIX_PATH="$QT_DIR" -G "MinGW Makefiles" -DPROGRAM_NAME="serialplot_biolib"
if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: CMake configuration failed!" -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit 1
}

# Build
Write-Host ""
Write-Host "[2/3] Building..." -ForegroundColor Yellow
mingw32-make "-j$env:NUMBER_OF_PROCESSORS"
if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Build failed!" -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit 1
}

# Deploy Qt DLLs
Write-Host ""
Write-Host "[3/4] Deploying Qt dependencies..." -ForegroundColor Yellow
windeployqt serialplot_biolib.exe --no-translations
if ($LASTEXITCODE -ne 0) {
    Write-Host "WARNING: windeployqt failed, but executable may still work" -ForegroundColor Yellow
}

# Create desktop shortcut
Write-Host ""
Write-Host "[4/5] Creating desktop shortcut..." -ForegroundColor Yellow
$DesktopPath = [Environment]::GetFolderPath("Desktop")
$ShortcutPath = Join-Path $DesktopPath "SerialPlot (MM DEV).lnk"
$WScriptShell = New-Object -ComObject WScript.Shell
$Shortcut = $WScriptShell.CreateShortcut($ShortcutPath)
$Shortcut.TargetPath = "$PWD\serialplot_biolib.exe"
$Shortcut.WorkingDirectory = "$PWD"
$Shortcut.Description = "SerialPlot MM Dev Version"
$Shortcut.Save()
Write-Host "Desktop shortcut created: $ShortcutPath" -ForegroundColor Green

# Success
Write-Host ""
Write-Host "[5/5] Build completed successfully!" -ForegroundColor Green
Write-Host ""
Write-Host "Executable location: $PWD\serialplot_biolib.exe" -ForegroundColor Cyan
Write-Host ""
Write-Host "To run: .\serialplot.exe or use run.bat" -ForegroundColor White
Write-Host "To install system-wide: mingw32-make install (requires admin)" -ForegroundColor White
Write-Host ""
Read-Host "Press Enter to exit"
