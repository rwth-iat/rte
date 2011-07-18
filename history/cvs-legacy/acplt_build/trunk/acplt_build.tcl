
# ACPLT Build Script
# (c) 2011 Chair of Process Control Engineering, RWTH Aachen University
# Author: Gustavo Quiros <g.quiros@plt.rwth-aachen.de>
# Author: Sten Gruener   <s.gruener@plt.rwth-aachen.de>

set basedir [pwd]

set builddir $basedir/acplt.build
set releasedir $basedir/acplt

set logfile $basedir/acplt_build.log

if {![info exists env(CVSROOT)]} then {
    puts stderr "Please set the environment variable CVSROOT and try again."
    exit 1
}

# Determine operating system
if {$tcl_platform(os) == "Linux"} then { 
    set os "linux" 
    set make "make"
} elseif {[lsearch $tcl_platform(os) "Windows"] >= 0} then {
    set os "nt"
    set make "mingw32-make"
    set env(CYGWIN) "nodosfilewarning" 
} else {
    puts stderr "error: unsupported operating system: $tcl_platform(os)"
    exit 1
}

file delete -force $logfile

proc print_msg {msg} {
    puts stderr "\[$msg\]"
}

# Execute a command
proc execute {args} {
    global logfile
    #set cmd [concat {exec -ignorestderr} $args]
    set cmd [concat {exec } $args { >>& $logfile}]
    #puts $cmd
    if { [catch $cmd msg] } {
        puts stderr "error: $msg"
        puts stderr "Consult the file '$logfile'"
        exit 1
    }
}

# Create directory structure
proc create_dirs {} {
    global builddir
    global os
    print_msg "Creating directory structure"
    file mkdir $builddir $builddir/bin $builddir/user $builddir/user/libs $builddir/database $builddir/lib
}

# Checkout a CVS module
proc checkout {module {dirname ""}} {
    print_msg "Checking out $module"
    if {$dirname == ""} then { set dirname $module }
    execute cvs checkout -P -d $dirname $module
}

# Checkout sources
proc checkout_acplt {} {
    global builddir
    global basedir
    global os
    cd $builddir
    checkout libml
    if { $os == "nt" } then { checkout oncrpc }
    checkout acplt base
    cd $builddir/base
    checkout libmpm
    checkout ov
    checkout acplt_makmak
    #cd $builddir/user
    #checkout fb
    #checkout iec61131stdfb
    cd $basedir
}

# Build in a directory
proc build {package args} {
    print_msg "Building $package"
    eval [concat "execute" $args]
}

# Build ACPLT
proc build_acplt {} {
    global builddir
    global base
    global os
    global make
    global basedir
    if { $os == "nt" } then { set makefile "msvc.mk" } else { set makefile "Makefile" }
    build libml make -C $builddir/libml -f $makefile
    if { $os == "nt" } then { 
        cd $builddir/oncrpc
        execute make.bat
        file copy -force $builddir/oncrpc/bin/oncrpc.lib $builddir/oncrpc/bin/oncrpcms.lib
        cd $basedir
    }
    build libmpm make -C $builddir/base/libmpm -f $makefile
    if { $os == "nt" } then {
        cd $builddir/base/plt/build/ntvc
        build plt nmake /f $makefile
        cd $builddir/base/ks/build/ntvc
        build ks nmake /f $makefile
        cd $builddir/base/ov/build/ntvc
        build ov make -f $makefile -k
        cd $basedir
    } else {
        build plt make -C $builddir/base/plt/build/$os
        build ks make -C $builddir/base/ks/build/$os
        build ov make -C $builddir/base/ov/build/$os
   }
   build acplt_makmak $make -C $builddir/base/acplt_makmak
}

