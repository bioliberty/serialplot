# Building SerialPlot on Windows

## Quick Start

### Option 1: Use the build script (Recommended)
1. Double-click `build.bat` or right-click and "Run with PowerShell" on `build.ps1`
2. Wait for the build to complete
3. Run `run.bat` or navigate to `build\serialplot.exe`

### Option 2: Manual build
1. Open PowerShell or Command Prompt
2. Navigate to the project directory
3. Run the following commands:

**PowerShell:**
```powershell
cd build
$env:PATH="C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.10.1\mingw_64\bin;$env:PATH"
cmake .. -DCMAKE_PREFIX_PATH="C:\Qt\6.10.1\mingw_64" -G "MinGW Makefiles"
mingw32-make -j
```

**Command Prompt:**
```batch
cd build
set PATH=C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.10.1\mingw_64\bin;%PATH%
cmake .. -DCMAKE_PREFIX_PATH="C:\Qt\6.10.1\mingw_64" -G "MinGW Makefiles"
mingw32-make -j
```

## Prerequisites

- Qt 6.10.1 (or similar version) with MinGW installed at `C:\Qt\`
- Qt SerialPort module
- Qt SVG module
- CMake
- Git (optional)

## Installation Options

### For Personal Use
1. **Desktop Shortcut**: Create a shortcut to `build\serialplot.exe`
2. **Add to PATH**: Add the build directory to your system PATH
3. **Run from build directory**: Use the provided `run.bat` script

### System-Wide Installation
Run as Administrator:
```batch
cd build
mingw32-make install
```

This installs SerialPlot to `C:\Program Files\` and adds it to the Start Menu.

## Deployment

### Portable Package
To create a portable version with all dependencies:

1. Create a deployment folder:
```batch
mkdir deploy
copy build\serialplot.exe deploy\
```

2. Run Qt's deployment tool:
```batch
set PATH=C:\Qt\6.10.1\mingw_64\bin;%PATH%
windeployqt deploy\serialplot.exe
```

3. Copy MinGW runtime DLLs:
```batch
copy C:\Qt\Tools\mingw1310_64\bin\libgcc_s_seh-1.dll deploy\
copy C:\Qt\Tools\mingw1310_64\bin\libstdc++-6.dll deploy\
copy C:\Qt\Tools\mingw1310_64\bin\libwinpthread-1.dll deploy\
```

4. The `deploy` folder now contains a portable version you can share

## Cleaning

To clean build artifacts and start fresh:
- Run `clean.bat`, or
- Manually delete the `build` directory

## Troubleshooting

### Qt not found
- Verify Qt installation path in the build scripts
- Make sure Qt SerialPort module is installed via Qt Maintenance Tool

### Build errors
- Clean the build directory and rebuild
- Check that MinGW and Qt paths are correct
- Ensure you have the latest CMake

### Missing DLLs when running
- Use `windeployqt` to copy required Qt DLLs
- Copy MinGW runtime DLLs as shown in the Deployment section

## Cross-Platform Building

### Building for Unix/Linux
You cannot cross-compile for Unix on Windows easily. Instead:
- Build on the target Linux/Unix system using the instructions in README.md
- Use WSL (Windows Subsystem for Linux) to build Linux binaries
- Use a Linux VM

### Building on Unix/Linux
See the main README.md for Linux build instructions:
```bash
mkdir build && cd build
cmake ..
make -j
```
