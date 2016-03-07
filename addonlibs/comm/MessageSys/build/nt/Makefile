#******************************************************************************#
#* A lightweight Makefile to invoke the prebuild mechanism
#******************************************************************************#

#Lower case makro
lc = $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))

#Project config parameters that are accessed by Eclipse
DATABASE = database 
DATABASECREATEKEYS = 
MAKMAKOPTIONS = -o
EXTRA_CC_FLAGS = -DLOG_KS=0 -DLOG_KS_ERROR=1 -DLOG_KS_WARNING=1 -DLOG_KS_INFO=1
EXTRA_LIBS = 
LD_FLAGS = 
COMPILER = GCC
PREBUILD = ../prebuild.tcl
POSTBUILD = ../postbuild.tcl
COMPILER_LO = $(call lc,$(COMPILER))

# Set TARGETOS to --targetWindows --targetLinux or --targetSolaris to cross-compile for the respective system.\n
TARGETOS = 
		
all:

%:
	acplt_makmak -l MessageSys $(MAKMAKOPTIONS) $(TARGETOS)
	tclsh $(PREBUILD) MessageSys $(MAKMAKOPTIONS)
	$(MAKE) EXTRA_CC_FLAGS='$(EXTRA_CC_FLAGS)' EXTRA_LIBS='$(EXTRA_LIBS)' LD_FLAGS='$(LD_FLAGS)' TARGET='$*' -f $(call lc,$(COMPILER)).mk $*
	tclsh $(POSTBUILD) MessageSys
