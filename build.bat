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
cmd /c mingw32-make -C src -f Makefile
:: .
:: > Executing the game
:: --------------------------
cmd /c start build\Snake.exe