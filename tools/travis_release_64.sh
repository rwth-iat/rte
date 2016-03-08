#!/bin/bash
# pack the linux release
echo "Packing the Linux 64bit release..."
cd ./build
tar -pczf acpltRTE-linux.tar.gz acplt
mv acpltRTE-linux64.tar.gz ../acpltRTE-linux64.tar.gz
cd ..
echo "Done"
