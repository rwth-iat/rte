# ACPLT/RTE Runtime Environment

This is the source code of a runtime environment for the process control domain.
It is developed since around 1999 at the [Chair of Process Control Engineering](http://www.plt.rwth-aachen.de) at the RWTH Aachen University.
It is well known in the German process control community under the name of the core technology ACPLT/OV.

The runtime environment is used at industry scale in the petro chemical plants at INEOS in Cologne.

## Compiling
To compile the ACPLT/RTE yourself you need to follow the steps presented here.
Currently, ACPLT/RTE can be compiled on GNU/Linux and MinGW on Windows.
Cross-compiling on Linux for Windows, using MinGW, is also possible, but more sophisticated.

On any platform, you'll need a C compiler, make, cmake, tclsh, bison and flex.

Debian/Untuntu command line:
```sh
sudo apt install build-essential tcl bison flex cmake

```

On Windows, you can setup a Cygwin or MSys environment or download the required tools separatly:
* http://mingw-w64.org/doku.php/download/mingw-builds
* https://cmake.org/download/
* https://www.magicsplat.com/tcl-installer/index.html
* https://github.com/lexxmark/winflexbison/releases
Make sure, that the binary dirctories of those tools are listed in your `%PATH%` variable.


Clone the acplt/rte project from github. Create a build directory, run cmake configuration and build:
```sh
git clone https://github.com/acplt/rte.git
mkdir build
cd build
cmake ..
make -j 4
```

On Windows with MinGW (without Cygwin), use `mingw32-make.exe` instead of `make`.


### Packaging

TODO


### Cross Compiling

TODO


## Documentation
* An API Reference can be found here: [API Reference](http://acplt.github.io/rte-www/doc/current/)
* Link to technological overview (german): [ACPLT-Technologiekonzept](https://github.com/acplt/rte-www/blob/gh-pages/doc/overview/ACPLT-Technologiekonzept.pdf)  
* Link to functionblock and sequencial-state-chart overview (german): [ACPLT-Funktionsbausteine_und_SSCs](https://github.com/acplt/rte-www/blob/gh-pages/doc/overview/Funktionsbausteine_und_SSCs.pdf)
* Old but useful documentations can be found at: [ACPLT-OV Doc old](https://github.com/acplt/rte-www/blob/gh-pages/doc/old)


## Usage

Starting the runtime environment from the build tree:
```sh
core/runtimeserver/ov_runtimeserver -f test.ovd -s MANAGER -l stdout -c 10000000 --force-create -w ksbase -w kshttp -w TCPbind -w fb
```
TODO explain command

TODO starting from *installed* package

TODO create and document a ready to use config file ("servers/MANAGER directory")


## Creating and building custom libraries

TODO
