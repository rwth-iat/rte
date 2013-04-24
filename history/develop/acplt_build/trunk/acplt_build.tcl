
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
proc checkout_better {module {notrunk ""}}  {
    global notrunklist
	set temp [split $module "/"]
	foreach x $temp {
	foreach y $notrunklist {
	if { [string equal $x $y ] } {
			 set notrunk "bla"
			}
		}
	}
	set dirname [lindex $temp end]
	print_msg "Checking out $module"
    if {$dirname == ""} then { set dirname $module }
    #execute cvs checkout -P -d $dirname $module
    if {$notrunk == ""} then {
	    execute svn co https://dev.plt.rwth-aachen.de/acplt-repo/$module/trunk $dirname	
    } else {
	    execute svn co https://dev.plt.rwth-aachen.de/acplt-repo/$module $dirname	
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
	global release
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
	checkout archive fb_dbcommands "" notrunk

    cd $builddir/user
	if {$release == 1} {
		foreach x $included_libs {
		checkout_better $x
		}
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
	cd $builddir/base/ov/build/cygwin
    build_cygwin ov make -f makefile
	#cd $builddir/base/acplt_makmak/build/cygwin
    #build_cygwin acplt_makmak make -f makefile
	#enabling plt and ks just for fb_dbcommands	
	cd $builddir/base/plt/build/cygwin
    build_cygwin plt make -f makefile
	cd $builddir/base/ks/build/cygwin
    build_cygwin ks make -f makefile
	cd $builddir/base/fb_dbcommands/build/cygwin
    build_cygwin fb_dbcommands make -f makefile
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
		#enabling plt and ks just for fb_dbcommands
        cd $builddir/base/plt/build/ntvc
        build plt nmake /f $makefile
        cd $builddir/base/ks/build/ntvc
        build ks nmake /f $makefile
        cd $builddir/base/ov/build/ntvc
        build ov make -f $makefile -k
        cd $builddir/base/fb_dbcommands/build/ntvc
        build fb_dbcommands make -f $makefile -k
        cd $basedir
    } else {
		#enabling plt and ks just for fb_dbcommands
        build plt make -C $builddir/base/plt/build/$os
        build ks make -C $builddir/base/ks/build/$os
        build ov make -C $builddir/base/ov/build/$os
		build fb_dbcommands make -C $builddir/base/fb_dbcommands/build/$os
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
	#install solely fb_dbcommands executable
	if { $target == "linux" } then {
		file copy -force $builddir/base/fb_dbcommands/build/$target/fb_dbcommands $builddir/bin
	} else {
		file copy -force $builddir/base/fb_dbcommands/build/$target/fb_dbcommands.exe $builddir/bin
	}
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
    checkout_better $libname
	set temp [split $libname "/"]
	set libname [lindex $temp end]
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


proc release_lib_better {libname option} {
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
		#if { [file exists $releasedir/user/$libname.build/build/linux/$libname.a] } {
		#    file mkdir $releasedir/user/$libname/build/linux/
		#	file copy -force $releasedir/user/$libname.build/build/linux/$libname.a $releasedir/user/$libname/build/linux/
		#}
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
	 if { [file exists $releasedir] } then {
	file delete -force $releasedir
	}
	
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

#switch ov file structure
proc separate {} {
 global included_libs
 global releasedir
 global builddir
 global addon_libs
 
 if { [file exists $releasedir/system] } then {
	file delete -force $releasedir/system/
}
 if { [file exists $releasedir/addons] } then {
	file delete -force $releasedir/addons/
}

#create directories
 file mkdir $releasedir/system/
 file mkdir $releasedir/system/systools/
# file mkdir $releasedir/system/sysbin/
# file mkdir $releasedir/system/sysdevbase/
 #file mkdir $releasedir/system/userlibs/bin
 #file mkdir $releasedir/system/userlibs/include
 file mkdir $releasedir/doc/
 file mkdir $releasedir/system/addonlibs
 #file mkdir $releasedir/addons/tools
 #file mkdir $releasedir/addons/libs
 file mkdir $releasedir/servers/
 file mkdir $releasedir/servers/MANAGER
 file mkdir $releasedir/servers/MANAGER/modelinstances
 file mkdir $releasedir/servers/MANAGER/logfiles 
 #file mkdir $releasedir/modelstore/
 #file mkdir $releasedir/templatestore/
 #file mkdir $releasedir/user/tools 
 #file mkdir $releasedir/user/bin
 file mkdir $releasedir/dev
 file mkdir $releasedir/templates
 file mkdir $releasedir/user
 
 
 
 #release and move addon libs
 foreach x $addon_libs {
        set xt $x
		set temp [split $x "/"]
		set x [lindex $temp end]
		if { [file exists $releasedir/user/$x] } then {
			file delete -force $releasedir/user/$x
		}
		
		release_lib $xt all
		
		#file copy $releasedir/user/$x  $releasedir/addons/$x 
		#file delete -force $releasedir/user/$x 
		file copy $releasedir/user/libs/${x}.dll  $releasedir/system/addonlibs/${x}.dll 
		file copy $releasedir/user/$x $releasedir/dev/$x 
		file mkdir $releasedir/dev/$x/doc
		file mkdir $releasedir/dev/$x/source
		#file copy $releasedir/user/libs/${x}.dll  $releasedir/user/$x/bin/${x}.dll 
    }
	cd $builddir/user

	file copy $releasedir/bin/  $releasedir/system/sysbin/
if { [file exists $releasedir/include] } then {
	file copy $releasedir/include/  $releasedir/system/sysdevbase/
	file delete -force $releasedir/include
}

 #move system libs
 foreach x $included_libs {
 set temp [split $x "/"]
	set x [lindex $temp end]
	if { [file exists $releasedir/user/$x] } then {
		file copy $releasedir/user/$x  $releasedir/system/sysdevbase/$x 
		file delete -force $releasedir/user/$x 
		file copy $releasedir/user/libs/${x}.dll   $releasedir/system/sysbin/${x}.dll 
	}
	}
	
#move folders in the new structure
     
	#file copy $releasedir/user/libs  $releasedir/system/bin
	file delete -force $releasedir/user 
	#set binfiles [glob -nocomplain $releasedir/bin/*.*]

#	foreach x $binfiles {
  #     file copy $releasedir/bin/$x  $releasedir/system/sysbin/$x
	   
   # }
	
	
if { [file exists $releasedir/bin] } then {
	#file copy $releasedir/bin/  $releasedir/system/bin/
	file delete -force $releasedir/bin/
}

#if { [file exists $releasedir/lib] } then {
#	file copy $releasedir/lib/  $releasedir/system/lib/
	file delete -force $releasedir/lib
#}
if { [file exists $releasedir/model] } then {
	file mkdir $releasedir/system/sysdevbase/ov/
	file copy $releasedir/model/  $releasedir/system/sysdevbase/ov/model
	file delete -force $releasedir/model
}
if { [file exists $releasedir/database] } then {
	file delete -force $releasedir/database
}
if { [file exists $releasedir/servers/ov_server.conf.example] } then {
	file delete $releasedir/servers/ov_server.conf.example
}
if { [file exists $releasedir/system/sysbin/tclsh.exe] } then {
	file copy $releasedir/system/sysbin/tclsh.exe $releasedir/system/systools/tclsh.exe
	file delete -force $releasedir/system/sysbin/tclsh.exe
}
if { [file exists $builddir/base/ov/source/runtimeserver/ov_server.conf] } then {
	#file copy $builddir/base/ov/source/runtimeserver/ov_server.conf  $releasedir/servers/ov_server.conf.example
}

#including tools

cd $releasedir/system/systools
checkout_lib {base_servertools}
checkout_lib {base_serverstarttools}
file copy $releasedir/system/systools/base_servertools/build_database.tcl $releasedir/system/systools/build_database.tcl
file copy $releasedir/system/systools/base_servertools/start_server.tcl $releasedir/system/systools/start_server.tcl
#file copy $releasedir/system/systools/base_servertools/fb_dbcommands.exe $releasedir/system/sysbin/fb_dbcommands.exe
file delete -force $releasedir/system/systools/base_servertools/

file copy $releasedir/system/systools/base_serverstarttools/build_database.bat $releasedir/servers/MANAGER/build_database.bat
file copy $releasedir/system/systools/base_serverstarttools/db.ovd $releasedir/servers/MANAGER/db.ovd
file copy $releasedir/system/systools/base_serverstarttools/start_server.bat $releasedir/servers/MANAGER/start_server.bat
file copy $releasedir/system/systools/base_serverstarttools/ov_server.conf $releasedir/servers/MANAGER/ov_server.conf
file delete -force $releasedir/system/systools/base_serverstarttools/

cd $releasedir
}


# ============== MAIN STARTS HERE ==================
set included_libs {develop/ks/trunk/ksserv develop/fb develop/ks/trunk/ksservtcp develop/ks/trunk/ksservhttp develop/ks/trunk/ksapi develop/ks/trunk/ksapitcp }
set addon_libs { develop/hmi/cshmi develop/iec61131stdfb develop/IOdriverlib develop/ks/trunk/fbcomlib }
set notrunklist {ks}
#iec61131stdfb IOdriverlib fbcomlib
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
	separate
#file rename -force $releasedir/user $releasedir/system



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
    separate
	if { [file exists $releasedir/dev/] } then {
	file delete -force $releasedir/dev/
}
    #cd $releasedir/system/user
	#foreach x $included_libs {
    #    file delete -force $x
    #}

    cd $releasedir/system/sysbin
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
   cd $releasedir/system
    file delete -force sysdevbase
	file mkdir  sysdevbase
    #file delete -force lib
	#cd $releasedir/system/ov
	#file delete -force model
	cd $basedir
		
	
	
	
	#if { [file exists $releasedir/system/bin/tclsh.exe] } then {
	#file copy  $releasedir/system/bin/tclsh.exe $releasedir/system/tclsh.exe
	#file delete $releasedir/system/bin/tclsh.exe
#}
set stripfiles [glob -nocomplain $releasedir/system/sysbin/*.*]

	foreach x $stripfiles {
       
	  # execute  "strip --strip-debug" $x
	  execute  "strip" "-g" "-S" "-d" $x
    }
	#file copy  $releasedir/system/tclsh.exe $releasedir/system/bin/tclsh.exe 
	#	file delete $releasedir/system/tclsh.exe


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
