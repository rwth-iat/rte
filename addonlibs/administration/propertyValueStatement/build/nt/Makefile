#******************************************************************************#
#* A lightweight Makefile to invoke the prebuild mechanism
#******************************************************************************#

#Lower case makro
lc = $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))

#Project config parameters that are accessed by Eclipse
SERVERSTARTKEYS = -w identification -w propertyValueStatement
#/Project config parameters that are accessed by Eclipse
MAKMAKOPTIONS = -o
EXTRA_CC_FLAGS = -DUA_EXTERNAL_NAMESPACES=1 -DLOG_KS=1 -DLOG_KS_ERROR=1 -DLOG_KS_WARNING=1 -DLOG_UA=1 -DLOG_UA_TO_OV=1 -DSTATIC_LINKING=1 -DDEBUG=1 -D_XOPEN_SOURCE=500 -D_GNU_SOURCE -Wextra -Wno-unused-parameter -Wno-declaration-after-statement -Wno-unused-functio
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
	acplt_makmak -l propertyValueStatement $(MAKMAKOPTIONS) $(TARGETOS)
	tclsh $(PREBUILD) propertyValueStatement $(MAKMAKOPTIONS)
	$(MAKE) MAKMAKOPTIONS='$(MAKMAKOPTIONS)' EXTRA_CC_FLAGS='$(EXTRA_CC_FLAGS)' EXTRA_LIBS='$(EXTRA_LIBS)' LD_FLAGS='$(LD_FLAGS)' TARGET='$*' -f $(call lc,$(COMPILER)).mk $*
	tclsh $(POSTBUILD) propertyValueStatement
