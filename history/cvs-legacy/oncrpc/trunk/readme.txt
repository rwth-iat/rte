IMPORTANT NOTE:
---------------

This is a patched version of Martin Gergeleits famous ONC/RPC for Windows NT
package. The patches made to this sources are listed at the end of this
README.TXT file.


Use the install.bat in the service subdirectory for easy installation.
Don't forget to start the portmapper using "Control Panel|Services|Portmap".


This patched package
  IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
  WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
  PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
  In no event will Harald Albrecht be liable for any lost revenue
  or profits or other special, indirect and consequential damages, even if
  Harald Albrecht has been advised of the possibility of such damages.


The sources have been adapted to Borland C++ 5.0. Therefore, the binaries
and executables contained in this package have been compiled with BC++ 5.0.


Changes made:
-------------

README.TXT --
  This readme file is new.

MAKEBC.BAT --
  Compiles everything using BC++. Enjoy!

RPC\SVC.H --
  Corrected the line specifying import information when another application
  includes the ONC/RPC header files:
    _declspec(dllimport) fd_set svc_fdset;
  Also added the necessary changes for BC++.

RPC\CLNT.H --
  Added some stuff, such that the following variable is now properly
  exported from the DLL and imported into other applications:
    struct rpc_createerr rpc_createerr;
  Also added the necessary changes for BC++.

LIBRPC\ONCRPC.DEF --
  Added the line "rpc_createerr DATA" at the end of this .DEF file.
  Added the line "_null_auth DATA" at the end of this .DEF file.

LIBRPC\MAKEFILE.BC --
TEST\MAKEFILE.BC --
RPCINFO\MAKEFILE.BC --
RPCGEN\MAKEFILE.BC --
SERVICE\MAKEFILE.BC --
  New Makefiles suitable for Borland C++. You must edit the BC makro to
  addapt them to your local BC setup.

SERVICE\INSTALL --
  Installs the ONC/RPC package into %SystemRoot%\system32 and copies
  the "RPC" file into %SystemRoot%\system32\drivers\etc. You only have
  to start the service using "Control Panel|Services|Portmap".

LIBRPC\ONCBCIMP.DEF --
  Necessary to bypass the strange behaviour of Mikro$haft to export the
  names of C functions in DLLs without the leading underscore! This file
  is used by IMPLIB to create a import library which maps the real function
  names containing the underscore to the names exported by the ONCRPC.DLL.

LIBRPC\ONCRPCBC.DEF --
  Necessary to be compatible with the brain damaged VC++. This exports the
  names of the C functions from the DLL without the leading underscore.

LIBRPC\RPC_PROT.C --
  Changed to be compliant with BC++, which is behaves much better according
  to the ANSI C++ spec:
    #ifdef WIN32
    extern
    #endif
    struct opaque_auth _null_auth;

LIBRPC\RPC_COMM.C --
  Same as above...
    #if definded(WIN32) && defined(__BORLANDC__)
    __declspec(dllexport)
    #endif
    struct opaque_auth _null_auth;
    #ifdef FD_SETSIZE
    #if definded(WIN32) && defined(__BORLANDC__)
    __declspec(dllexport)
    #endif
    fd_set svc_fdset;
    #else
    int svc_fds;
    #endif /* def FD_SETSIZE */
    #if definded(WIN32) && defined(__BORLANDC__)
    __declspec(dllexport)
    #endif
    struct rpc_createerr rpc_createerr;

LIBRPC\GETRPCEN.C --
  Corrected wrong prototype for "interpret()". It was missing the
  "char *val"...
    static struct rpcent *
    interpret(val, len)
        char *val;
        {
        ...

RPCGEN\RPC_MAIN.C --
  Addapted for Borland's CPP Makro Preprocessor.

RPCINFO\GETOPT.C --
  Removed strange "extern strlen(), _write();" which caused compilation
  errors.

Harald Albrecht
harald@plt.rwth-aachen.de
Chair of Process Control Engineering

