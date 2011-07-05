
all %:
	mkdir -p build/linux
	../../bin/ov_makmak -l fb -pu .. -pa ../../../../base -pb ../../../../bin
	patch -p0 < library-open.patch
	make -C build/linux $*
