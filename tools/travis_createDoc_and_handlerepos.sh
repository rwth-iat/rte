#!/bin/bash

# starting in rte

if [ ${OV_ARCH_BITWIDTH} == "32" ]; then bash ./tools/travis_release_and_crossCompile.sh ; fi
if [ ${OV_ARCH_BITWIDTH} == "64" ]; then bash ./tools/travis_release_64.sh ; fi
cd ..
# now in acplt

echo "Moving everything to rte-www"
if [ ${OV_ARCH_BITWIDTH} == "32" ]; then
  cp ./rte/acpltRTE-linux32.tar.gz ./rte-www/releases/`date +%F-%R`-${TAG}_acpltRTE-linux32.tar.gz
  cp ./rte/acpltRTE-win32.zip ./rte-www/releases/`date +%F-%R`-${TAG}_acpltRTE-win32.zip
  cp ./rte/acpltRTE-RPi.tar.gz ./rte-www/releases/`date +%F-%R`-${TAG}_acpltRTE-RPi.tar.gz
fi
if [ ${OV_ARCH_BITWIDTH} == "64" ]; then
  cp ./rte/acpltRTE-linux64.tar.gz ./rte-www/releases/`date +%F-%R`-${TAG}_acpltRTE-linux64.tar.gz
fi

if [ ${OV_ARCH_BITWIDTH} == "32" ]; then
  bash ./rte/tools/travis_cleanReleaseDir.sh
  # change to rte
  cd ./rte
  echo "Creating docs"
  mkdir html
  makeinfo --html --force doc/libov.texi -o html/ --no-warn
  
  # change to rte-www
  cd ../rte-www
  git rm -r -f ./doc/current/*
  mkdir doc
  cd doc
  mkdir current
  cd ..
  # back in rte-www
  cp -r ../rte/html/* ./doc/current/
  git add -A ./doc/current/*
  cd ..
  # back in acplt
fi

#change to rte-www
cd rte-www
git add -A ./releases/*
cd ..
