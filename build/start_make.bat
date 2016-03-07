@echo off
rem set ACPLTDEV_PATH to the AcpltDevelopment kit directory
rem it is assumed that MinGW and tools are located in this directory
set ACPLTDEV_PATH=%~dp0..\..\..\

SET LIB=%ACPLTDEV_PATH%MinGW32\lib
SET MINGW=%ACPLTDEV_PATH%MinGW32
SET INCLUDE=%ACPLTDEV_PATH%MinGW32\include
set BISON_SIMPLE=%ACPLTDEV_PATH%tools\share\bison.simple
SET PATH=%ACPLTDEV_PATH%MinGW32\bin;%ACPLTDEV_PATH%MinGW32\lib;%ACPLTDEV_PATH%tools\bin;%PATH%

make %1
pause
