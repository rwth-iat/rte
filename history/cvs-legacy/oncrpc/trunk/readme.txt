ONC/RPC for Windows NT
----------------------

Version 1.15.1

Welcome to the ONC/RPC package for Windows NT (and also for
Windows 95/98). This Version is based on Martin Gergeleit's
famous ONC/RPC package (last official version 1.11).

Unfortunately, Martin Gergeleit doesn't actively maintain the package
any longer and has shut down his web pages, so the Chair of Process
Control Engineering (of Aachen University of Technology) now hosts the
ONC/RPC package.

But first a message from our lawers:

 * THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
 * ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

What's new? (in descending order)

- v1.15: Changes in rpc.h, svc_auth and makefile in order to support
  MS Visual C+ 6.0.

- v1.14: Support for automatic installation via InstallShield Express
  2.04 or later. This includes automatic installation of even updates
  of the ONC/RPC NT portmap service! Just insert the installation
  floppy, run the setup program and select the Custom mode, so you can
  install the NT portmap service. That's it -- the portmapper is
  installed or updated automatically.

  The magic is done behind the scenes by a DLL accompanying the
  portmap.exe service executable. The DLL is named portmap.srg and
  contains the necessary logic for self-registration (it misuses the
  OLE server registration mechanism).

  You can also register or deregister the portmapper from the command
  line: just start the portmapper executable with either the command
  line argument "/registerservice" or "/unregisterservice".

  Also added version information to the oncrpc.dll and the portmap.exe
  binaries.

- v1.12: The sources have been adapted to Borland C++ 5.0. The
  binaries included in this packages have been compiled using
  BC++. For the people insisting on using MS Visual C+, we're
  providing the necessary import library as "bin\oncrpcms.lib". The
  software is still compilable using VC+ 4.2. To compile the package
  with Borland C++, use the makefile.bc makefiles or the makebc.bat
  script in the root directory of this package.

- v1.12: Corrected rpc\svc.h, so the fdset used in a RPC server is
  exported properly:
    _declspec(dllimport) fd_set svc_fdset
  Changed rpc\clnt.h accordingly to export a structure necessary to
  check for client errors:
    struct rpc_createerr rpc_createerr;
  Also updated the def files accordingly, like oncrpc.def, so data
  structures are properly exported.

But now enjoy!

Your ACPLT/KS Team.
email:ks@rwth-aachen.de
http://www.plt.rwth-aachen.de/ks
