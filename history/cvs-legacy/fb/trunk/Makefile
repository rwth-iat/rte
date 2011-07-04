
all:
	mkdir -p build/linux
	../../bin/ov_makmak -l fb -pu .. -pa ../../../../base -pb ../../../../bin
	patch -p0 < generic.mk.patch
	make -C build/linux
