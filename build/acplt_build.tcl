# ACPLT Build Script
# (c) 2016 Chair of Process Control Engineering, RWTH Aachen University
# Author: Gustavo Quiros <g.quiros@plt.rwth-aachen.de>
# Author: Sten Gruener   <s.gruener@plt.rwth-aachen.de>
# Author: Constantin Wagner <c.wagner@plt.rwth-aachen.de>
# Author: Holger Jeromin <h.jeromin@plt.rwth-aachen.de>
# Author: Lars Evertz <l.evertz@plt.rwth-aachen.de>
#
# Usage: tclsh acplt_build.tcl (release) (compileonly) (cross FILENAME) (repo REPOSITORY_URL)
set release 0
set checkout 0
set compileonly 0
#build and fb_dbcommands and therefor c++ ks
set build_dbcommands 1
set libsuffix 0
set exesuffix 0
set notbuildedlibs 0
set ov_debug "OV_DEBUG=1"
set ov_arch_bitwidth_str "OV_ARCH_BITWIDTH=32"
set ov_arch_bitwidth_int 32
set cross 0
set crossFilename ""
set targetOS ""
set crossArch ""
set gitRepo "https://github.com/acplt/rte/trunk/"


foreach arg $argv {
	if {$arg == "checkout"} {
		set release 0
		set checkout 1
	}
	if {$arg == "release"} {
		set release 1
		set checkout 0
	}
	if {$arg == "compileonly"} {
		set compileonly 1
	}
	if {$arg == "no_dbcommands"} {
		set build_dbcommands 0
	}
	if {$arg == "32"} {
		set ov_arch_bitwidth_str "OV_ARCH_BITWIDTH=32"
		set ov_arch_bitwidth_int 32
	}
	if {$arg == "64"} {
		set ov_arch_bitwidth_str "OV_ARCH_BITWIDTH=64"
		set ov_arch_bitwidth_int 64
	}
	if {$crossFilename == 1} {
		set crossFilename $arg
	}
	if {$arg=="cross"} {
		set cross 1
		set crossFilename 1
	}
	if {$gitRepo == 1} {
		set gitRepo $arg
	}
	if {$arg=="repo"} {
		set gitRepo 1
	}
}


set basedir [pwd]

set builddir $basedir/acplt.build
set releasedir $basedir/acplt
set flag 0;
set logfile $basedir/acplt_build.log

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

#read in cross settings if wanted
if {$cross==1} {
	source $crossFilename
	if {$targetOS=="nt"} {
		set build_dbcommands 0
		set libsuffix ".dll"
		set exesuffix ".exe"
		set batsuffix ".bat"
	} else {
		set libsuffix ".so"
		set exesuffix ""
		set batsuffix ".sh"
	}
	
} else {
	set targetOS $os
}



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
		puts stderr "error while executing: $cmd"
		puts stderr "error: $msg"
		puts stderr "Consult the file '$logfile'"
		exit 1
		#return 1
	}
	return 0
}

