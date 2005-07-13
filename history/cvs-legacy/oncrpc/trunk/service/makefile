!include <ntwin32.mak>

# If the rpc include directory is not included in the standard path
# you have to give the path to it here.
RPCINCLUDEPATH = ..

# If the rpc library is not included in the standard lib path
# you have to give the path to it here.
RPCLIBPATH = ..\bin\\

all: portmap.exe inst_pm.exe

# Update the object file if necessary

clean:
    del *.obj *.exe

portmap.exe: service.obj servreg.obj portmap.obj $(RPCLIBPATH)oncrpc.lib portmap.res
    $(link) $(ldebug) $(conflags) -out:portmap.exe portmap.obj service.obj servreg.obj portmap.res $(conlibs) $(RPCLIBPATH)oncrpc.lib wsock32.lib user32.lib
    copy portmap.exe ..\bin

inst_pm.exe: inst_pm.obj
    $(link) $(ldebug) $(conflags) -out:inst_pm.exe inst_pm.obj $(conlibs)
    copy inst_pm.exe ..\bin

.c.obj:
    $(cc) /I$(RPCINCLUDEPATH) $(cdebug) $(cflags) $(cvars) $*.c

.rc.res:
    rc $*.rc
