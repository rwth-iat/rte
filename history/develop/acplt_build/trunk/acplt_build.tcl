
# ACPLT Build Script
# (c) 2011 Chair of Process Control Engineering, RWTH Aachen University
# Author: Gustavo Quiros <g.quiros@plt.rwth-aachen.de>
# Author: Sten Gruener   <s.gruener@plt.rwth-aachen.de>
#
# Usage: tclsh acplt_build.tcl (release) (checkout)
set release 0
set checkout 0 

foreach arg $argv {
	if {$arg == "checkout"} {
		set release 0
		set checkout 1
		break
	}
	if {$arg == "release"} {
		set release 1
		set checkout 0
		break
	}
}

set basedir [pwd]

set builddir $basedir/acplt.build
set releasedir $basedir/acplt

set logfile $basedir/acplt_build.log

#if {![info exists env(CVSROOT)]} then {
#    puts stderr "Please set the environment variable CVSROOT and try again."
#    exit 1
#c}

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

#cygwin stuff
if { $os == "nt" } then {
	set bash "bash -c "
}

file delete -force $logfile


####################### PROCEDURES #######################


# findDirectories
# basedir - the directory to start looking in
# pattern - A pattern, as defined by the glob command, that the files must match

#WARNING DOES NOT WORK FOR WINDOWS BECAUSE OF THE SPACES IN THE DIRNAMES

proc findDirectories {directory pattern} {

    # Fix the directory name, this ensures the directory name is in the
    # native format for the platform and contains a final directory seperator
    set directory [string trimright [file join [file normalize $directory] { }]]

    # Starting with the passed in directory, do a breadth first search for
    # subdirectories. Avoid cycles by normalizing all file paths and checking
    # for duplicates at each level.

    set directories [list]
    set parents $directory
    while {[llength $parents] > 0} {

        # Find all the children at the current level
        set children [list]
        foreach parent $parents {
            set children [concat $children [glob -nocomplain -type {d r} -path $parent *]]
        }

        # Normalize the children
        set length [llength $children]
        for {set i 0} {$i < $length} {incr i} {
            lset children $i [string trimright [file join [file normalize [lindex $children $i]] { }]]
        }

        # Make the list of children unique
        set children [lsort -unique $children]

        # Find the children that are not duplicates, use them for the next level
        set parents [list]
        foreach child $children {
            if {[lsearch -sorted $directories $child] == -1} {
                lappend parents $child
            }
        }

        # Append the next level directories to the complete list
        set directories [lsort -unique [concat $directories $parents]]
    }

    # Get all the files in the passed in directory and all its subdirectories
    set result [list]
    foreach directory $directories {
        set result [concat $result [glob -nocomplain -type {r d} -path $directory -- $pattern]]
    }

    # Normalize the filenames
    set length [llength $result]
    for {set i 0} {$i < $length} {incr i} {
        lset result $i [file normalize [lindex $result $i]]
    }

    # Return only unique filenames
    return [lsort -unique $result]
}

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

# Execute a command
proc execute_ignore {args} {
    global logfile
    set cmd [concat {exec } $args { >>& $logfile}]
    #puts $cmd
    if { [catch $cmd msg] } {
    }
}

