ifndef $arch_bit
arch_bit=$(shell getconf LONG_BIT)
endif
all:
	tclsh acplt_build.tcl $(arch_bit)

trunk:
	tclsh acplt_build.tcl $(arch_bit)

clean:
	rm acplt.build -rf
	rm acplt -rf
	rm acplt_build.log

source:
	tclsh acplt_build.tcl source $(arch_bit)

release:
	tclsh acplt_build.tcl release $(arch_bit)

compileonly:
	tclsh acplt_build.tcl compileonly $(arch_bit)
