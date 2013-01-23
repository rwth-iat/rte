@echo off

mingw32-make -C librpc -f Makefile.mingw %1
if errorlevel 1 goto end

rem mingw32-make -C rpcgen -f Makefile.mingw %1
rem if errorlevel 1 goto end

rem mingw32-make -C service -f Makefile.mingw %1
rem if errorlevel 1 goto end

rem mingw32-make -C rpcinfo -f Makefile.mingw %1
rem if errorlevel 1 goto end

rem mingw32-make -C test -f Makefile.mingw %1

:end
