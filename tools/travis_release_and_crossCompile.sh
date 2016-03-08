#!/bin/bash

# pack the linux release
cd ./build
tar -pczf acpltRTE-linux.tar.gz acplt
mv acpltRTE-linux32.tar.gz ../acpltRTE-linux32.tar.gz
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
zip -qr acpltRTE-win32.zip acplt
mv acpltRTE-win32.zip ../acpltRTE-win32.zip
cd ..

# raspberry pi build
# get toolchain
mkdir ~/RaspberryTools
cd ~/RaspberryTools
git clone https://github.com/raspberrypi/tools
cd ~/build/acplt/rte/build
rm -R acplt acplt.build
export CPATH=/home/travis/RaspberryTools/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/lib/gcc/arm-linux-gnueabihf/4.8.3/include/
export PATH=/home/travis/RaspberryTools/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/:$PATH
tclsh acplt_build.tcl cross raspberryPi.cross
tail -n 100 acplt_build.log
tar -pczf acpltRTE-RPi.tar.gz acplt
mv acpltRTE-RPi.tar.gz ../acpltRTE-RPi.tar.gz
cd ..



