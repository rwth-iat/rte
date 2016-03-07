puts "== Begin processing static files ==" 
cd "../../staticfiles"
set contentname "content"
set mimetypename "mimetype"
set encodingname "encoding"
set webserverlib "kshttp"
set staticfileclassname "staticfile"

proc stripLineComments {inputString {commentChars ";#"}} {
	# Switch the RE engine into line-respecting mode instead of the default whole-string mode
	regsub -all -line "(\s+)\/\/.*$" $inputString "" commentStripped
	# Now strip the whitespace
	regsub -all -line {^[ \t\r]*(.*\S)?[ \t\r]*$} $commentStripped {\1}
}
proc processDir {dirname} {
	global out
	global contentname mimetypename encodingname webserverlib staticfileclassname
	
	set filelist [glob -nocomplain -types {f} *]
	foreach filename $filelist {
		puts -nonewline "Including file: $filename..."
		#reading file
		set in [open $filename r]
		#mimetype business
		set extension [file extension $filename]
		
		#default value
		set mimetype "text/html"
		set encodingvalue "windows-1252"
		if { $extension == ".tcl" } {
			puts "skipping myself"
			continue
		} elseif { $filename == ".project" || $filename == ".settings" || $filename == ".svn"} {
			puts "skipping generated file"
			continue
		} elseif { $extension == ".fbd" } {
			puts "skipping generated file"
			continue
		} elseif { $extension == ".exe" } {
			puts "skipping exe binary file"
			continue
		} elseif { $extension == ".js" } {
			set mimetype "text/javascript"
		} elseif { $extension == ".css" } {
			set mimetype "text/css"
		} elseif { $extension == ".xhtml" } {
			set mimetype "application/xhtml+xml"
		} elseif { $extension == ".svg" } {
			set mimetype "image/svg+xml"
		} elseif { $extension == ".jpg" || $extension == ".jpeg" || $extension == ".png" || $extension == ".ico" || $extension == ".gif" || $extension == ".log" || $extension == ".php"} {
			#ignore binary formats
			puts "skipping binary or unwanted file"
			continue
		}
		
		puts $out "	pindexhtml = Ov_SearchChildEx(ov_containment, pdom, \"$filename\", ${webserverlib}_${staticfileclassname});"
		puts $out "	if(!pindexhtml)"
		puts $out "	\{"
		puts $out "		result = Ov_CreateObject(${webserverlib}_${staticfileclassname}, pindexhtml, pdom, \"$filename\");"
		puts $out "		if(Ov_Fail(result))"
		puts $out "		\{"
		puts $out "			ov_logfile_error(\"Fatal: Could not create Object '$filename': %s\", ov_result_getresulttext(result));"
		puts $out "			return result;"
		puts $out "		\}"
		puts $out "		ov_string_setvalue(&(pindexhtml->v_content), \"\\"
		
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
		puts $out "		ov_string_setvalue(&(pindexhtml->v_${mimetypename}), \"$mimetype\");";
		puts $out "		ov_string_setvalue(&(pindexhtml->v_${encodingname}), \"$encodingvalue\");";
		puts $out "	\}"
		puts "done"
	}
	
	#Search for child directories
	set dirlist [glob -nocomplain -types {d} *]
	foreach dirname $dirlist {
		if { $dirname == "tclsh.exe"} {
			puts "skipping false positive directory: $dirname"
			continue
		} elseif { $dirname == ".settings" || $dirname == ".svn" || $dirname == "bugList" || $dirname == "exportKSX" || $dirname == "requirementTests" || $dirname == "bak" || $dirname == ".externalToolBuilders"} {
			puts "skipping directory: $dirname"
			continue
		}
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
		
		processDir {percentdirname}
		
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
puts $out "#ifndef OV_COMPILE_LIBRARY_kshttpMsgExt"
puts $out "#define OV_COMPILE_LIBRARY_kshttpMsgExt"
puts $out "#endif"
puts $out ""
puts $out "#include \"config.h\""
puts $out ""
puts $out "OV_RESULT kshttpMsgExt_include_localfiles(OV_INSTPTR_ov_domain pstaticfiles){";
puts $out "	OV_RESULT result = OV_ERR_OK;"
puts $out "	OV_INSTPTR_${webserverlib}_${staticfileclassname}	pindexhtml = NULL;"
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
