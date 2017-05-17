# Crosscompilation for ACPLT/RTE
The acplt_build.tcl script is able of basic cross compilation.
To do a cross build there are some prerequisites:
1. A native ACPLT Environment has to be there in order to have acplt_makmak an ov_codegen. See Section ACPLT Environment.
2. A cross-definitions File has to exist. Take a look at windows32.cross, windows64.cross, raspberryPi.cross for an example.
3. You need a cross toolchain like i686-w64-mingw32. CPATH has to be set according to your cross-toolchain (includes path).

When these prerequisites are met you can invoke the build script with:
```sh
tclsh acplt_build.tcl cross cross-definitions-filename
```
## ACPLT Environment
During the build process the binaries for acplt_makmak an ov_codegen are needed.
Both of them need to be found by the host OS, so you need to put them into PATH.
1. Therefore build a release or download it from [Releases](http://acplt.github.io/rte-www/releases/) for your host OS. For example on a linux 64 bit host:
```sh
link=`wget -qO- http://acplt.github.io/rte-www/releases/ | grep -oEm 1 "[0-9]{4}-[0-9]{2}-[0-9]{2}-[0-9]{2}:[0-9]{2}-[0-9a-z]{10}_acpltRTE-linux64.tar.gz" | head -1`
wget -q http://acplt.github.io/rte-www/releases/$link
tar -zxf ./$link acplt/system/sysbin
mv acplt/system/sysbin acplt_env
rm -rf acplt $link
```
2. Export the path in PATH. For example:
```sh
export PATH=$PATH:$(pwd)/acplt_env/
```
3. Start the cross compilation. For example:
```sh
svn co https://github.com/acplt/rte/trunk/build acplt_build
cd acplt_build
tclsh acplt_build.tcl cross windows32.cross
```
