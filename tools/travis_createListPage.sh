#!/bin/bash

WIN32RELEASES=`find . -maxdepth 1  -name '*acpltRTE-win32.zip' -type f -printf '%T@ %p\n' | sort -rn | tail -10 | cut -f2- -d" "`
LINUX32RELEASES=`find . -maxdepth 1 -name '*acpltRTE-linux32.tar.gz' -type f -printf '%T@ %p\n' | sort -rn | tail -10 | cut -f2- -d" " `
LINUX64RELEASES=`find . -maxdepth 1 -name '*acpltRTE-linux64.tar.gz' -type f -printf '%T@ %p\n' | sort -rn | tail -10 | cut -f2- -d" " `
RPIRELEASES=`find . -maxdepth 1 -name '*acpltRTE-RPi.tar.gz' -type f -printf '%T@ %p\n' | sort -rn | tail -10 | cut -f2- -d" " `

rm table1.txt
touch table1.txt
for rel in ${WIN32RELEASES}; do
  DATE=`echo $rel | cut -f1-4 -d'-' | cut -c3-`
  TAG=`echo $rel | cut -f5 -d'-' | cut -f1 -d'_'`
  NAME=`echo $rel | cut -c3-`
  RELSIZE=`du -h $rel | cut -f1`
  echo "<tr><td><a href='./$rel'>$NAME</a></td><td>$TAG</td><td>$DATE</td><td>$RELSIZE</td></tr>" >> table1.txt
done

rm table2.txt
touch table2.txt
for rel in ${LINUX32RELEASES}; do
  DATE=`echo $rel | cut -f1-4 -d'-' | cut -c3-`
  TAG=`echo $rel | cut -f5 -d'-' | cut -f1 -d'_'`
  NAME=`echo $rel | cut -c3-`
  RELSIZE=`du -h $rel | cut -f1`
  echo "<tr><td><a href='./$rel'>$NAME</a></td><td>$TAG</td><td>$DATE</td><td>$RELSIZE</td></tr>" >> table2.txt
done

rm table3.txt
touch table3.txt
for rel in ${LINUX64RELEASES}; do
  DATE=`echo $rel | cut -f1-4 -d'-' | cut -c3-`
  TAG=`echo $rel | cut -f5 -d'-' | cut -f1 -d'_'`
  NAME=`echo $rel | cut -c3-`
  RELSIZE=`du -h $rel | cut -f1`
  echo "<tr><td><a href='./$rel'>$NAME</a></td><td>$TAG</td><td>$DATE</td><td>$RELSIZE</td></tr>" >> table3.txt
done

rm table4.txt
touch table4.txt
for rel in ${RPIRELEASES}; do
  DATE=`echo $rel | cut -f1-4 -d'-' | cut -c3-`
  TAG=`echo $rel | cut -f5 -d'-' | cut -f1 -d'_'`
  NAME=`echo $rel | cut -c3-`
  RELSIZE=`du -h $rel | cut -f1`
  echo "<tr><td><a href='./$rel'>$NAME</a></td><td>$TAG</td><td>$DATE</td><td>$RELSIZE</td></tr>" >> table4.txt
done

cat head1.txt table1.txt head2.txt table2.txt head3.txt table3.txt head4.txt table4.txt foot.txt > index.html
