#!/bin/bash
cd ./rte-www/releases/
mkdir tmpSafe
echo "Deleting empty Release archives (everything smaller than 2M)"
rm -v `find . -maxdepth 1  -name '*acpltRTE-*' -type f -size -2000k`
echo "Saving the 10 latest releases of everything"
mv -v `find . -maxdepth 1  -name '*acpltRTE-linux32.tar.gz' -type f -printf '%T@ %p\n' | sort -n | tail -10 | cut -f2- -d" " ` tmpSafe/.
mv -v `find . -maxdepth 1  -name '*acpltRTE-linux64.tar.gz' -type f -printf '%T@ %p\n' | sort -n | tail -10 | cut -f2- -d" " ` tmpSafe/.
mv -v `find . -maxdepth 1  -name '*acpltRTE-win32.zip' -type f -printf '%T@ %p\n' | sort -n | tail -10 | cut -f2- -d" " ` tmpSafe/.
mv -v `find . -maxdepth 1  -name '*acpltRTE-RPi.tar.gz' -type f -printf '%T@ %p\n' | sort -n | tail -10 | cut -f2- -d" " ` tmpSafe/.
echo "Deleting everything else"
rm -v `find . -maxdepth 1 -name '*acpltRTE-*' -type f`
mv -v tmpSafe/* .
rm -vR tmpSafe
echo "Cleanup done."
cd ../../
