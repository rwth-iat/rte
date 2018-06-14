SET IFBS_HOME=%~dp0clients\iFBSpro
SET ACPLT_HOME=%~dp0acplt

SET MINGW=%~dp0MinGW
SET LIB=%MINGW%\lib
SET INCLUDE=%MINGW%\include
SET PATH=%~dp0acplt\system\addonlibs;%~dp0acplt\system\syslibs;%~dp0acplt\system\sysbin;%MINGW%\bin;%MINGW%\lib;%PATH%
#SET PATH=%~dp0acplt\system\addonlibs;%~dp0acplt\system\syslibs;%~dp0acplt\system\sysbin;%MINGW%\bin;%MINGW%\lib;%MINGW%\libexec\gcc\i686-w64-mingw32\4.8.3;%PATH%
SET CC=gcc
start cmd
