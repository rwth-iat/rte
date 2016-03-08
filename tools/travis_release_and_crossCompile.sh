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
cd ./build
rm -R acplt acplt.build
tclsh acplt_build.tcl cross windows32.cross
zip -r acpltRTE-win32.zip acplt
mv acpltRTE-win32.zip ../`date +%F:%R:%S`_acpltRTE-win32.zip
cd ..

# raspberry pi build
# get toolchain
mkdir ~/RaspberryTools
cd ~/RaspberryTools
git clone https://github.com/raspberrypi/tools
cd ~/build/rte/build
rm -R acplt acplt.build
export CPATH=/home/travis/RaspberryTools/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.8.3/include/
export PATH=/home/travis/RaspberryTools/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/:$PATH
tclsh acplt_build.tcl cross raspberryPi.cross
tar -pczf acpltRTE-RPi.tar.gz acplt
mv acpltRTE-RPi.tar.gz ../`date +%F:%R:%S`_acpltRTE-RPi.tar.gz
cd ..



