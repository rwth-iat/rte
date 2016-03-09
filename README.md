This is the source code of a runtime environment for the process control domain. It is developed since around 1999 at the [Chair of Process Control Engineering](http://www.plt.rwth-aachen.de) at the RWTH Aachen University.
It is well known in the German process control community under the name of the core technology ACPLT/OV.

#Compiling
You need tcl, bison, flex for compiling the runtime environment. Just change into the build directory and call 
`tclsh ./acplt_build.tcl no_dbcommands 64`
for a 64 bit build.

The runtime environment runs on Linux (x86, x64, ARM) and Windows (x86, x64).

#Documentation
An API Reference can be found here: [API Reference](http://acplt.github.io/rte-www/doc/current/)

#Releases
A Release list can be found here: [Releases](http://acplt.github.io/rte-www/releases/)

#Using the environment
TODO

The runtime environment is used at industry scale in the petro chemical plants at INEOS in Cologne.

[![Build Status](https://travis-ci.org/acplt/rte.svg?branch=master)](https://travis-ci.org/acplt/rte)
