This is the source code of a runtime environment for the process control domain. It is developed since around 1999 at the [Chair of Process Control Engineering](http://www.plt.rwth-aachen.de) at the RWTH Aachen University.
It is well known in the German process control community under the name of the core technology ACPLT/OV.

# Compiling
On Linux you need tcl, bison, flex:
```sh
sudo apt-get install tcl bison flex

```
You also need a C/C++ compiler:
```sh
sudo apt-get install gcc g++
```
to compile the runtime environment. Clone the acplt/rte project from github, navigate to build folder and start the build process:
```sh
git clone https://github.com/acplt/rte.git
cd rte/build && make
```
Your architecture is detected automatically via tcl script. Otherwise you can specify it by calling the buildscript in the build directory yourself:
```sh
tclsh acplt_build.tcl 64 no_dbcommands
```
The runtime environment runs on Linux (x86, x64, ARM) and Windows (x86, x64).
For cross compilation see [README_CROSSCOMPILATION.md](http://acplt.github.io/rte/build/README_CROSSCOMPILATION.md).

# Documentation
* An API Reference can be found here: [API Reference](http://acplt.github.io/rte-www/doc/current/)
* Link to technological overview (german): [ACPLT-Technologiekonzept](https://github.com/acplt/rte-www/blob/gh-pages/doc/overview/ACPLT-Technologiekonzept.pdf)  
* Link to functionblock and sequencial-state-chart overview (german): [ACPLT-Funktionsbausteine_und_SSCs](https://github.com/acplt/rte-www/blob/gh-pages/doc/overview/Funktionsbausteine_und_SSCs.pdf)
* Old but useful documentations can be found at: [ACPLT-OV Doc old](https://github.com/acplt/rte-www/blob/gh-pages/doc/old)

## releases
Here you'll find the latest 10 releases (of the 10 latest building commits) for linux (32 and 64 bit), windows 32 and Raspberry Pi: [Releases](http://acplt.github.io/rte-www/releases/)

# Using the environment

## start the Runtime Environment:
build a new database file:
```sh
cd &&./build/acplt/servers/MANAGER/build_database.sh
```
start up:
```sh
./build/acplt/servers/MANAGER/start_server.sh
```
further settings(e.g., which libs should be loaded at start up) of the RTE can be made within the configuration file that can be found here
```sh
/build/acplt/servers/MANAGER/ov_server.conf
```

The runtime environment is used at industry scale in the petro chemical plants at INEOS in Cologne.

[![Build Status](https://travis-ci.org/acplt/rte.svg?branch=master)](https://travis-ci.org/acplt/rte)

