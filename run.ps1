# Run SerialPlot with correct Qt DLLs

$QT_DIR = "C:\Qt\6.10.1\mingw_64"
$MINGW_DIR = "C:\Qt\Tools\mingw1310_64"

# Set PATH to use ONLY Qt and MinGW from the correct location
# This prevents loading DLLs from Anaconda or other Qt installations
$env:PATH = "$MINGW_DIR\bin;$QT_DIR\bin;C:\Windows\System32;C:\Windows"

if (Test-Path "build\serialplot_biolib.exe") {
    Write-Host "Starting SerialPlot..." -ForegroundColor Green
    Set-Location build
    .\serialplot_biolib.exe
} else {
    Write-Host "ERROR: serialplot_biolib.exe not found!" -ForegroundColor Red
    Write-Host "Please run build.bat first."
    Read-Host "Press Enter to exit"
}