proc copy_wildcard {src target} {
    set files [concat [glob -nocomplain $src]]
    foreach file $files {
        file copy -force $file $target
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
proc checkout {prefix module {dirname ""} {notrunk ""}} {
    print_msg "Checking out $module"
    if {$dirname == ""} then { set dirname $module }
    #execute cvs checkout -P -d $dirname $module
    if {$notrunk == ""} then {
	    execute svn co https://dev.plt.rwth-aachen.de/acplt-repo/$prefix/$module/trunk $dirname	
    } else {
	    execute svn co https://dev.plt.rwth-aachen.de/acplt-repo/$prefix/$module $dirname	
    }
}

proc checkout_lib {x} {
	if { [string equal $x ksbase] } {
			checkout develop/ks/trunk/ks_revised $x $x notrunk
	} elseif { [string equal -length 2 $x ks] || [string equal $x fbcomlib] } {
	    	checkout develop/ks/trunk $x $x notrunk
	} else {
	    	checkout develop $x
	}
}

# Checkout sources
proc checkout_acplt {} {
    global builddir
    global basedir
    global os
    global included_libs
    cd $builddir
    checkout archive libml
    #for source release - checkout all
    #if { $os == "nt" } then { 
    checkout archive oncrpc
    #}
    checkout archive acplt base
    cd $builddir/base
    #checkout libmpm
    checkout develop ov
    #checkout develop/DevTools acplt_makmak
    cd $builddir/user
    foreach x $included_libs {
	checkout_lib $x
    }

    cd $basedir
 }

# Build in a directory
proc build {package args} {
    print_msg "Building $package"
    eval [concat "execute" $args]
}

# Build in a directory using cygwin bash and ignoring errors
proc build_cygwin {package args} {
	global bash
    print_msg "Building $package"
    eval [concat "execute" $bash \"$args\"]
}

proc build_acplt_mingw {} {
	print_msg "Compiling with mingw32"
    global base
    global os
    global make
    global basedir
	global builddir
	print_msg "Building oncrpc"
	cd $builddir/oncrpc/
	execute makemingw.bat
	#file copy -force $builddir/oncrpc/bin/oncrpc.a $builddir/oncrpc/bin/oncrpcms.a
	cd $builddir/libml
	build_cygwin libml make -f mingw.mk
	#cd $builddir/base/libmpm 
	#build_cygwin libmpm make -f Makefile
	#cd $builddir/base/plt/build/cygwin
    #build_cygwin plt make -f makefile
	#cd $builddir/base/ks/build/cygwin
    #build_cygwin ks make -f makefile
	cd $builddir/base/ov/build/cygwin
    build_cygwin ov make -f makefile
	#cd $builddir/base/acplt_makmak/build/cygwin
   #build_cygwin acplt_makmak make -f makefile
}

# Build ACPLT (msvc in windows [depricated] and gcc in linux)
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
        #file copy -force $builddir/oncrpc/bin/oncrpc.lib $builddir/oncrpc/bin/oncrpcms.lib
        cd $basedir
    }
    # build libmpm make -C $builddir/base/libmpm -f $makefile
    if { $os == "nt" } then {
        #cd $builddir/base/plt/build/ntvc
        #build plt nmake /f $makefile
        #cd $builddir/base/ks/build/ntvc
        #build ks nmake /f $makefile
        cd $builddir/base/ov/build/ntvc
        build ov make -f $makefile -k
        cd $basedir
    } else {
        #build plt make -C $builddir/base/plt/build/$os
        #build ks make -C $builddir/base/ks/build/$os
        build ov make -C $builddir/base/ov/build/$os
   }
   #if { $os == "nt" } then {
   #	build acplt_makmak $make -C $builddir/base/acplt_makmak/build/ntvc
   #} else {
   #	build acplt_makmak $make -C $builddir/base/acplt_makmak/build/linux
   #}
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
        set libfiles [concat [glob -nocomplain $dir/*.lib $dir/*.a]]
    }
    foreach file $binfiles {
	file copy -force $file $builddir/bin
    }
    foreach file $libfiles {
	file copy -force $file $builddir/lib
    }
    # execute make -C $dir install
} 

proc install_acplt { target } {
    global builddir
    install $builddir/oncrpc/bin
    install $builddir/base/plt/build/$target
    install $builddir/base/ks/build/$target
    install $builddir/base/ov/build/$target
    #install $builddir/base/acplt_makmak/build/$target
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

proc release_lib {libname} {
	relase_lib libname "all"
}

proc release_lib {libname option} {
    global releasedir
    global os
    global make
    cd $releasedir/user/
    file delete -force $releasedir/user/$libname/
    checkout_lib $libname
    cd $releasedir/user/$libname/build/$os/
    if { $option == "all" } then {
	    print_msg "Note: no debug symbols will be created"
    }
    build $libname $make $option
    print_msg "Deploying $libname"
    file delete -force $releasedir/user/$libname.build/
    file copy -force $releasedir/user/$libname/ $releasedir/user/$libname.build/
    file delete -force $releasedir/user/$libname/
    file mkdir $releasedir/user/$libname/
    file mkdir $releasedir/user/$libname/model/
    copy_wildcard $releasedir/user/$libname.build/model/*.ov? $releasedir/user/$libname/model/
    file mkdir $releasedir/user/$libname/include/
    copy_wildcard $releasedir/user/$libname.build/include/*.h $releasedir/user/$libname/include/
    #export libname.a file for compiling under windows
    if { $os == "nt" } then {
		#if { [file exists $releasedir/user/$libname.build/build/nt/$libname.a] } {
		#		file copy -force $releasedir/user/$libname.build/build/nt/$libname.a $releasedir/user/$libname/build/nt/
		#}
		if { [file exists $releasedir/user/$libname.build/build/nt/$libname.lib] } {
		    file mkdir $releasedir/user/$libname/build/nt/
			file copy -force $releasedir/user/$libname.build/build/nt/$libname.lib $releasedir/user/$libname/build/nt/
		}
    }
    if { $os == "linux" } then {
		#if { [file exists $releasedir/user/$libname.build/build/nt/$libname.a] } {
		#		file copy -force $releasedir/user/$libname.build/build/nt/$libname.a $releasedir/user/$libname/build/nt/
		#}
		if { [file exists $releasedir/user/$libname.build/build/linux/$libname.a] } {
		    file mkdir $releasedir/user/$libname/build/linux/
			file copy -force $releasedir/user/$libname.build/build/linux/$libname.a $releasedir/user/$libname/build/linux/
		}
    }
    file delete -force $releasedir/user/$libname.build/
}

proc create_release {} {
	global os
	global releasedir
	global builddir
	global env

    #create a release
	set env(ACPLT_HOME) $releasedir
	if { $os == "nt" } then {
		set env(PATH) $releasedir/bin/\;$env(PATH)
	} else {
		set env(PATH) $releasedir/bin/:$env(PATH)
	}

	print_msg "Creating release in $releasedir"
	file mkdir $releasedir
	#bin dir
	file mkdir $releasedir/bin
	set libfiles [concat [glob -nocomplain $builddir/bin/*]]
	foreach file $libfiles {
		file copy -force $file $releasedir/bin/
	}
	#lib dir
	file mkdir $releasedir/lib
	#if { $os == "nt" } then {
	#	set lib ".lib"
	#}
	set lib ".a"

	set libfiles [list "$builddir/lib/libov$lib"]
	foreach file $libfiles {
		file copy -force $file $releasedir/lib/
	}
	if { $os == "nt" } then {
		file copy -force "$builddir/lib/oncrpc$lib" $releasedir/lib/
	#	file copy -force "$builddir/lib/oncrpcms$lib" $releasedir/lib/
	}
	#model dir
	file mkdir $releasedir/model
	set libfiles [concat [glob -nocomplain $builddir/base/ov/model/ov.*]]
	foreach file $libfiles {
		file copy -force $file $releasedir/model
	}
	#database dir
	file mkdir $releasedir/database
	#include dir
	file mkdir $releasedir/include
	file mkdir $releasedir/include/ks
	copy_wildcard $builddir/base/ks/include/ks/*.h $releasedir/include/ks
	file mkdir $releasedir/include/libov
	copy_wildcard $builddir/base/ov/include/libov/*.h $releasedir/include/libov
	file mkdir $releasedir/include/libovks
	copy_wildcard $builddir/base/ov/include/libovks/*.h $releasedir/include/libovks
	file mkdir $releasedir/include/plt
	copy_wildcard $builddir/base/plt/include/plt/*.h $releasedir/include/plt
	if { $os == "nt" } then {
		file mkdir $releasedir/include/rpc
		copy_wildcard $builddir/oncrpc/rpc/*.h $releasedir/include/rpc
	}
	#user dir
	file mkdir $releasedir/user
	file mkdir $releasedir/user/libs
	#download tclsh
	if { $os == "nt" } then {
		print_msg "Downloading tclsh.exe"
		cd $releasedir/bin
		execute "wget" "http://tclkit.googlecode.com/files/tclkitsh-8.5.8-win32.upx.exe"
		file copy -force tclkitsh-8.5.8-win32.upx.exe tclsh.exe
		file delete -force tclkitsh-8.5.8-win32.upx.exe
	}
}

# ============== MAIN STARTS HERE ==================
set included_libs {ksserv fb iec61131stdfb ksservtcp ksservhttp ksapi ksapitcp fbcomlib}

if {$release != 1} {
	puts "Running this script with 'release' option will create releases"
}
if {$checkout != 1} {
	puts "Running this script with 'checkout' option will checkout only"
}

if {$checkout == 1} {
	puts "== CHECKOUT ONLY =="
}

if {$release == 1} {
	# Create source release
    print_msg "== CREATING SOURCE RELEASE =="
	
	#backup old checkout (otherwise binaries will be in the source-release)
	if { [file exists acplt.build_backup] } {
		file delete -force acplt.build_backup
	}
	if { [file exists acplt.build] } {
		file rename -force acplt.build acplt.build_backup
	}
	if { [file exists acplt-source] } {
		file delete -force acplt-source
	}
	
	#cheking out a fresh copy
	create_dirs
	checkout_acplt

	#rename 
	file rename -force acplt.build acplt-source
	print_msg "Deleting .svn directories"
	#remove .svn directories
	if { $os == "nt"} {
		#God, I hate spaces in windows dirnames
		file copy delete_svn_folders.bat acplt-source
		cd "acplt-source"
		execute "delete_svn_folders.bat"
		file delete -force "delete_svn_folders.bat"
		cd ".."
	} else {
	    cd "acplt-source"
		set dirs [findDirectories "." ".svn"]
		foreach dir $dirs {
			file delete -force $dir
		}
		cd ".."
	}

    set date [clock format [clock seconds] -format "%Y%m%d"]
    set name "acplt-server-source-$date"
    cd $basedir
	print_msg "Compressing"
    if { $os == "linux" } then {
    	execute "zip" "-r" $name "./acplt-source"
    } else {
	execute "7z" "a" "$name.zip" "./acplt-source"
    }
    file delete -force "acplt-source"
	
	#rename back to save compiling overhead
	file delete -force "acplt.build"
	if { [file exists acplt.build_backup] } {
		file rename acplt.build_backup acplt.build
	}
	print_msg "== SOURCE RELEASE CREATED =="
# end
}


create_dirs
checkout_acplt
if {$checkout == 1} {
	exit 0
}

if { $os == "nt" } then {
	#for depricated msvc use following:
	#build_acplt
	#install_acplt {ntvc}
	#note that ov_runtimeserver has never been compiled with msvc
	build_acplt_mingw
	install_acplt cygwin
} else {
	build_acplt
	install_acplt linux
}

if {$release == 1} {
# Create develop release
    print_msg "== CREATING DEVELOP RELEASE =="
}
    create_release
    foreach x $included_libs {
	release_lib $x debug
    }
if {$release == 1} {
    set date [clock format [clock seconds] -format "%Y%m%d"]
    set name "acplt-server-develop-$date"
    cd $basedir
	print_msg "Compressing"
    if { $os == "linux" } then {
    	execute "zip" "-r" "$name-linux" "./acplt"
    } else {
	execute "7z" "a" "$name-win.zip" "./acplt"
    }
	print_msg "== DEVELOP RELEASE CREATED =="
# end
}

if {$release == 1} {
# Create runtime release
    print_msg "== CREATING RUNTIME RELEASE =="
    create_release
    foreach x $included_libs {
        release_lib $x all
    }
    set date [clock format [clock seconds] -format "%Y%m%d"]
    set name "acplt-server-runtime-$date"
    cd $releasedir/user
    foreach x $included_libs {
        file delete -force $x
    }
    cd $releasedir/bin
    file delete -force acplt_makmak
    file delete -force acplt_makmak.exe
    file delete -force acplt_builder
    file delete -force acplt_builder.exe
    file delete -force ov_builder
    file delete -force ov_builder.exe
    file delete -force ov_codegen
    file delete -force ov_codegen.exe
    file delete -force ov_makmak
    file delete -force ov_makmak.exe
    cd $releasedir
    file delete -force include
    file delete -force model
    file delete -force lib
    cd $basedir
	print_msg "Compressing"
    if { $os == "linux" } then {
    	execute "zip" "-r" "$name-linux" "./acplt"
    } else {
	execute "7z" "a" "$name-win.zip" "./acplt"
    }
	print_msg "== RUNTIME RELEASE CREATED =="
# end
}

#start_server
if { $os == "nt" } then {
    puts "Build finished, set the environment variable ACPLT_HOME to '[string map {/ \\} $basedir
]\\acplt'."
    puts "Also, include the directories '%ACPLT_HOME%\\bin' and '%ACPLT_HOME%\\user\\libs' to your %PATH%"
} else {
    puts "Build finished, set the environment variable ACPLT_HOME to '$basedir/acplt'."
    puts "Also, include the directory '\$ACPLT_HOME/bin' in PATH, and '\$ACPLT_HOME/bin:\$ACPLT_HOME/user/libs' in LD_LIBRARY_PATH."
}
