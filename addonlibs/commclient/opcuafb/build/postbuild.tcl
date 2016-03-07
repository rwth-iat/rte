puts "== Begin moving FBDs to template folder =="

set fbdfiles {"demo.fbd"}

set targetfolder "$::env(ACPLT_HOME)/templates/opcuafb"
file mkdir $targetfolder
if {$targetfolder != "" && [file isdirectory $targetfolder]} {
	foreach fbdfilefullname $fbdfiles {
		#we are in build/nt or build/linux
		set fbdfilefullname "../../$fbdfilefullname"
		set fbdfilename [file tail $fbdfilefullname]
		set targetname "$targetfolder/$fbdfilename"
		puts -nonewline "Copying file $fbdfilefullname to $targetfolder..."
		if {[file exists $fbdfilefullname] == 0} {
			puts "not available. Skipped!"
			continue
		}
		if {[file exists $targetname] == 1} {
			puts -nonewline "deleting old..."
			file delete $targetname
		}
		file copy $fbdfilefullname $targetname
		puts "done."
	}
} else {
	puts "Targetfolder $targetfolder not found. Aborting."
}
puts "== End processing templates =="
