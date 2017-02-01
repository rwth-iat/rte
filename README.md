This is the source code of a runtime environment for the process control domain. It is developed since around 1999 at the [Chair of Process Control Engineering](http://www.plt.rwth-aachen.de) at the RWTH Aachen University.
It is well known in the German process control community under the name of the core technology ACPLT/OV.

#Compiling
You need tcl, bison and flex 
tcl:
```sh
sudo apt-get install tcl
```
bison:
```sh
sudo apt-get install bison
```
flex:
```sh
sudo apt-get install flex
```
to compile the runtime environment. Navigate to build folder and run the build command:
```sh
cd build && tclsh acplt_build.tcl
```
or
```sh
cd build && tclsh acplt_build.tcl no_dbcommands 64
```
for a 64 bit build.

The runtime environment runs on Linux (x86, x64, ARM) and Windows (x86, x64).

#Documentation
An API Reference can be found here: [API Reference](http://acplt.github.io/rte-www/doc/current/)

#Releases
A Release list can be found here: [Releases](http://acplt.github.io/rte-www/releases/)

#Using the environment

##start the Runtime Environment:
build a new database file:
```sh
./build/acplt/servers/MANAGER/build_database.sh
```
start up:
```sh
./build/acplt/servers/MANAGER/start_server.sh
```
further settings(e.g., which libs should be loaded at start up) of the RTE can be made within the configuration file that can be found here
```sh
./build/acplt/servers/MANAGER/ov_server.conf
```

The runtime environment is used at industry scale in the petro chemical plants at INEOS in Cologne.

[![Build Status](https://travis-ci.org/acplt/rte.svg?branch=master)](https://travis-ci.org/acplt/rte)
