# ACPLT/RTE Runtime Environment

This is the source code of a runtime environment for the process control domain.
It is developed since around 1999 at the [Chair of Process Control Engineering](http://www.plt.rwth-aachen.de) at the RWTH Aachen University.
It is well known in the German process control community under the name of the core technology ACPLT/OV.

The runtime environment is used at industry scale in the petro chemical plants at INEOS in Cologne.



## Building ACPLT/OV

### Prerquisites

To compile the ACPLT/RTE yourself you need to follow the steps presented here.
Currently, ACPLT/RTE can be compiled on GNU/Linux and MinGW on Windows.
Cross-compiling on Linux for Windows is also possible, using the MinGW compilers.

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

TODO Setup with MSYS2; Cygwin

### Compilation

Clone the acplt/rte project from github. Create a build directory, run cmake configuration and build:
```sh
git clone https://github.com/acplt/rte.git
cd rte
mkdir build
cd build
cmake ..
make -j 4
```

On Windows with MinGW (without Cygwin), use `mingw32-make.exe` instead of `make`.


### Packaging

TODO

### Cross Compiling

Cross compilation is possible and tested on Linux, especially using `arm-linux-gnueabihf-gcc` to target Raspberri Pi and similar platforms or using `x86_64-w64-mingw32-cpp` to target Windows systems.
CMake toolchain files for those target platforms are provided in the repository.
However, cross compilation requires additional steps to make the code generation tool available on the host platform before starting the build:

At first, a build tree for the host platform has to be set up. It is used to compile only the make target `ov_codegen`.
Afterwards, a new build tree for the target platform is set up, using the CMake variables `CMAKE_TOOLCHAIN_FILE` to select the compilation toolchain and `OV_CODEGEN_PATH` to provide the path to the previously compiled ov_codegen tool.

```sh
git clone https://github.com/acplt/rte.git
cd rte
mkdir build
cd build
cmake ..
make -j 4 ov_codegen

cd ..
mkdir build-win32
cd build-win32
cmake cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/gcc-linux-armhf.cmake -DOV_CODEGEN_PATH="../build/core/codegen/ov_codegen" ..
make -j 4 ov_codegen
```



## Documentation

* An API Reference can be found here: [API Reference](http://acplt.github.io/rte-www/doc/current/)
* Link to technological overview (german): [ACPLT-Technologiekonzept](https://github.com/acplt/rte-www/blob/gh-pages/doc/overview/ACPLT-Technologiekonzept.pdf)  
* Link to functionblock and sequencial-state-chart overview (german): [ACPLT-Funktionsbausteine_und_SSCs](https://github.com/acplt/rte-www/blob/gh-pages/doc/overview/Funktionsbausteine_und_SSCs.pdf)
* Old but useful documentations can be found at: [ACPLT-OV Doc old](https://github.com/acplt/rte-www/blob/gh-pages/doc/old)



## Usage

On Linux, thanks to the `RPATH` set by CMake, the `ov_runtimeserver` can be started directly from the build tree and will find all the shared libraries, compiled in the same build setup. 
```sh
core/runtimeserver/ov_runtimeserver -f test.ovd -s MANAGER -l stdout -c 10000000 --force-create -w ksbase -w kshttp -w TCPbind -w fb
```
The parameters shown above will make the `ov_runtimeserver` create a new database file (`--force-create`) of size 10 MB (`-c`) named `test.ovd` (`-f`), start a server named "MANAGER" (`-s`) with that database, and load the shared libraries `ksbase`, `kshttp`, `TCPbind` and `fb`, which will make it accessible via TCP port 7509 and provide basic functionality for function block networks.

Unfortunately, Windows executables don't support something like `RPATH`. Thus, the runtimeserver can only be started from the build tree if the build directory of every required .dll library is added to the %PATH%.

TODO starting from *installed* package

TODO create and document a ready to use config file ("servers/MANAGER directory")



## Creating and building custom libraries

TODO
