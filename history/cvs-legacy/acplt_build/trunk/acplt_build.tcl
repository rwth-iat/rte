
# ACPLT Build Script
# (c) 2011 Chair of Process Control Engineering, RWTH Aachen University
# Author: Gustavo Quiros <g.quiros@plt.rwth-aachen.de>

set basedir [pwd]

set env(ACPLT_HOME) $basedir/acplt

set logfile acplt_build.log

if {![info exists env(CVSROOT)]} then {
    puts stderr "Please set the environment variable CVSROOT and try again."
    exit 1
}

# Determine operating system
if {$tcl_platform(os) == "Linux"} then { 
    set os "linux" 
} elseif {$tcl_platform(os) == "Windows NT"} then { 
    set os "nt"
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
    global basedir
    global os
    print_msg "Creating directory structure"
    file mkdir $basedir/acplt $basedir/acplt/bin $basedir/acplt/user $basedir/acplt/user/libs $basedir/acplt/database $basedir/acplt/lib
}

# Checkout a CVS module
proc checkout {module {dirname ""}} {
    print_msg "Checking out $module"
    if {$dirname == ""} then { set dirname $module }
    execute cvs checkout -P -d $dirname $module
}

# Checkout sources
proc checkout_acplt {} {
    global basedir
    global os
    cd $basedir/acplt
    checkout libml
    if { $os == "nt" } then { checkout oncrpc }
    checkout acplt base
    cd $basedir/acplt/base
    checkout libmpm
    checkout ov
    cd $basedir/acplt/user
    checkout fb
    checkout iec61131stdfb
    cd $basedir
}

# Build in a directory
proc build {package args} {
    print_msg "Building $package"
    eval [concat "execute" $args]
}

# Build ACPLT
proc build_acplt {} {
    global basedir
    global os
    if { $os == "nt" } then { set makefile "msvc.mk" } else { set makefile "Makefile" }
    build libml make -C $basedir/acplt/libml -f $makefile
    if { $os == "nt" } then { 
        cd $basedir/acplt/oncrpc
        execute make.bat
        file copy -force $basedir/acplt/oncrpc/bin/oncrpc.lib $basedir/acplt/oncrpc/bin/oncrpcms.lib
        cd $basedir
    }
    build libmpm make -C $basedir/acplt/base/libmpm -f $makefile
    if { $os == "nt" } then {
        cd $basedir/acplt/base/plt/build/ntvc
        build plt nmake /f $makefile
        cd $basedir/acplt/base/ks/build/ntvc
        build ks nmake /f $makefile
        cd $basedir/acplt/base/ov/build/ntvc
        build ov make -f $makefile -k
        cd $basedir
    } else {
        build plt make -C $basedir/acplt/base/plt/build/$os
        build ks make -C $basedir/acplt/base/ks/build/$os
        build ov make -C $basedir/acplt/base/ov/build/$os
   }
}

proc install {dir} {
    global basedir
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
	file copy -force $file $basedir/acplt/bin
    }
    foreach file $libfiles {
	file copy -force $file $basedir/acplt/lib
    }
    # execute make -C $dir install
} 

proc install_acplt {} {
    global basedir
    global os
    if { $os == "nt" } then { set target "ntvc" } else { set target $os }
    install $basedir/acplt/oncrpc/bin
    install $basedir/acplt/base/plt/build/$target
    install $basedir/acplt/base/ks/build/$target
    install $basedir/acplt/base/ov/build/$target
}

proc makmak {library opts} {
    global basedir
    global os
    set makmak $basedir/acplt/bin/ov_makmak
    cd $basedir/acplt/user/$library/build/$os
    eval [concat "execute \"$makmak\" -l $library -pu \"../../..\" -pa \"../../../../base\" -pb \"../../../../bin\"" $opts]
    cd $basedir
}

proc build_lib {libname deps patch baselibs} {
    global basedir
    global os
    file mkdir $basedir/acplt/user/$libname/build/$os
    set depopts ""
    foreach dep $deps { append depopts "-d $dep" }
    makmak $libname $depopts
    if $patch then {
	# Temporary solution for ov_library_open problem
	execute patch $basedir/acplt/user/fb/build/generic.mk library-open.patch
    }
    if { $os == "nt" } then { 
        set makefile "msvc.mk"
	set lib $libname.dll
    } else {
        set makefile "Makefile"
	set lib $libname.so
    }
    if { $baselibs != "" } then { set baselibs "BASELIBS=$baselibs" } 
    eval [concat "build $libname make -C \"$basedir/acplt/user/$libname/build/$os\" -f $makefile" $baselibs]
    file copy -force $basedir/acplt/user/$libname/build/$os/$lib $basedir/acplt/user/libs
}

proc dbutil {args} {
    global basedir
    global os
    set dbutil $basedir/acplt/bin/ov_dbutil
    eval "execute $dbutil $args"
}

proc start_server {} {
    global basedir
    global os
    set database $basedir/acplt/database/database.ovd
    if [file exists $database] then {
    } else {
        dbutil -f $database -c [expr 1024 * 1024]
    }
    execute $basedir/acplt/bin/tmanager.exe &
    if { $os == "linux" } then {
        global env
        set env(LD_LIBRARY_PATH) $basedir/acplt/bin
    }
    execute $basedir/acplt/bin/ov_server -f $database -s fb_server -w fb -w iec61131stdfb 
}

create_dirs
checkout_acplt
build_acplt
install_acplt
if { $os == "nt" } then { 
    build_lib fb "" 1 "../../../../lib/oncrpcms.lib"
} else {
    build_lib fb "" 1 ""
}
build_lib iec61131stdfb fb 0 ""
#start_server

if { $os == "nt" } then {
    puts "Build finished, set the environment variable ACPLT_HOME to '$basedir\\acplt'."
    puts "Also, include the directories '%ACPLT_HOME%\\bin' and '%ACPLT_HOME%\\user\\libs' in your path."
} else {
    puts "Build finished, set the environment variable ACPLT_HOME to '$basedir/acplt'."
    puts "Also, include the directory '\$ACPLT_HOME/bin' in PATH, and '\$ACPLT_HOME/bin:\$ACPLT_HOME/user/libs' in LD_LIBRARY_PATH."
}
