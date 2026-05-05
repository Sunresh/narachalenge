
@echo off

clear

:: 1. Wipe the old build folder
if exist build rd /s /q build

:: 2. Configure the project (creates the build folder)
cmake -B build -S .

:: 3. Compile the code
cmake --build build --config Release
@REM cd build
@REM cpack -G NSIS -C Release
@REM cd ..
:: 4. Launch the application
if %ERRORLEVEL% EQU 0 (
    echo Launching nara.exe...
    .\build\Release\nara.exe
) else (
    echo Build failed. Check errors above.
    pause
)
