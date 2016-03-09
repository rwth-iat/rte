#!/bin/bash
cd ./rte-www/releases/
mkdir tmpSafe
echo "Deleting empty Release archives (everything smaller than 2M)"
rm -v `find . -name '*acpltRTE-*' -type f -size -2000k`
echo "Saving the 10 latest releases of everything"
cp -v `find . -name '*acpltRTE-linux32.tar.gz' -type f -printf '%T@ %p\n' | sort -n | tail -10 | cut -f2- -d" " ` tmpSafe/.
cp -v `find . -name '*acpltRTE-linux64.tar.gz' -type f -printf '%T@ %p\n' | sort -n | tail -10 | cut -f2- -d" " ` tmpSafe/.
cp -v `find . -name '*acpltRTE-win32.zip' -type f -printf '%T@ %p\n' | sort -n | tail -10 | cut -f2- -d" " ` tmpSafe/.
cp -v `find . -name '*acpltRTE-RPi.tar.gz' -type f -printf '%T@ %p\n' | sort -n | tail -10 | cut -f2- -d" " ` tmpSafe/.
echo "Deleting everything else"
rm -v `find . -name '*acpltRTE-*' -type f`
mv -v tmpSafe/* .
rm -vR tmpSafe
echo "Cleanup done."
cd ../../
