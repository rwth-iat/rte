puts "== Begin processing static files ==" 
cd "../../staticfiles"

proc stripLineComments {inputString {commentChars ";#"}} {
	# Switch the RE engine into line-respecting mode instead of the default whole-string mode
	regsub -all -line "(\s+)\/\/.*$" $inputString "" commentStripped
	# Now strip the whitespace
	regsub -all -line {^[ \t\r]*(.*\S)?[ \t\r]*$} $commentStripped {\1}
}
proc processDir {dirname} {
	global out
	set filelist [glob -nocomplain -types {f} *]
	foreach filename $filelist {
		puts -nonewline "Including file: $filename..."
		puts $out "	pindexhtml = Ov_SearchChildEx(ov_containment, pdom, \"$filename\", kshttpd_staticfile);"
		puts $out "	if(!pindexhtml)"
		puts $out "	\{"
		puts $out "		result = Ov_CreateObject(kshttpd_staticfile, pindexhtml, pdom, \"$filename\");"
		puts $out "		if(Ov_Fail(result))"
		puts $out "		\{"
		puts $out "			ov_logfile_error(\"Fatal: Could not create Object '$filename': %s\", ov_result_getresulttext(result));"
		puts $out "			return result;"
		puts $out "		\}"
		puts $out "		ov_string_setvalue(&(pindexhtml->v_content), \"\\"
		#reading file
		set in  [open $filename r]
		#mimetype business
		set extension [file extension $filename]
		
		if { $extension == ".jpg" || $extension == ".jpeg" || $extension == ".png" || $extension == ".ico"} {
			#ignore binary formats
			continue
		}
		
		#default value
		set mimetype "text/html"
		if { $extension == ".js" } {
			set mimetype "text/javascript"
		} elseif { $extension == ".css" } {
			set mimetype "text/css"
		}
		# line-by-line, read the original file
		while {[gets $in line] != -1} {
			#transform $line somehow, order of manipulation matters
			
			#recode backslashes
			set line [string map {"\\" "\\\\"} $line]
			#recode quotes
			set line [string map {"\"" "\\\""} $line]
			if { $extension == ".js" || $extension == ".html" || $extension == ".xhtml"} {
				#strip line comments and trim white spaces at line begin and end
#this is buggy, so deactivate
#				set line [stripLineComments $line]
			}
			#recode lineending
			set line "$line\\n\\"
			# then write the transformed line
			puts $out $line
		}
		close $in
		puts $out "\");"
		puts $out "		ov_string_setvalue(&(pindexhtml->v_mimetype), \"$mimetype\");";
		puts $out "	\}"
		puts "done"
	}
	
	set dirlist [glob -nocomplain -types {d} *]
	foreach dirname $dirlist {
		cd $dirname;
		puts "Processing directory: $dirname"
		puts $out "	"
		puts $out "	/*************************************** directory $dirname */"
		puts $out "	pnewdom = Ov_SearchChildEx(ov_containment, pdom, \"$dirname\", ov_domain);"
		puts $out "	if(!pnewdom)"
		puts $out "	\{"
		puts $out "		result = Ov_CreateObject(ov_domain, pnewdom, pdom, \"$dirname\");"
		puts $out "		if(Ov_Fail(result))"
		puts $out "		\{"
		puts $out "			ov_logfile_error(\"Fatal: Could not create Domain '$dirname': %s\", ov_result_getresulttext(result));"
		puts $out "			return result;"
		puts $out "		\}"
		puts $out "	\}"
		#move domain pointer into new dom
		puts $out "	pdom = pnewdom;"
		
		processDir {dirname}
		
		#move domain pointer back
		puts $out "	pdom = Ov_GetParent(ov_containment, pdom);"
		cd ".."
	}
}

puts -nonewline "Building c header..."
set out [open "../source/generated_static_index.c" w]
puts $out "//this is an included index of static files"
puts -nonewline $out "//generated: "
puts $out [clock format [clock seconds] -format {%Y-%m-%d %H:%M:%S}]
puts $out "//do not edit or check in this file since it will be overwritten"
puts $out "#ifndef OV_COMPILE_LIBRARY_kshttpd"
puts $out "#define OV_COMPILE_LIBRARY_kshttpd"
puts $out "#endif"
puts $out ""
puts $out "#include \"config.h\""
puts $out ""
puts $out "OV_RESULT include_localfiles(OV_INSTPTR_ov_domain pstaticfiles){";
puts $out "	OV_RESULT result = OV_ERR_OK;"
puts $out "	OV_INSTPTR_kshttpd_staticfile	pindexhtml = NULL;"
puts $out "	OV_INSTPTR_ov_domain	pdom = pstaticfiles;"
puts $out "	OV_INSTPTR_ov_domain	pnewdom = NULL;"
puts $out "	"
puts "done"

processDir {.}

#make gcc happy and use this variable at least once :)
puts $out "	pdom = pnewdom;"
puts $out "	"
puts $out "	return result;"
puts $out "\};"

close $out

cd ".."
puts "== End processing static files =="