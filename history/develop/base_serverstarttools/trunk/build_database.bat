@echo off
rem dient nur als Windows-Aufrufschell für das Skript build_database.tcl
cmd /c %~dp0..\..\system\sysbin\tclsh %~dp0..\..\system\systools\build_database.tcl
PAUSE

