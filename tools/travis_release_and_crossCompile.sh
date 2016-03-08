#!/bin/bash

# pack the linux release
cd ./build
tar -pczf acpltRTE-linux.tar.gz acplt
mv acpltRTE-linux.tar.gz ../`date +%F:%R:%S`_acpltRTE-linux.tar.gz
cd ..

# windows build
# copy local binaries
mkdir ~/localBins
cp ./build/acplt/system/sysbin/* ~/localBins/.
export CPATH=/usr/i686-w64-mingw32/include/
export PATH=/home/travis/localBins:$PATH
ls -lah ~
find ~ -name 'ov_codegen'
pwd
ls -lah ~/localBins
file ~/localBins/*
echo $PATH
which ov_codegen
cd ./build
rm -R acplt acplt.build
tclsh acplt_build.tcl cross windows32.cross
tail -n 100 acplt_build.log
zip -r acpltRTE-win32.zip acplt
mv acpltRTE-win32.zip ../`date +%F:%R:%S`_acpltRTE-win32.zip
cd ..
