@echo off
rem *******************************************************************
rem Dient nur als Windows-Aufrufschell für das Skript start_server.tcl
rem
rem Muss direkt im Verzeichnis des zu startenden Servers liegen
rem
rem ********************************************************************
rem

cmd /c ..\..\system\sysbin\tclsh ..\..\system\systools\start_server.tcl

pause
