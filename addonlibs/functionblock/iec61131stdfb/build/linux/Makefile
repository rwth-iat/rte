#******************************************************************************#
#* A lightweight Makefile to invoke the prebuild mechanism
#******************************************************************************#

#Lower case makro
lc = $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))

#Project config parameters that are accessed by Eclipse
DATABASE = database 
MAKMAKOPTIONS =
EXTRA_LIBS = 
LD_FLAGS = -lm
COMPILER = GCC
COMPILER_LO = $(call lc,$(COMPILER))
PREBUILD = ../prebuild.tcl
POSTBUILD = ../postbuild.tcl
COMPILER_LO = $(call lc,$(COMPILER))

# Set TARGETOS to --targetWindows --targetLinux or --targetSolaris to cross-compile for the respective system.\n
TARGETOS = 
		
all:

%:
	acplt_makmak -l iec61131stdfb $(MAKMAKOPTIONS) $(TARGETOS)
	-tclsh $(PREBUILD) iec61131stdfb $(MAKMAKOPTIONS)
	$(MAKE) EXTRA_LIBS='$(EXTRA_LIBS)' LD_FLAGS='$(LD_FLAGS)' TARGET='$*' -f $(COMPILER_LO).mk $*
	-tclsh $(POSTBUILD) iec61131stdfb
