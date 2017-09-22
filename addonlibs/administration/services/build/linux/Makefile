#******************************************************************************#
#* A lightweight Makefile to invoke the prebuild mechanism
#******************************************************************************#

#Lower case makro
lc = $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))

#Project config parameters that are accessed by Eclipse
SERVERSTARTKEYS = -w services
#/Project config parameters that are accessed by Eclipse
MAKMAKOPTIONS = 
EXTRA_CC_FLAGS = 
EXTRA_LIBS = 
LD_FLAGS = 
COMPILER = GCC
PREBUILD = ../prebuild.tcl
POSTBUILD = ../postbuild.tcl
COMPILER_LO = $(call lc,$(COMPILER))

# Set TARGETOS to --targetWindows --targetLinux or --targetSolaris to cross-compile for the respective system.
TARGETOS = 

all:

%:
	acplt_makmak -l services $(MAKMAKOPTIONS) $(TARGETOS)
	tclsh $(PREBUILD) services $(MAKMAKOPTIONS)
	$(MAKE) MAKMAKOPTIONS='$(MAKMAKOPTIONS)' EXTRA_CC_FLAGS='$(EXTRA_CC_FLAGS)' EXTRA_LIBS='$(EXTRA_LIBS)' LD_FLAGS='$(LD_FLAGS)' TARGET='$*' -f $(call lc,$(COMPILER)).mk $*
	tclsh $(POSTBUILD) services
