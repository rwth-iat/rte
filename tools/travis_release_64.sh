#!/bin/bash
# pack the linux release
echo "Packing the Linux 64bit release..."
cd ./build
rm -R ./acplt/dev
tar -pczf acpltRTE-linux64.tar.gz acplt
mv acpltRTE-linux64.tar.gz ../acpltRTE-linux64.tar.gz
cd ..
echo "Done"