proc install {dir} {
    global builddir
    global os
    print_msg "Installing from $dir"
    if { $os == "linux" } then {
        set binfiles [concat [glob -nocomplain $dir/*.so] [glob -nocomplain -type {f x} $dir/*]]
	set libfiles [concat [glob -nocomplain $dir/*.a]]
    } 
    if { $os == "nt" } then {
        set binfiles [concat [glob -nocomplain $dir/*.dll $dir/*.exe]]
        set libfiles [concat [glob -nocomplain $dir/*.lib]]
    }
    foreach file $binfiles {
	file copy -force $file $builddir/bin
    }
    foreach file $libfiles {
	file copy -force $file $builddir/lib
    }
    # execute make -C $dir install
} 

proc install_acplt {} {
    global builddir
    global os
    if { $os == "nt" } then { set target "ntvc" } else { set target $os }
    install $builddir/oncrpc/bin
    install $builddir/base/plt/build/$target
    install $builddir/base/ks/build/$target
    install $builddir/base/ov/build/$target
    install $builddir/base/acplt_makmak
}

proc makmak {library opts} {
    global builddir
    global basedir
    global os
    set makmak $builddir/bin/ov_makmak
    cd $builddir/user/$library/build/$os
    eval [concat "execute \"$makmak\" -l $library -pu \"../../..\" -pa \"../../../../base\" -pb \"../../../../bin\"" $opts]
    cd $basedir
}

proc build_lib {libname deps patch baselibs} {
    global builddir
    global os
    file mkdir $builddir/user/$libname/build/$os
    set depopts ""
    foreach dep $deps { append depopts "-d $dep" }
    makmak $libname $depopts
    if $patch then {
	# Temporary solution for ov_library_open problem
	execute patch $builddir/user/fb/build/generic.mk library-open.patch
    }
    if { $os == "nt" } then { 
        set makefile "msvc.mk"
	set lib $libname.dll
    } else {
        set makefile "Makefile"
	set lib $libname.so
    }
    if { $baselibs != "" } then { set baselibs "BASELIBS=$baselibs" } 
    eval [concat "build $libname make -C \"$builddir/user/$libname/build/$os\" -f $makefile" $baselibs]
    file copy -force $builddir/user/$libname/build/$os/$lib $builddir/user/libs
}

proc dbutil {args} {
    global releasedir
    global os
    set dbutil $releasedir/bin/ov_dbutil
    eval "execute $dbutil $args"
}

proc start_server {} {
    global releasedir
    global os
    set database $releasedir/database/database.ovd
    if [file exists $database] then {
    } else {
        dbutil -f $database -c [expr 1024 * 1024]
    }
    execute $releasedir/bin/tmanager.exe &
    if { $os == "linux" } then {
        global env
        set env(LD_LIBRARY_PATH) $releasedir/bin
    }
    execute $releasedir/bin/ov_server -f $database -s fb_server -w fb -w iec61131stdfb 
}

proc release_lib {libname parallel} {
    global releasedir
    global os
    global make
    cd $releasedir/user/
    checkout $libname
    execute acplt_makmak -l $libname
    cd $releasedir/user/$libname/build/$os/
    build $libname $make all $parallel
    print_msg "Deploying $libname"
    file delete -force $releasedir/user/$libname.build/
    file copy -force $releasedir/user/$libname/ $releasedir/user/$libname.build/
    file delete -force $releasedir/user/$libname/
    file mkdir $releasedir/user/$libname/
    file mkdir $releasedir/user/$libname/model/
    set models [concat [glob -nocomplain $releasedir/user/$libname.build/model/*.ov?]]
    foreach file $models {
        file copy -force $file $releasedir/user/$libname/model/
    }
    #export libname.a file for compiling under windows
    if { $os == "nt" } then {
        file mkdir $releasedir/user/$libname/build/nt/
        file copy -force $releasedir/user/$libname.build/build/nt/$libname.a $releasedir/user/fb/build/$libname/
    }
    file delete -force $releasedir/user/$libname.build/
}

create_dirs
checkout_acplt
build_acplt
install_acplt
#if { $os == "nt" } then { 
#    build_lib fb "" 1 "../../../../lib/oncrpcms.lib"
#} else {
#    build_lib fb "" 1 ""
#}
#build_lib iec61131stdfb fb 0 ""

#create a release
set env(ACPLT_HOME) $releasedir
if { $os == "nt" } then {
    set env(PATH) $env(PATH)\;$releasedir/bin/
} else {
    set env(PATH) $env(PATH):$releasedir/bin/
}

print_msg "Creating release in $releasedir"
file delete -force $releasedir
file mkdir $releasedir
#bin dir
file copy $builddir/bin $releasedir/bin
#lib dir
file copy $builddir/lib $releasedir/lib
if { $os == "nt" } then {
    set libfiles [concat [glob -nocomplain $releasedir/lib/*.lib]]
    foreach file $libfiles {
	set rootname [file rootname $file]
	file copy -force $file $rootname.a
    }
}
#model dir
file mkdir $releasedir/model
set libfiles [concat [glob -nocomplain $builddir/base/ov/model/ov.*]]
foreach file $libfiles {
    file copy -force $file $releasedir/model
}
#database dir
file mkdir $releasedir/database
#database include
file mkdir $releasedir/include
file mkdir $releasedir/include/ks
set headers [concat [glob -nocomplain $builddir/base/ks/include/ks/*.h]]
foreach file $headers {
    file copy -force $file $releasedir/include/ks
}
file mkdir $releasedir/include/libov
set headers [concat [glob -nocomplain $builddir/base/ov/include/libov/*.h]]
foreach file $headers {
    file copy -force $file $releasedir/include/libov
}
file mkdir $releasedir/include/libovks
set headers [concat [glob -nocomplain $builddir/base/ov/include/libovks/*.h]]
foreach file $headers {
    file copy -force $file $releasedir/include/libovks
}
file mkdir $releasedir/include/plt
set headers [concat [glob -nocomplain $builddir/base/plt/include/plt/*.h]]
foreach file $headers {
    file copy -force $file $releasedir/include/plt
}
if { $os == "nt" } then {
    file mkdir $releasedir/include/rpc
    set headers [concat [glob -nocomplain $builddir/oncrpc/rpc/*.h]]
    foreach file $headers {
        file copy -force $file $releasedir/include/rpc
    }
}
#user dir
file mkdir $releasedir/user
file mkdir $releasedir/user/libs
#-fb
release_lib fb ""
#-iec61131stdfb
release_lib iec61131stdfb ""


#start_server
if { $os == "nt" } then {
    puts "Build finished, set the environment variable ACPLT_HOME to '[string map {/ \\} $basedir
]\\acplt'."
    puts "Also, include the directories '%ACPLT_HOME%\\bin' and '%ACPLT_HOME%\\user\\libs' in your path."
} else {
    puts "Build finished, set the environment variable ACPLT_HOME to '$basedir/acplt'."
    puts "Also, include the directory '\$ACPLT_HOME/bin' in PATH, and '\$ACPLT_HOME/bin:\$ACPLT_HOME/user/libs' in LD_LIBRARY_PATH."
}
