puts "== Begin processing js files ==" 
#cd "../../staticfiles"

set baseovpath "/communication/httpservers/httpserver/staticfiles/hmi"
set contentname "content"
set mimetypename "mimetype"
set encodingname "encoding"
set staticfileclass "/acplt/ksservhttp/staticfile"

proc stripLineComments {inputString {commentChars ";#"}} {
	# Switch the RE engine into line-respecting mode instead of the default whole-string mode
	regsub -all -line "(\s+)\/\/.*$" $inputString "" commentStripped
	# Now strip the whitespace
	regsub -all -line {^[ \t\r]*(.*\S)?[ \t\r]*$} $commentStripped {\1}
}

proc printDomain {ovpath} {
	global out
	
	puts $out " INSTANCE  $ovpath :"
	puts $out "    CLASS /acplt/ov/domain;"
	puts $out " END_INSTANCE;"
	puts $out ""
}

proc processDir {ovpath} {
	global out
	global contentname mimetypename encodingname staticfileclass
	
	set filelist [glob -nocomplain -types {f} *]
	foreach filename $filelist {
		puts -nonewline "Including file: $filename..."
		#reading file
		set in [open $filename r]
		#mimetype business
		set extension [file extension $filename]
		
		#default value
		set mimetype "text/html"
		if { $filename == "generateFBD.tcl" } {
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
		} elseif { $extension == ".svg" } {
			set mimetype "image/svg+xml"
		} elseif { $extension == ".jpg" || $extension == ".jpeg" || $extension == ".png" || $extension == ".ico" || $extension == ".gif" || $extension == ".log" || $extension == ".php"} {
			#ignore binary formats
			puts "skipping binary or unwanted file"
			continue
		}
		
		set percentfilename [string map {"." "%2E"} $filename]
		set percentfilename [string map {"-" "%2D"} $percentfilename]
		puts $out " INSTANCE  $ovpath/$percentfilename :"
		puts $out "    CLASS $staticfileclass;"
		puts $out "    VARIABLE_VALUES"
		puts -nonewline $out "        $contentname : INPUT  STRING = \""
		
		# line-by-line, read the original file
		while {[gets $in line] != -1} {
			#transform $line somehow, order of manipulation matters
			
#			#recode backslashes
#			set line [string map {"\\" "\\\\"} $line]
			#recode quotes
			set line [string map {"\"" "\\\""} $line]
			if { $extension == ".js" || $extension == ".html" || $extension == ".xhtml"} {
				#strip line comments and trim white spaces at line begin and end
#this is buggy, so deactivate
#				set line [stripLineComments $line]
			}
			#recode lineending
#			set line "$line\\n\\"
			# then write the transformed line
			puts $out $line
		}
		close $in
		puts $out "\";"
		puts $out "        $mimetypename : INPUT  STRING = \"$mimetype\";"
		puts $out "        $encodingname : INPUT  STRING = \"\";"
		puts $out "    END_VARIABLE_VALUES;"
		puts $out " END_INSTANCE;"
		puts $out ""
		puts "done"
	}
	
	#Search for child directories
	set dirlist [glob -nocomplain -types {d} *]
	foreach dirname $dirlist {
		if { $dirname == "tclsh.exe"} {
			puts "skipping false positive directory: $dirname"
			continue
		} elseif { $dirname == ".settings" || $dirname == ".svn" || $dirname == "bugList" || $dirname == "exportKSX" || $dirname == "requirementTests"} {
			puts "skipping directory: $dirname"
			continue
		}
		cd $dirname;
		set percentdirname [string map {"." "%2E"} $dirname]
		set percentdirname [string map {"-" "%2D"} $percentdirname]
		set newovpath "$ovpath/$percentdirname"
		
		puts "Processing directory: $dirname"
		
		printDomain $newovpath
		processDir $newovpath
		
		cd ".."
	}
}

puts -nonewline "Building FBD header..."
set out [open "hmiJavascript.fbd" w]
puts $out "/******************************"
puts $out "this is an index of static files"
puts -nonewline $out "generated: "
puts $out [clock format [clock seconds] -format {%Y-%m-%d %H:%M:%S}]
puts $out "do not edit or check in this file"
puts $out "******************************/"
puts $out ""
puts "done"

printDomain $baseovpath

processDir $baseovpath


close $out

puts "== End processing files =="
