# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

# If you want to use the new multithreaded server feature uncomment this line
#MT = -DMULTITHREAD

# If the rpc include directory is not included in the standard path
# you have to give the path to it here.
RPCINCLUDEPATH = ..

# If the rpc library is not included in the standard lib path
# you have to give the path to it here.
RPCLIBPATH = ..\bin\\

OBJS =		COUNT.OBJ \
		COU_SVC.OBJ \
		COU_XDR.OBJ

RPCGEN = 	..\bin\rpcgen.exe

all: COU_SVC.EXE DO_COU.EXE

clean:
	del *.OBJ *.EXE COU.H COU_CLNT.* COU_SVC.* COU_XDR.*

COU_SVC.EXE: 	COUNT.OBJ \
		COU_SVC.OBJ \
		COU_XDR.OBJ \
		$(RPCLIBPATH)oncrpc.lib
     $(link) $(conlflags) $(ldebug) -out:COU_svc.exe COUNT.obj COU_XDR.obj COU_SVC.obj $(conlibsdll) $(RPCLIBPATH)oncrpc.lib
     copy COU_svc.exe ..\bin

DO_COU.EXE: 	COU_CLNT.OBJ \
		COU_XDR.OBJ \
		DO_COU.OBJ \
		$(RPCLIBPATH)oncrpc.lib
     $(link) $(conlflags) $(ldebug) -out:DO_COU.exe COU_XDR.obj COU_CLNT.obj DO_COU.OBJ $(conlibsdll) $(RPCLIBPATH)oncrpc.lib
     copy DO_COU.exe ..\bin

.c.obj:
    $(cc) /I$(RPCINCLUDEPATH) $(MT) $(cdebug) $(cflags) $(cvarsdll) $*.c

COUNT.obj: COUNT.c COU.H
COU_SVC.OBJ: COU_SVC.c
COU_CLNT.OBJ: COU_CLNT.c
DO_COU.OBJ: DO_COU.c COU.H
COU_XDR.OBJ: COU_XDR.c

COU_XDR.C:	COU.X
    $(RPCGEN) COU.X

COU.H:	COU.X
    $(RPCGEN) COU.X

COU_CLNT.C:	COU.X
    $(RPCGEN) COU.X

COU_SVC.C:	COU.X
    $(RPCGEN) COU.X
