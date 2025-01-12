@echo off

:: Setup build folder
set BUILD_DIR=out\build\x64-Debug
set INSTALL_PREFIX=C:\vcpkg\installed\x64-windows
set HECS_PATH=C:\DEV\CppStuff\HECS

:: Check if cl.exe is available
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] cl.exe not found. Please run this script from a Developer Command Prompt.
    exit /b 1
)

:: Optional: Do a quick check of cl.exe to confirm we are in x64 environment
echo Checking compiler target...
cl > temp_cl_output.txt 2>&1
findstr /i "x86" temp_cl_output.txt >nul
if %errorlevel%==0 (
    echo [ERROR] It looks like you're in an x86 environment. Please open the x64 Developer prompt.
    del temp_cl_output.txt
    exit /b 1
)
del temp_cl_output.txt

echo Compiler ok

cd /d "%~dp0"

echo Installing HECS...
call "%HECS_PATH%\setup.bat"
echo Installed HECS

:: Clean build folder to ensure no leftover x86 caches
if exist "%BUILD_DIR%" (
    rmdir /S /Q "%BUILD_DIR%"
)

:: Configure (choose your generator)
cmake -S . -B "%BUILD_DIR%" ^
      -G "Ninja" ^
      -DCMAKE_BUILD_TYPE=Debug ^
      -DCMAKE_INSTALL_PREFIX="%INSTALL_PREFIX%"
if %errorlevel% neq 0 (
    echo Failed to configure CMake project.
    exit /b %errorlevel%
)

:: Build
cmake --build "%BUILD_DIR%" --config Debug
if %errorlevel% neq 0 (
    echo Build failed.
    exit /b %errorlevel%
)

:: Install
cmake --install "%BUILD_DIR%" --prefix "%INSTALL_PREFIX%"
if %errorlevel% neq 0 (
    echo Install failed.
    exit /b %errorlevel%
)

echo Build and installation completed successfully.
