@echo off
echo "Installing ONC/RPC for NT..."
..\bin\inst_pm remove
copy ..\bin\oncrpc.dll %SystemRoot%\system32
copy ..\bin\portmap.exe %SystemRoot%\system32
if exist %SystemRoot%\system32\drivers\etc\rpc goto inst_pm
mkdir %SystemRoot%\system32\drivers\etc
copy ..\etc\rpc %SystemRoot%\system32\drivers\etc
:inst_pm
..\bin\inst_pm %SystemRoot%\system32\portmap.exe
echo "Done."
