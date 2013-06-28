
# ACPLT Build Script
# (c) 2011 Chair of Process Control Engineering, RWTH Aachen University
# Author: Gustavo Quiros <g.quiros@plt.rwth-aachen.de>
# Author: Sten Gruener   <s.gruener@plt.rwth-aachen.de>
#
# Usage: tclsh acplt_build.tcl (release) (checkout)
set release 0
set checkout 0 
set libsuffix 0
set exesuffix 0
set bleedingedge 0
set notbuildedlibs 0
foreach arg $argv {
	if {$arg == "checkout"} {
		set release 0
		set checkout 1
		
	}
	if {$arg == "release"} {
		set release 1
		set checkout 0
		
	}
	if {$arg == "bleedingedge"} {
		set bleedingedge 1
	
	}
}



set basedir [pwd]

set builddir $basedir/acplt.build
set releasedir $basedir/acplt
set flag 0;
set logfile $basedir/acplt_build.log

#if {![info exists env(CVSROOT)]} then {
#    puts stderr "Please set the environment variable CVSROOT and try again."
#    exit 1
#c}

# Determine operating system
if {$tcl_platform(os) == "Linux"} then { 
    set os "linux" 
    set make "make"
	set libsuffix ".so"
	set exesuffix ""
	set batsuffix ".sh"
} elseif {[lsearch $tcl_platform(os) "Windows"] >= 0} then {
    set os "nt"
    set make "mingw32-make"
    set env(CYGWIN) "nodosfilewarning" 
	set libsuffix ".dll"
	set exesuffix ".exe"
	set batsuffix ".bat"
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
# return 0 if success
# return 1 if fail
proc execute {args} {
	set args [join $args]
    global logfile
    #set cmd [concat {exec -ignorestderr} $args]
    set cmd [concat {exec } $args { >>& $logfile}]
    #puts $cmd
    if { [catch $cmd msg] } {
        puts stderr "error: $msg"
        puts stderr "Consult the file '$logfile'"
        #exit 1
		return 1
    }
	return 0
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
	global releasedir
    global os
    print_msg "Creating directory structure"
    file mkdir $builddir $builddir/bin $builddir/user $builddir/user/libs $builddir/database $builddir/lib
	file delete -force $releasedir
	file mkdir $releasedir
	file mkdir $releasedir/system
	file mkdir $releasedir/system/sysdevbase
	file mkdir $releasedir/system/sysbin
	file mkdir $releasedir/dev
	file mkdir $releasedir/system/addonlibs
	file mkdir $releasedir/doc/
	file mkdir $releasedir/templates
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
	global releasedir
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

    cd $releasedir/dev
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
	
    return [execute $args]
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

#proc start_server {} {
#    global releasedir
#    global os
#    set database $releasedir/database/database.ovd
#    if [file exists $database] then {
#    } else {
#        dbutil -f $database -c [expr 1024 * 1024]
#    }
#    execute $releasedir/bin/tmanager.exe &
#    if { $os == "linux" } then {
#        global env
#        set env(LD_LIBRARY_PATH) $releasedir/bin
#    }
#    execute $releasedir/bin/ov_server -f $database -s fb_server -w fb -w iec61131stdfb 
#}

proc release_lib {libname} {
	relase_lib libname "all"
}

proc release_lib {libname option} {
    global releasedir
    global os
    global make
    cd $releasedir/dev/
    file delete -force $releasedir/dev/$libname/
    checkout_better $libname
	set temp [split $libname "/"]
	set libname [lindex $temp end]
    cd $releasedir/dev/$libname/build/$os/
    if { $option == "all" } then {
	    print_msg "Note: no debug symbols will be created"
    }
    build $libname $make $option
    print_msg "Deploying $libname"
    file delete -force $releasedir/dev/$libname.build/
    file copy -force $releasedir/dev/$libname/ $releasedir/dev/$libname.build/
    file delete -force $releasedir/dev/$libname/
    file mkdir $releasedir/dev/$libname/
    file mkdir $releasedir/dev/$libname/model/
    copy_wildcard $releasedir/dev/$libname.build/model/*.ov? $releasedir/dev/$libname/model/
    file mkdir $releasedir/dev/$libname/include/
    copy_wildcard $releasedir/dev/$libname.build/include/*.h $releasedir/dev/$libname/include/
    #export libname.a file for compiling under windows
    if { $os == "nt" } then {
		#if { [file exists $releasedir/user/$libname.build/build/nt/$libname.a] } {
		#		file copy -force $releasedir/user/$libname.build/build/nt/$libname.a $releasedir/user/$libname/build/nt/
		#}
		if { [file exists $releasedir/dev/$libname.build/build/nt/$libname.lib] } {
		    file mkdir $releasedir/dev/$libname/build/nt/
			file copy -force $releasedir/dev/$libname.build/build/nt/$libname.lib $releasedir/dev/$libname/build/nt/
		}
    }
    if { $os == "linux" } then {
		#if { [file exists $releasedir/user/$libname.build/build/nt/$libname.a] } {
		#		file copy -force $releasedir/user/$libname.build/build/nt/$libname.a $releasedir/user/$libname/build/nt/
		#}
		if { [file exists $releasedir/dev/$libname.build/build/linux/$libname.a] } {
		    file mkdir $releasedir/dev/$libname/build/linux/
			file copy -force $releasedir/dev/$libname.build/build/linux/$libname.a $releasedir/dev/$libname/build/linux/
		}
    }
    file delete -force $releasedir/dev/$libname.build/
}


proc release_lib_better {libname option} {
	global releasedir
	global os
	global make
	cd $releasedir/dev/
	file delete -force $releasedir/dev/$libname/
	checkout_better $libname
	set temp [split $libname "/"]
	set libnametemp [lindex $temp end]
	set libnamepraefix [lindex $temp end-1]
	#print_msg "$libnamepraefix"
	#print_msg "$releasedir/dev/$libnametemp"
	
	set libs [glob -types d -tails -nocomplain -directory $releasedir/dev/$libnametemp "*"] 
	
	#print_msg "$libs"
	print_msg "$libnametemp"
	#lib contains the list of the libs to build
	if {[lsearch $libs "source"] > -1 } { set libs $libnametemp }
	#correct the paths... lifing libraries up fomr the "core"	dir
	foreach lib $libs {
		#set libname $lib
		if { [file exists $releasedir/dev/$libnametemp/$lib] && $lib != "ov" } {
			
			file copy -force $releasedir/dev/$libnametemp/$lib $releasedir/dev/$lib
			file delete -force $releasedir/dev/$libnametemp/$lib
		}
	}

	if {[lsearch [glob -types d -tails -nocomplain -directory $releasedir/dev/$libnametemp "*"] "source"] == -1 } { file delete -force $releasedir/dev/$libnametemp 
	#print_msg "B���HM"
	}
	
	set not_yet_build $libs
	#the array should be used later, if someone will write dependency detection
	#ov is built
	set built {"ov"}
	set index_ov [lsearch $not_yet_build "ov"]
	set not_yet_build [lreplace $not_yet_build $index_ov $index_ov]
	set break_in_next_iteration 0
	while { [llength $not_yet_build]>0 && $break_in_next_iteration==0 } {
		#break if no successful compile
		set break_in_next_iteration 1 
		foreach libname $not_yet_build {
			cd $releasedir/dev/$libname/build/$os/
			if { $option == "all" } then {
				print_msg "Note: no debug symbols will be created"
			}
			set success [build $libname $make $option]
			
			if { $success == 0 } {
				#iterate once more
				set break_in_next_iteration 0 
				#removing $libname from not_yet_build
				set index [lsearch $not_yet_build $libname]
				set not_yet_build [lreplace $not_yet_build $index $index]
				#$libname is now built
				lappend built $libname
				
				#deploying
				print_msg "Deploying $libname"
				file delete -force $releasedir/dev/$libname.build/
				file copy -force $releasedir/dev/$libname/ $releasedir/dev/$libname.build/
				file delete -force $releasedir/dev/$libname/
				file mkdir $releasedir/dev/$libname/
				file mkdir $releasedir/dev/$libname/model/
				copy_wildcard $releasedir/dev/$libname.build/model/*.ov? $releasedir/dev/$libname/model/
				file mkdir $releasedir/dev/$libname/include/
				copy_wildcard $releasedir/dev/$libname.build/include/*.h $releasedir/dev/$libname/include/
				if { $os == "linux" } then {
					if { [file exists $releasedir/dev/$libname.build/build/linux/$libname.a] } then {
						file mkdir $releasedir/dev/$libname/build/linux/
						file copy -force $releasedir/dev/$libname.build/build/linux/$libname.a $releasedir/dev/$libname/build/linux/
					}
				}
				file delete -force $releasedir/dev/$libname.build/
			} else {
				print_msg "$libname may have unmet dependencies, retrying next iteration"
			}
		}
	}
	if { [llength $not_yet_build] > 0 } {
		append notbuildedlibs $not_yet_build
		print_msg "Libraries $not_yet_build could not be built, giving up"
	}
	
	#foreach lib $libs {
	#	set libname $lib
	#	if { [file exists $releasedir/dev/$libnametemp/$libname] } {
	#		file copy -force $releasedir/dev/$libnametemp/$libname $releasedir/dev/$libname
	#		file delete -force $releasedir/dev/$libnametemp/$libname
	#	}
	#	cd $releasedir/dev/$libname/build/$os/
	#	if { $option == "all" } then {
	#		print_msg "Note: no debug symbols will be created"
	#	}
	#	build $libname $make $option
	#	print_msg "Deploying $libname"
	#	file delete -force $releasedir/dev/$libname.build/
	#	file copy -force $releasedir/dev/$libname/ $releasedir/dev/$libname.build/
	#	file delete -force $releasedir/dev/$libname/
	#	file mkdir $releasedir/dev/$libname/
	#	file mkdir $releasedir/dev/$libname/model/
	#	copy_wildcard $releasedir/dev/$libname.build/model/*.ov? $releasedir/dev/$libname/model/
	#	file mkdir $releasedir/dev/$libname/include/
	#	copy_wildcard $releasedir/dev/$libname.build/include/*.h $releasedir/dev/$libname/include/
	#	#export libname.a file for compiling under windows
	#	if { $os == "nt" } then {
	#		#if { [file exists $releasedir/user/$libname.build/build/nt/$libname.a] } {
	#		#		file copy -force $releasedir/user/$libname.build/build/nt/$libname.a $releasedir/user/$libname/build/nt/
	#		#}
	#		if { [file exists $releasedir/dev/$libname.build/build/nt/$libname.lib] } then {
	#			file mkdir $releasedir/dev/$libname/build/nt/
	#			file copy -force $releasedir/dev/$libname.build/build/nt/$libname.lib $releasedir/dev/$libname/build/nt/
	#		}
	#	}
	#	if { $os == "linux" } then {
	#		#if { [file exists $releasedir/user/$libname.build/build/nt/$libname.a] } {
	#		#		file copy -force $releasedir/user/$libname.build/build/nt/$libname.a $releasedir/user/$libname/build/nt/
	#		#}
	#		if { [file exists $releasedir/dev/$libname.build/build/linux/$libname.a] } then {
	#			file mkdir $releasedir/dev/$libname/build/linux/
	#			file copy -force $releasedir/dev/$libname.build/build/linux/$libname.a $releasedir/dev/$libname/build/linux/
	#		}
	#	}
	#	file delete -force $releasedir/dev/$libname.build/
	#}
	#if {[lsearch $libs "source"] == -1 } { file delete -force $releasedir/dev/$libnametemp }
}	





proc create_release {} {
	global os
	global releasedir
	global builddir
	global env

    #create a release
	set env(ACPLT_HOME) $releasedir
	if { $os == "nt" } then {
		set env(PATH) $releasedir/system/sysbin/\;$env(PATH)
	} else {
		set env(PATH) $releasedir/system/sysbin/:$env(PATH)
	}

	print_msg "Creating release in $releasedir"
	# if { [file exists $releasedir] } then {
	#file delete -force $releasedir
	#}
	
	file mkdir $releasedir
	#bin dir
	#file mkdir $releasedir/bin
	set libfiles [concat [glob -nocomplain $builddir/bin/*]]
	foreach file $libfiles {
		file copy -force $file $releasedir/system/sysbin/
	}
	#lib dir
	file mkdir $releasedir/system/sysdevbase/ov/lib/
	#if { $os == "nt" } then {
	#	set lib ".lib"
	#}
	set lib ".a"

	set libfiles [list "$builddir/lib/libov$lib"]
	foreach file $libfiles {
		file copy -force $file $releasedir/system/sysdevbase/ov/lib/
	}
	if { $os == "nt" } then {
		file copy -force "$builddir/lib/oncrpc$lib" $releasedir/system/sysdevbase/ov/lib/
	#	file copy -force "$builddir/lib/oncrpcms$lib" $releasedir/lib/
	}
	#model dir
	file mkdir $releasedir/system/sysdevbase/ov/model
	set libfiles [concat [glob -nocomplain $builddir/base/ov/model/ov.*]]
	foreach file $libfiles {
		file copy -force $file $releasedir/system/sysdevbase/ov/model
	}
	#database dir
	#file mkdir $releasedir/database
	#include dir
	file mkdir $releasedir/system/sysdevbase/ov/include
	file mkdir $releasedir/system/sysdevbase/ov/include/ks
	copy_wildcard $builddir/base/ks/include/ks/*.h $releasedir/system/sysdevbase/ov/include/ks
	file mkdir $releasedir/system/sysdevbase/ov/include/libov
	copy_wildcard $builddir/base/ov/include/libov/*.h $releasedir/system/sysdevbase/ov/include/libov
	file mkdir $releasedir/system/sysdevbase/ov/include/libovks
	copy_wildcard $builddir/base/ov/include/libovks/*.h $releasedir/system/sysdevbase/ov/include/libovks
	file mkdir $releasedir/system/sysdevbase/ov/include/plt
	copy_wildcard $builddir/base/plt/include/plt/*.h $releasedir/system/sysdevbase/ov/include/plt
	if { $os == "nt" } then {
		file mkdir $releasedir/system/sysdevbase/ov/include/rpc
		copy_wildcard $builddir/oncrpc/rpc/*.h $releasedir/system/sysdevbase/ov/include/rpc
	}
	#user dir
	file mkdir $releasedir/dev
	#file mkdir $releasedir/user/libs
	#download tclsh
	if { $os == "nt" } then {
		print_msg "Downloading tclsh.exe"
		cd $releasedir/system/sysbin
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
 global libsuffix
 global	exesuffix
 global flag
 global os
 
 set libs [glob -types d -tails -nocomplain -directory $releasedir/dev "*"]
 
 #move system libs
#print_msg "$libs" 
 foreach x $libs {
 set temp [split $x "/"]
	set x [lindex $temp end]
	if { [file exists $releasedir/dev/$x] } then {
		file copy $releasedir/dev/$x  $releasedir/system/sysdevbase/$x 
		file delete -force $releasedir/dev/$x 
	}
	if { [file exists $releasedir/system/addonlibs/${x}$libsuffix] } then {
		file copy $releasedir/system/addonlibs/${x}$libsuffix   $releasedir/system/sysbin/${x}$libsuffix 
		file delete -force $releasedir/system/addonlibs/${x}$libsuffix
	}
 }
if { [file exists $releasedir/system/sysbin/tmanager.exe] } then {
		file delete -force $releasedir/system/sysbin/tmanager.exe 
	}
	cd $releasedir
}

proc create_systools_and_servers {} {
	global releasedir
	global batsuffix
	
	file mkdir $releasedir/servers/
	file mkdir $releasedir/servers/MANAGER
	file mkdir $releasedir/servers/MANAGER/modelinstances
	file mkdir $releasedir/servers/MANAGER/logfiles 
	
	if { [file exists $releasedir/servers/ov_server.conf.example] } then {
		file delete $releasedir/servers/ov_server.conf.example
	}
	


	#including sys tools
	cd $releasedir/system
	checkout_lib {systools}
	#file copy $releasedir/system/systools/systools/fb_dbcommands.exe $releasedir/system/sysbin/fb_dbcommands.exe
	file delete -force $releasedir/system/systools/.svn/
	
	#serverstarttools
	checkout_lib {base_serverstarttools}
	set files [glob -nocomplain $releasedir/system/base_serverstarttools/*$batsuffix]
    foreach f $files {
		file copy $f $releasedir/servers/MANAGER/
    }
	
	file copy $releasedir/system/base_serverstarttools/ov_server.conf $releasedir/servers/MANAGER/ov_server.conf 
	file delete -force $releasedir/system/base_serverstarttools/	

}

# ============== MAIN STARTS HERE ==================
if { $bleedingedge == 1 } then {
	set included_libs {develop/ks/trunk/ksbase develop/ks/trunk/TCPbind develop/ks/trunk/ksxdr develop/ks/trunk/kshttp  develop/ks/trunk/ksapi develop/fb develop/shutdown}
	set addon_libs { develop/hmi/cshmi develop/iec61131stdfb develop/IOdriverlib archive/vdivde3696 }
	
	print_msg "if you take the new ones, you'll see what you get"
} else {
    print_msg "taking the safe way is a sign of weakness"
	set addon_libs { common/user }
	set included_libs {common/core}
	}

set notrunklist { ks common ServiceSystem }
#iec61131stdfb IOdriverlib fbcomlib develop/ks/trunk/MessageSys develop/ServiceSystem/trunk/ServiceProvider develop/ACPLTlab003lindyn
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
create_systools_and_servers

foreach x $included_libs {
	release_lib_better $x debug
}
#move included_libs to system
separate
foreach x $addon_libs {
	release_lib_better $x debug
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
    set date [clock format [clock seconds] -format "%Y%m%d"]
    set name "acplt-server-runtime-$date"

    create_dirs
	create_release
	create_systools_and_servers
    foreach x $included_libs {
        release_lib_better $x all
    }
    separate
    foreach x $addon_libs {
        release_lib_better $x all
    }
	if { [file exists $releasedir/dev/] } then {
		file delete -force $releasedir/dev/
	}

    cd $releasedir/system/sysbin
    file delete -force acplt_makmak$exesuffix
    file delete -force acplt_builder$exesuffix
    file delete -force ov_builder$exesuffix
    file delete -force ov_codegen$exesuffix
    file delete -force ov_makmak$exesuffix
    file delete -force $releasedir/system/sysdevbase
    #file delete -force lib
	#cd $releasedir/system/ov
	#file delete -force model
	cd $basedir
		
	
	#temporaly move tclsh.exe so it is not stripped
	if { [file exists $releasedir/system/sysbin/tclsh.exe] } then {
		file copy  $releasedir/system/sysbin/tclsh.exe $releasedir/system/tclsh.exe
		file delete -force $releasedir/system/sysbin/tclsh.exe
	}

	set stripfiles [glob -nocomplain $releasedir/system/sysbin/*.*]

	foreach x $stripfiles {
       
	  # execute  "strip --strip-debug" $x
	  execute  "strip" "-g" "-S" "-d" $x
    }
	
	#restore tclsh.exe so it is not stripped
	if { [file exists $releasedir/system/tclsh.exe] } then {
		file copy  $releasedir/system/tclsh.exe $releasedir/system/sysbin/tclsh.exe 
		file delete -force $releasedir/system/tclsh.exe
	}


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
