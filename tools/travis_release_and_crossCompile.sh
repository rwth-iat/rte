#!/bin/bash

# pack the linux release
cd ./build
tar -pczf acpltRTE-linux.tar.gz acplt
mv acpltRTE-linux.tar.gz ../`date +%F:%R:%S`_acpltRTE-linux.tar.gz
