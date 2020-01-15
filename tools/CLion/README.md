# CLion Setup

This document describes the setup of CLion for ACPLT/RTE projects.
This includes configuration for toolchains, OVM highlighting and source template generation.

## Setup project workspace

Clone an existing Project or create a new one (see _rte\_example-project_ for reference).
Open the Project in CLion at the root.

## Adding toolchain in CLion

CLion on Windows knows serveral types of toolchaines.
These include _mingw_, _cygwin_, _Visual Studio_, _WSL_ and _remote_.
On Linux CLion only differentiates between _local_ and _remote_

To add a new Toolchain navigate to ``Build, Execution, Devleopment>Toolchains`` in the settings and click the plus sign.

### MINGW-W64 (MSYS2)

* In _Environment_ select ``$MSYS2_ROOT$/mingw64``.

CLion will probably complain about _sh.exe_ beeing in the _PATH_.
This error can be ignored for now.

* In _Debugger_ CLion should have found a MinGW-w64 GDB.

Switch to ``Build, Execution, Devleopment>CMake`` and add a new CMake configuraion.
Select the mingw toolchain under _Toolchain_ and set the _Build type_ to a preferred value.

To fix the _sh.exe_ problem add ``-DCMAKE_SH="CMAKE_SH-NOTFOUND"`` to the _CMake options_.


### CYGWIN
TODO

### WSL

This requires a WSL setup with enabled ssh server and required packages installed.
See rte readme for instructions on setup on Linux.

Set the credentials for the ssh connection and the rest should be detected automatically.
For instructions how to start the ssh server in wsl on startup, search the web.

## OVM Highlighting
In CLion open 'File>Import Settings...' from the menu and navigate to ``$rte-repository$/tools/CLion`` and choose the ``CLion_Settings_OVM_Highlighting.zip``. This loads the highlighting settings for ovm files.

## Template generation
ACPLT/RTE includes a tool called acplt_builder to generate templates (c file stubs) for ov classes from ovm files.
These templates are usually places in the ``source/sourcetemplates`` directory of an ov library.

! This feature requires the ovm highlighting as it defines the OV Model file type.

To enable automatic generation for templates on modification of the model file, install the ``File Watchers`` plugin in CLion.
Configuration files(unix/unix) for this Plugin are provided in the ``$rte-repository$/tools/CLion`` directory.
The appropriate file (_WatcherTasks.xml.*_) needs to be copied into the ``.idea`` directory in the project root and renamed to _.xml_ .

Preferences for the plugin are found in the Settings dialog ``Tools>File Watcher``.
The default settings expect the _acplt\_builder_ executable in the path.
An absolute path may be specified in the settings.
The 

_acplt\_builder_ requires the ovm files of included libraies for template generation.
Search path are defined in the OV\_LIBRARY\_PATH environment variable, a ``:``(unix systems) resp. ``;``(windows) separated list (similar to _PATH_) with libraries present in an equally named subdirectory.

By default the directories rte/core, rte/syslibs, rte\_fblib, rte\_field and rte\_misc in the project root as well as the project root itself are included in the OV\_LIBRARY\_PATH.
Additonal path can be specified in the _File Watcher_ settings dialog.
