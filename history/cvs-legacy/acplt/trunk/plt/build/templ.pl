# Template linker

if ("@ARGV" =~ m/\s(\w+)\.o\s/) {
    $objpref = $1;
    $objname = "$1.o";
    $cppname = "$1_inst.h";
} else {
    die('Cant find object file');
}

$make = $ENV{"MAKE"};
if ($make eq "") {
   $make = "make"
}

$compile = "$make $objname";
$link = "@ARGV";

print STDERR "Template linker (c) 1997 ACPLT\n";

#
# Read include file
#

%old = ( );
if (open (IN, "<$cppname")) { 
    while (<IN>) {
	if (/template class (\w+<w+.*>);/) {
	    @old{$1} = 1;
	}
    }
} else {
    open (IN, ">>$cppname") || die("cant read nor touch $cppname");
}
close (IN);

@olds = sort(keys(%old));
%new = %old;    
#
# iterate until finished or no change
#

$iter = 0;
do {
    $needanother = 0;
    ++ $iter;
    print STDERR  "Starting run $iter...\n";
    print STDERR "$link\n";
    open (MSGS, "$link 2>&1 |") || die('cant start linker');
    while (<MSGS>) {
        print STDERR $_;
	if (!/In function.*:$/) {
          if (/(\w+<\s*\w+.*>)::~?\w+.*\(.*\)/
           || /(\w+<\s*\w+.*>).*\s+virtual table/) {
	    # member
	    @new{$1} = 1;
	    #print STDERR ".";
	    $needanother = 1;
            }
	} else {
#  	    print STDERR $_ unless (/In function.*:$/);
        }
    } 
    print STDERR "\n";
    @news = sort(keys(%new));

    if ($needanother) {
	if (@news eq @olds) {
#           print "NEWLIST @news OLDLIST @olds";
	    die("Last iteration resolved no new symbols");	
	}
	open (OUT, ">$cppname") || die('cant open output');
	foreach $templ (@news) {
	    printf OUT "template class %s;\n", $templ;
	}
	sleep 2;
	close OUT;
#	printf STDERR "%d new templates found, total %d\n", $#news-$#olds, $#news;
	@olds = @news;
	print STDERR "$compile\n";
	$err = system($compile);
	if ($err) {
            print STDERR "Could not remake main object.\n";
	    exit($err);
	}
    }
} while($needanother);	
print STDERR "Finished: All templates seem to be instantiated.\n";
exit 0;


