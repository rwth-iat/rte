@echo off
rem *******************************************************************
rem Dient nur als Windows-Aufrufschell f�r das Skript start_server.tcl
rem
rem Muss direkt im Verzeichnis des zu startenden Servers liegen
rem
rem ********************************************************************
rem

"%~dp0..\..\system\sysbin\tclsh" "%~dp0..\..\system\systools\start_server.tcl"

pause
