cd librpc
make -f Makefile.bc %1
if errorlevel 1 goto end
cd ..\rpcgen
make -f Makefile.bc %1
if errorlevel 1 goto end
cd ..\service
make -f Makefile.bc %1
if errorlevel 1 goto end
cd ..\rpcinfo
make -f Makefile.bc %1
if errorlevel 1 goto end
cd ..\test
make -f Makefile.bc %1
:end
cd ..
