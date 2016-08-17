#!/bin/sh

cd librpc
make -f makefile.linux $*
#cd ../rpcgen
#make -f makefile.linux
#cd ../service
#make -f makefile.linux
#cd ../rpcinfo
#make -f makefile.linux
#cd ../test
#make -f makefile.linux
cd ..
