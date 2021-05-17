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

Executables (like the ov_runtimeserver) will be placed in `bin/` and the OV libraries in`lib/`, inside the prefix directory.
Some OV libraries will install additional files like .fbd template files to the install prefix, typically in `templates/`.
When building for Windows platforms, the OV libraries' DLL files will be added to the `bin` directory.
This convention will ensure that the ov_runtimeserver will find the OV libraries for dynamic linking.


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


### Static linking

TODO


## Documentation

* An API Reference can be found here: [API Reference](http://acplt.github.io/rte-www/doc/current/)
* Link to technological overview (german): [ACPLT-Technologiekonzept](https://github.com/acplt/rte-www/blob/gh-pages/doc/overview/ACPLT-Technologiekonzept.pdf)  
* Link to functionblock and sequencial-state-chart overview (german): [ACPLT-Funktionsbausteine_und_SSCs](https://github.com/acplt/rte-www/blob/gh-pages/doc/overview/Funktionsbausteine_und_SSCs.pdf)
* Old but useful documentations can be found at: [ACPLT-OV Doc old](https://github.com/acplt/rte-www/blob/gh-pages/doc/old)



## Usage

The main application of ACPLT/RTE is the `ov_runtimeserver` / `ov_runtimeserver.exe`.
It manages the object database and dyanmically links with OV libraries which provide functionality by adding object classes.
With the help of `TCPbind`, `ksbase` and `kshttp` OV libraries (which are bundled in this repository), it allows to provide an ACPLT/KS server.
Alternatively, the `opcua` OV library (also bundled in this repository) can be used to run an OPC UA / IEC 62541 server in OV.

The ov_runtimeserver can either be configured by comand line arguments or using a configuration file.
A simple invocation for starting an OV server with a plain new object database would be
```sh
ov_runtimeserver -f test.ovd -s MANAGER -l stdout -c 10000000 --force-create -w ksbase -w kshttp -w TCPbind -w fb
```

The parameters will make the `ov_runtimeserver` create a new database file (`--force-create`) of size 10 MB (`-c`) named `test.ovd` (`-f`), start a server named "MANAGER" (`-s`) with that database, and load the OV libraries `ksbase`, `kshttp`, `TCPbind` and `fb`, which will make it accessible via TCP port 7509 and provide basic functionality for function block networks.

As an alternative, these configuration options can be supplied via configuration file.
An example configuration file is provided in [misc/MANAGER.conf](misc/MANAGER.conf) in this repository.
The path to the configuration file is specified via the `-cf` command line option.


### Running from build tree

On Linux, thanks to the `RPATH` set by CMake, the `ov_runtimeserver` can be started directly from the build tree and will find all the shared libraries which have been compiled in the same build setup. 
```sh
core/runtimeserver/ov_runtimeserver -cf MANAGER.conf
```

Unfortunately, Windows executables don't support something like `RPATH`.
To test ov from the build tree all build artefacts need to be moved to a single directory.
This can be achived by adding following lines to the CMakeList.txt in the project root directory.
```
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "" )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "" )
```
This creates a directory out in the cmake build directory where all executables and shared libraries are moved.


### Running from installed package

On unixoid systems, if ACPLT/RTE has been installed via `make install` without a special prefix, `ov_runtimeserver` should be part of the $PATH and be invokable like any other installed program.
The OV libraries are placed in a system shared library directory, so they are automatically found for dynamic linking.

If, on the other hand, ACPLT/RTE has been packaged in a local directory, the `ov_runtimeserver` will need a proper `LD_LIBRARY_PATH` environment variable, pointing to the `lib/` directory, to locate the OV libraries at runtime.
On Windows, this is not necessary, since the OV libarary DLL files are placed in the `bin/` directory together with the executable program.


### Tools

- TODO usage of ov_dbutil



## Creating and building custom libraries

ACPLT/OV is meant to be extended with custom libraries, providing custom object classes.
The object-oriented interface of these libraries is described using a domain specific language (DSL), the .ovm files.
In these files, object classes with attributes, operations and inheritance are declared as well as relationship types.

From this declaration, C header files and some definitions are generated during compilation.
The corresponding implementation code needs to provided as C source files.
Templates for these source files can also be generated from the OVM model (see below).


### Project types and project dependencies

- TODO building against rte/library project build tree vs. including rte sources as subdirectory
- TODO application project structure vs. library project structure (include vs. reference rte and other library projects)


### Creating a new project

- TODO CMakeLists.txt template (ref to example project)


### Creating a new library

For the provided tools (CMake build scripts, acplt_builder for templates) to work, OV libraries must follow the following file structure:

```txt
<library_name>/
├── include/                     [optional, for custom C header files]
│   └── example_custom_header.h
├── model/
│   ├── <library_name>.ovm
│   ├── <library_name>.ovf       [optional, C header for function type decl.]
│   └── <library_name>.ovt       [optional, C header for data type declarations]
├── source/
│   ├── className1.c
│   ├── className2.c
│   └── example_custom_code.c
└── CMakeLists.txt
```

For simple OV libraries, the `CMakeLists.txt` file only requires two lines:
```cmake
add_ov_library(<library_name>)
ov_library_includes(<library_name> <dependency1> <dependency2>)
```

The `add_ov_library()` function defines a CMake library target for the OV library, which is compiled from all `.c` files in the `source/` directory as well as the sources generated from the OVM file in the `model/` directory.
The `include/` directory is automatically added as include path for compilation (if present).
The `ov_library_includes()` defines OV library dependencies which must be considered for code generation and linking.
It also sets up the OV code generation build step for the library.

Notes:
- `ov_library_includes()` must name all OV library dependencies that are listed as `#include` in the library's OVM file.
- `ov_library_includes()` must always be present. Typically there's at least one dependency, e.g. on one of the core libraries like `ov` or `fb`.
- `ov_library_includes()` must not be used to link against other libraries/targets than OV libraries.

Additional settings for the compilation or linking of the library can be added through the usual CMake commands, e.g.
- adding compiler flags like `target_compile_definitions(<library_name> PRIVATE -DMY_PREPROCESSOR_DEF=1)`
- linking additional libraries like
  ```cmake
  if (WIN32)
      target_link_libraries(<library_name> PRIVATE winmm.lib ws2_32.lib)
  endif()
  ```
  (If compiled libraries are provided with the OV library sources, they should by convention be placed in a `lib/` directory.)
- Adding additional build steps for preperation of additional sources like
  ```cmake
  add_custom_command (
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated_foo.c
      COMMAND tclsh ${CMAKE_CURRENT_SOURCE_DIR}/prebuild.tcl foo.txt ${CMAKE_CURRENT_BINARY_DIR}/generated_foo.c
      DEPENDS foo.txt
      COMMENT "Using tclsh to generate foo for <library_name>"
      )
  target_sources(<library_name> PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated_foo.c)
  ```


### Generating library source templates

- TODO codegen vs. acplt_builder
- TODO acplt_builder usage
- TODO OV_LIBRARY_PATH environment variable


## Debugging

- TODO Valgrind
