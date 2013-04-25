#!/bin/bash

#wrapping shell to start a tcl script
#checking tcl
if [ -z $(which tclsh) ]; then
    echo "tclsh not found, please install it using e.g. sudo apt-get install tcl8.5"
	exit 1
fi
tclsh ../../system/systools/build_database.tcl
echo "Press any key to exit... "
read key
exit 0

