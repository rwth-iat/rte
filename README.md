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

Instrcutions for MSys2 (mingw-w64):

Install the msys2 and update the base installation.
```sh
pacman -Syu
```
Follow the instructions that are provided.

Now install the required packages in the msys2 environment.
Here we choose the 64-bit mingw-w64 toolchain.
```sh
pacman -S tcl flex bison mingw-w64-x86_64-toolchain
```

Add the ``$MSYS2_ROOT$\usr\bin`` directory to the PATH (either system wide or user).
The Toolchain environment is places in ``MSYS2_ROOT\mingw64``.

TODO Setup with Cygwin

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


### Packaging / Installing

Using the CMake install feature, we can pack or install the ACPLT/OV runtime executable together with the libraries for dynamic linking and templates/example files of the libraries.
To do so, we must specify the install prefix (`CMAKE_INSTALL_PREFIX`) in the CMake configuration step and run the installation with `make install`.
The install prefix may be set to `/usr` or `/usr/local` for direct install on Linux or to a relative directory path (e.g. `export/`) for packing the install tree in a local directory:

```sh
cd build
cmake -DCMAKE_INSTALL_PREFIX=export/ ..
make -j 4 install
```


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
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/gcc-linux-armhf.cmake -DOV_CODEGEN_PATH="../build/core/codegen/ov_codegen" ..
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

Unfortunately, Windows executables don't support something like `RPATH`.
To test ov from the build tree all build artefacts need to be moved to a single directory.
This can be achived by adding following lines to the CMakeList.txt in the project root directory.
```
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "" )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "" )
```
This creates a directory out in the cmake build directory where all executables and shared libraries are moved.

TODO starting from *installed* package

TODO create and document a ready to use config file ("servers/MANAGER directory")



## Creating and building custom libraries

TODO