# Same as execute, just ignores errors
proc execute_ignore {args} {
	set args [join $args]
	
	global logfile
	#set cmd [concat {exec -ignorestderr} $args]
	set cmd [concat {exec } $args { >>& $logfile}]
	#puts $cmd
	if { [catch $cmd msg] } {
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
proc move_wildcard {src target} {
	set files [concat [glob -nocomplain $src]]
	foreach file $files {
		file copy -force $file $target
		file delete -force $file
	}
}

# Create directory structure
proc create_dirs {} {
	global builddir
	global releasedir
	global os
	print_msg "Creating directory structure"
	file mkdir $builddir $builddir/bin $builddir/lib
	if {[file exists $releasedir] == 1} then {
		#on windows the mkdir of a fresh deleted file often gives a permission denied
		file rename -force $releasedir tempdelete
		file delete -force tempdelete
	}
	file mkdir $releasedir
	file mkdir $releasedir/system
	file mkdir $releasedir/system/sysdevbase
	file mkdir $releasedir/system/sysbin
	file mkdir $releasedir/system/syslibs
	file mkdir $releasedir/dev
	file mkdir $releasedir/system/addonlibs
	file mkdir $releasedir/doc/
	file mkdir $releasedir/templates
}


proc get_revision {} {
	global logfile
	if {[file isdirectory "../.git"]} {
		return [exec git describe --abbrev=7 --dirty --always --tags]
	}

	#this one gets git commit id of the trunk = master branch
	#a bit hacky due to hard-coded uri, but this is the only way I found
	execute_ignore svn info svn info https://github.com/acplt/rte/trunk
	set in [open $logfile r]
	set first 0
	set revision 0
	#first we need to extract the revision of last change
	while {[gets $in line] != -1} {
		if {[regexp {.*Rev:\s+([0-9]+).*} $line] } then {
				regexp {.*Rev:\s+([0-9]+).*} $line _ first 
				set revision $first
				break
		   }
	}
	close $in
	if {$revision != 0} {
		execute_ignore svn propget git-commit --revprop -r $revision
	}
	set in [open $logfile r]
	set first 0
	while {[gets $in line] != -1} {
		if {[regexp {([0-9a-z]{30})} $line] } then {
				regexp {([0-9a-z]{30})} $line _ first 
				set line [string range $line 0 6]
				print_msg "Commit hash: $line"
				break
		   }
		set line "unknown"
	}
	close $in


	return $line
}


# Checkout a SVN module or copy if the file are there (from git?)
proc checkout_dir {module {vcs_server "github"} {dirname ""} } {
	global basedir
	global gitRepo
	set temp [split $module "/"]
	if {$dirname == ""} {
		set dirname [lindex $temp end]
	}
	if {[file isdirectory $basedir/../$module]} {
		file copy -force $basedir/../$module ./$dirname
		print_msg "reused directory $module"
		return
	}
	if {$vcs_server == "github"} {
		print_msg "Checking out $module from github"
		execute svn co ${gitRepo}${module} $dirname
	} elseif {$vcs_server == "acpltpublish"} {
		print_msg "Checking out $module from acplt publish"
		execute svn co https://dev.plt.rwth-aachen.de/acplt-repo/publish/$module $dirname
	} elseif {$vcs_server == "acpltroot"} {
		print_msg "Checking out $module from acplt root"
		execute svn co https://dev.plt.rwth-aachen.de/acplt-repo/$module/trunk $dirname
	} elseif {$vcs_server == "acpltrootnotrunk"} {
		print_msg "Checking out $module from acplt root"
		execute svn co https://dev.plt.rwth-aachen.de/acplt-repo/$module $dirname
	}
}

# Checkout sources
proc checkout_acplt {} {
	global builddir
	global basedir
	global os
	global system_libs
	global releasedir
	global release
	global date
	global build_dbcommands
	
	cd $builddir
	if {$build_dbcommands == 1} {
		checkout_dir "legacy/oncrpc"
		checkout_dir "legacy/comm" "github" "base"
		cd $builddir/base
		checkout_dir "legacy/fbs_dienste"
	} else {
		file mkdir $builddir/base
		cd $builddir/base
	}
	checkout_dir "core/ov"

	#get the number of the current release - $date is global
	cd $basedir
	set date [get_revision]
 }

# Build in a directory
proc build_package {package args} {
	global ov_debug
	global ov_arch_bitwidth_str
	global crossArch
	print_msg "Building $package via build_package"
	if {$crossArch == "ARM"} {
		set crossOverrideBitwidthFlags	"OV_ARCH_BITWIDTH_CFLAGS= OV_ARCH_BITWIDTH_LDFLAGS= "	
		return [execute $args $ov_debug $ov_arch_bitwidth_str $crossOverrideBitwidthFlags]
	} else {
		return [execute $args $ov_debug $ov_arch_bitwidth_str]
	}
}

# Build in a directory using cygwin bash and ignoring errors
proc build_cygwin {package args} {
	global bash
	global ov_debug
	global ov_arch_bitwidth_str
	print_msg "Building $package via build_cygwin"
	
	eval [concat "execute" $bash \\\"$args $ov_debug $ov_arch_bitwidth_str\\\"]
}

proc build_acplt_mingw {} {
	print_msg "Compiling acplt with mingw32"
	global base
	global os
	global make
	global basedir
	global builddir
	global build_dbcommands
	
	if {$build_dbcommands == 1} {
		print_msg "Building oncrpc for fb_dbcommands"
		cd $builddir/oncrpc/
		execute makemingw.bat
	}
	cd $builddir/base/ov/source/libml
	build_cygwin libml make -f mingw.mk
	#cd $builddir/base/libmpm 
	#build_cygwin libmpm make -f Makefile
	cd $builddir/base/ov/build/cygwin
	build_cygwin ov make -f makefile
	#cd $builddir/base/acplt_makmak/build/cygwin
	#build_cygwin acplt_makmak make -f makefile
	#enabling plt and ks just for fb_dbcommands	
	if {$build_dbcommands == 1} {
		cd $builddir/base/plt/build/cygwin
		build_cygwin plt make -f makefile
		cd $builddir/base/ks/build/cygwin
		build_cygwin ks make -f makefile
		cd $builddir/base/fbs_dienste/build/cygwin
		build_cygwin fb_dbcommands make -f makefile
	}
}

# Build ACPLT (msvc in windows [depricated] and gcc in linux)
proc build_acplt {} {
	global builddir
	global base
	global os
	global make
	global basedir
	global build_dbcommands
	global cross
	global CrossPrefix
	global targetOS
	global crossArch
	variable crossArgs

	if { $os == "nt" } then { set makefile "msvc.mk" } else { set makefile "Makefile" }
#libml
	if {$cross==1} { 
		set crossArgs "GCC_BIN_PREFIX=$CrossPrefix"
		if {$crossArch == "ARM"} {
			build_package libml make -C $builddir/base/ov/source/libml -f $makefile $crossArgs OV_ARCH_BITWIDTH_CFLAGS= OV_ARCH_BITWIDTH_LDFLAGS= 
		} else {
			build_package libml make -C $builddir/base/ov/source/libml -f $makefile $crossArgs
		}
	} else {
		build_package libml make -C $builddir/base/ov/source/libml -f $makefile
	}

	if { $os == "nt" && $build_dbcommands == 1 } then { 
		cd $builddir/oncrpc
		execute make.bat
		cd $basedir
	}
	# build libmpm make -C $builddir/base/libmpm -f $makefile
	if { $os == "nt" } then {
		if {$build_dbcommands == 1} {
			#enabling plt and ks just for fb_dbcommands
			cd $builddir/base/plt/build/ntvc
			build_package plt nmake /f $makefile
			cd $builddir/base/ks/build/ntvc
			build_package ks nmake /f $makefile
			cd $builddir/base/fb_dbcommands/build/ntvc
			build_package fb_dbcommands make -f $makefile -k
		}
		cd $builddir/base/ov/build/ntvc
		build_package ov make -f $makefile -k
		cd $basedir
	} else {
		if {$build_dbcommands == 1} {
			if {$cross==1} { 
				set crossArgsPrefix "GCC_BIN_PREFIX=$CrossPrefix"
				#enabling plt and ks just for fb_dbcommands
				build_package plt make -C $builddir/base/plt/build/$os $crossArgsPrefix
				build_package ks make -C $builddir/base/ks/build/$os $crossArgsPrefix
				build_package fbs_dienste make -C $builddir/base/fbs_dienste/build/$os $crossArgsPrefix
			} else {
				#enabling plt and ks just for fb_dbcommands
				build_package plt make -C $builddir/base/plt/build/$os
				build_package ks make -C $builddir/base/ks/build/$os
				build_package fbs_dienste make -C $builddir/base/fbs_dienste/build/$os
			}
			
		}
		set crossArgsPrefix ""
		set crossArgsCGDir ""
		set crossArgsCG	""
		if {$cross==1} { 
			set crossArgsPrefix "GCC_BIN_PREFIX=$CrossPrefix"
			set crossArgsCGDir "OV_CODEGEN_DIR= "
			set crossArgsCG	"OV_CODEGEN_EXE=ov_codegen"
			if {$targetOS=="nt"} {
				set crossWindresDefs "WINDRESDEFS=--define _WIN32"
				build_package ov make -C $builddir/base/ov/build/cygwin $crossArgsPrefix $crossArgsCGDir $crossArgsCG $crossWindresDefs 
			} else {
				if {$crossArch == "ARM"} {
					build_package ov make -C $builddir/base/ov/build/$os $crossArgsPrefix $crossArgsCGDir $crossArgsCG OV_ARCH_BITWIDTH_CFLAGS= OV_ARCH_BITWIDTH_LDFLAGS= 
				} else {
					build_package ov make -C $builddir/base/ov/build/$os $crossArgsPrefix $crossArgsCGDir $crossArgsCG
				}
			}
		} else {
			build_package ov make -C $builddir/base/ov/build/$os
		}
   }
   #if { $os == "nt" } then {
   #	build_package acplt_makmak $make -C $builddir/base/acplt_makmak/build/ntvc
   #} else {
   #	build_package acplt_makmak $make -C $builddir/base/acplt_makmak/build/linux
   #}
}

proc install_dir {dir} {
	global builddir
	global targetOS
	print_msg "Installing from $dir"
	if { $targetOS == "linux" } then {
		set binfiles [concat [glob -nocomplain $dir/*.so] [glob -nocomplain -type {f x} $dir/*]]
		set libfiles [concat [glob -nocomplain $dir/*.a]]
	} 
	if { $targetOS == "nt" } then {
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
	global build_dbcommands
	
	if {$build_dbcommands == 1} {
		#install solely fb_dbcommands executable
		if { $target == "linux" } then {
			file copy -force $builddir/base/fbs_dienste/build/$target/fb_dbcommands $builddir/bin
		} else {
			file copy -force $builddir/base/fbs_dienste/build/$target/fb_dbcommands.exe $builddir/bin
		}
	}
	install_dir $builddir/base/ov/build/$target
	#install_dir $builddir/base/acplt_makmak/build/$target
}

proc release_lib_better {libname option} {
	global releasedir
	global os
	global make
	global compileonly
	global cross	
	global targetOS
	global CrossPrefix
	global crossArch

	cd $releasedir/dev/
	if { $compileonly != 1 } then {
		file delete -force $releasedir/dev/$libname/
		checkout_dir $libname
	}
	set temp [split $libname "/"]
	set libnametemp [lindex $temp end]
	set libnamepraefix [lindex $temp end-1]

	set libs [glob -types d -tails -nocomplain -directory $releasedir/dev/$libnametemp "*"] 
	
	print_msg "prepare $libnametemp with release_lib_better"
	#lib contains the list of the libs to build
	if {[lsearch $libs "source"] > -1 } { set libs $libnametemp }
	#correct the paths... lifting libraries up from the "core" dir
	foreach lib $libs {
		#set libname $lib
		if { [file exists $releasedir/dev/$libnametemp/$lib] && $lib != "ov" } {
			file copy -force $releasedir/dev/$libnametemp/$lib $releasedir/dev/$lib
			file delete -force $releasedir/dev/$libnametemp/$lib
		}
	}

	if {[lsearch [glob -types d -tails -nocomplain -directory $releasedir/dev/$libnametemp "*"] "source"] == -1 } {
		file delete -force $releasedir/dev/$libnametemp
		#print_msg "BÄÄÄHM"
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
			cd $releasedir/dev/$libname/build/$targetOS/
			
			if { $option == "all" } then {
				print_msg "Note: no debug symbols will be created"
			}
			if {$cross == 1} {
				if {$targetOS == "nt"} {
					set MAKMAKOPTION "TARGETOS=--targetWindows"
				} else {
					set MAKMAKOPTION "TARGETOS=--targetLinux"
				}
				if {$crossArch == "ARM"} {
					set MAKMAKTEMP "${MAKMAKOPTION} -mARM"
					set MAKMAKOPTION $MAKMAKTEMP
				}
				set success [build_package $libname $make $option GCC_BIN_PREFIX=$CrossPrefix BIN_DIR= $MAKMAKOPTION]
			} else {			
				set success [build_package $libname $make $option]
			}
			
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
				if {$option == "debug"} {
					#OLD CODE follows - we did not release source than
					#file delete -force $releasedir/dev/$libname.build/
					#file copy -force $releasedir/dev/$libname/ $releasedir/dev/$libname.build/
					#file delete -force $releasedir/dev/$libname/
					#file mkdir $releasedir/dev/$libname/
					#file mkdir $releasedir/dev/$libname/model/
					#copy_wildcard $releasedir/dev/$libname.build/model/*.ov? $releasedir/dev/$libname/model/
					#file mkdir $releasedir/dev/$libname/include/
					#copy_wildcard $releasedir/dev/$libname.build/include/*.h $releasedir/dev/$libname/include/
					#if { $os == "linux" } then {
					#	if { [file exists $releasedir/dev/$libname.build/build/linux/$libname.a] } then {
					#		file mkdir $releasedir/dev/$libname/build/linux/
					#		file copy -force $releasedir/dev/$libname.build/build/linux/$libname.a $releasedir/dev/$libname/build/linux/
					#	}
					#}
					#file delete -force $releasedir/dev/$libname.build/
					#NEW CODE: release everything: just make sure to clean up .svn information
					remove_svn_dirs "$releasedir/dev/$libname"
				} else {
					#No need to anything here - dev will be deleted entirely by the runtime release
				}
			} else {
				print_msg "$libname may have unmet dependencies, retrying next iteration"
			}
		}
	}
	if { [llength $not_yet_build] > 0 } {
		append notbuildedlibs $not_yet_build
		print_msg "Libraries $not_yet_build could not be built, giving up"
	}
}	

proc create_release {} {
	global os
	global releasedir
	global builddir
	global env
	global libsuffix
	global build_dbcommands
	global cross	

	#create a release
	set env(ACPLT_HOME) $releasedir
	if { $cross != 1} {	
		if { $os == "nt" } then {
			set env(PATH) $releasedir/system/sysbin/\;$env(PATH)
		} else {
			set env(PATH) $releasedir/system/sysbin/:$env(PATH)
		}
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
	move_wildcard $releasedir/system/sysbin/*$libsuffix $releasedir/system/syslibs/
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
	file mkdir $releasedir/system/sysdevbase/ov/include/libov
	copy_wildcard $builddir/base/ov/include/libov/*.h $releasedir/system/sysdevbase/ov/include/libov
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
proc separate_dev {} {
	global system_libs
	global releasedir
	global builddir
	global addon_libs
	global libsuffix
	global exesuffix
	global flag
	global os
	
	set libs [glob -types d -tails -nocomplain -directory $releasedir/dev "*"]
	
	#move system libs
	#print_msg "$libs" 
	foreach x $libs {
		set temp [split $x "/"]
		set x [lindex $temp end]
		if { [file exists $releasedir/dev/$x] } then {
			file copy $releasedir/dev/$x $releasedir/system/sysdevbase/$x 
			file delete -force $releasedir/dev/$x 
		}
		if { [file exists $releasedir/system/addonlibs/${x}$libsuffix] } then {
			file copy $releasedir/system/addonlibs/${x}$libsuffix $releasedir/system/syslibs/${x}$libsuffix 
			file delete -force $releasedir/system/addonlibs/${x}$libsuffix
		}
	}
	if { [file exists $releasedir/system/sysbin/tmanager.exe] } then {
		file delete -force $releasedir/system/sysbin/tmanager.exe 
	}
	cd $releasedir
}

proc create_systools_and_servers {} {
	global basedir
	global releasedir
	global batsuffix
	global os
	
	file mkdir $releasedir/servers/
	file mkdir $releasedir/servers/MANAGER
	file mkdir $releasedir/servers/MANAGER/modelinstances
	file mkdir $releasedir/servers/MANAGER/logfiles 
	
	#including systools
	cd $releasedir/system
	file mkdir $releasedir/system/systools
	copy_wildcard $basedir/systools/*.tcl $releasedir/system/systools/
	copy_wildcard $basedir/systools/*.fbd $releasedir/system/systools/
	
	#including serverstarttools
	copy_wildcard $basedir/serverstarttools/*$batsuffix $releasedir/servers/MANAGER/
	file copy $basedir/serverstarttools/ov_server.conf $releasedir/servers/MANAGER/ov_server.conf 
}

#/*
#* Recursively removes all .svn dirs from the given folder
#*/
proc remove_svn_dirs {dir} {
	global os
	global basedir
	set current_dir [pwd]
	#remove .svn directories
	if { $os == "nt"} {
		#God, I hate spaces in windows dirnames
		file copy $basedir/delete_svn_folders.bat $dir
		cd $dir
		#file delete -force ".svn"
		execute_ignore "delete_svn_folders.bat"
		file delete -force "delete_svn_folders.bat"
		cd $current_dir
	} else {
		cd $dir
		file delete -force ".svn"
		set dirs [findDirectories "." ".svn"]
		foreach dir $dirs {
			file delete -force $dir
		}
		cd $current_dir
	}
}

proc compress {archivename dir} {
	global os
	global ov_arch_bitwidth_int
	print_msg "Compressing"
	if { $os == "linux" } then {
		execute "tar -zcvf $archivename-linux$ov_arch_bitwidth_int.tar.gz $dir"
	} else {
		execute "7z a $archivename-win$ov_arch_bitwidth_int.zip $dir"
	}
}

# ============== MAIN STARTS HERE ==================
set system_libs {syslibs/comm/ksbase syslibs/comm/TCPbind syslibs/comm/UDPbind syslibs/comm/ksxdr syslibs/comm/kshttp syslibs/comm/opcua syslibs/functionblock/fb syslibs/functionblock/ssc}
set addon_libs {addonlibs/hmi/cshmi addonlibs/commclient/ksapi addonlibs/commclient/fbcomlib addonlibs/commclient/opcuafb addonlibs/functionblock/iec61131stdfb addonlibs/functionblock/vdivde3696 addonlibs/functionblock/ACPLTlab003lindyn addonlibs/functionblock/IOdriverlib addonlibs/field/modbusTcpLib addonlibs/comm/MessageSys addonlibs/comm/kshttpMsgExt addonlibs/processcontrol/cmdlib addonlibs/processcontrol/PCMsgParser addonlibs/processcontrol/PCMsgCreator addonlibs/functionblock/SSChelper}
print_msg "checking out all libraries of the acplt system"

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
	
	#checking out a fresh copy
	create_dirs
	checkout_acplt

	file mkdir $builddir/syslibs
	cd $builddir/syslibs
	foreach libname $system_libs {
		checkout_dir $libname
	}

	file mkdir $builddir/addonlibs
	cd $builddir/addonlibs
	foreach libname $addon_libs {
		checkout_dir $libname
	}

	cd $basedir
	#rename 
	file rename -force acplt.build acplt-source
	print_msg "Deleting .svn directories for acplt-server-source archive"
	remove_svn_dirs "$basedir/acplt-source"

	compress "acplt-server-source-$date" "./acplt-source"

	file delete -force "acplt-source"
	
	#rename back to save compiling overhead
	#file delete -force "acplt.build"
	#if { [file exists acplt.build_backup] } {
	#	file rename acplt.build_backup acplt.build
	#}
	file delete -force "acplt-build"
	print_msg "== SOURCE RELEASE CREATED =="
# end
}

create_dirs
if { $compileonly != 1 } then {
	checkout_acplt
} else {
	set date [get_revision]
}
cd $basedir

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
	if {$targetOS == "nt"} {
		install_acplt cygwin
	} else {
		install_acplt linux
	}
}

if {$release == 1} {
# Create develop release
	print_msg "== CREATING DEVELOP RELEASE =="
}

create_release
create_systools_and_servers

foreach libname $system_libs {
	release_lib_better $libname debug
}
#move system_libs to system
separate_dev
foreach libname $addon_libs {
	release_lib_better $libname debug
}

if {$release == 1} {
	cd $basedir
	compress "acplt-server-develop-$date" "./acplt"
	print_msg "== DEVELOP RELEASE CREATED =="
# end
}

if {$release == 1} {
# Create runtime release
	print_msg "== CREATING RUNTIME RELEASE =="
	set ov_debug ""
	file delete -force $builddir
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
		if {$targetOS == "nt"} {
		install_acplt cygwin
		} else {
			install_acplt linux
		}
	}

	create_release
	create_systools_and_servers
	foreach libname $system_libs {
		release_lib_better $libname all
	}
	separate_dev
	foreach libname $addon_libs {
		release_lib_better $libname all
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
		file copy $releasedir/system/sysbin/tclsh.exe $releasedir/system/tclsh.exe
		file delete -force $releasedir/system/sysbin/tclsh.exe
	}

	set stripfiles [glob -nocomplain $releasedir/system/sysbin/*.*]

	foreach x $stripfiles {
		# execute "strip --strip-debug" $x
		execute "strip" "-g" "-S" "-d" $x
	}
	
	#restore tclsh.exe so it is not stripped
	if { [file exists $releasedir/system/tclsh.exe] } then {
		file copy $releasedir/system/tclsh.exe $releasedir/system/sysbin/tclsh.exe 
		file delete -force $releasedir/system/tclsh.exe
	}

	compress "acplt-server-runtime-$date" "./acplt"
	print_msg "== RUNTIME RELEASE CREATED =="
# end
}

puts "Build finished"
