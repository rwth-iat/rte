#!/bin/bash
cd ./rte-www/releases/
mkdir tmpSafe
cp fb_dbcommands.exe tmpSafe/fb_dbcommands.exe
echo "Deleting empty Release archives (everything smaller than 2M)"
rm `find . -type f -size -2000k`
echo "Saving the 10 latest releases of everything"
cp `find . -name '*-linux32.tar.gz' -type f -printf '%T@ %p\n' | sort -n | tail -10 | cut -f2- -d" " ` tmpSafe/.
cp `find . -name '*-linux64.tar.gz' -type f -printf '%T@ %p\n' | sort -n | tail -10 | cut -f2- -d" " ` tmpSafe/.
cp `find . -name '*-win32.zip' -type f -printf '%T@ %p\n' | sort -n | tail -10 | cut -f2- -d" " ` tmpSafe/.
cp `find . -name '*-RPi.tar.gz' -type f -printf '%T@ %p\n' | sort -n | tail -10 | cut -f2- -d" " ` tmpSafe/.
echo "Deleting everything else"
rm `find . -type f`
mv tmpSafe/* .
rm -R tmpSafe
echo "Cleanup done."
cd ../../
