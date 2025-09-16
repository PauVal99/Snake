::@echo off
:: > Setup required Environment
:: -------------------------------------
set COMPILER_DIR=C:\raylib\w64devkit\bin
set PATH=%PATH%;%COMPILER_DIR%
cd %~dp0
:: .
:: > Compile simple .rc file
:: ----------------------------
cmd /c windres src\snake.rc -o src\snake.rc.data
:: .
:: > Generating project
:: --------------------------
cmd /c mingw32-make -C src -f Makefile ^
PROJECT_NAME=Snake ^
PROJECT_VERSION=1.0 ^
PROJECT_DESCRIPTION="Trying to make a snake game." ^
PROJECT_INTERNAL_NAME=Snake ^
PROJECT_PLATFORM=PLATFORM_DESKTOP ^
PROJECT_BUILD_PATH="..\build"
:: .
:: > Executing the game
:: --------------------------
cmd /c start build\Snake.exe