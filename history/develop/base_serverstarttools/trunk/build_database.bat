@echo off
rem dient nur als Windows-Aufrufschell f�r das Skript build_database.tcl
cmd /c %~dp0..\..\system\sysbin\tclsh %~dp0..\..\system\systools\build_database.tcl
PAUSE

