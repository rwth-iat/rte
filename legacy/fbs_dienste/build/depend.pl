#! /usr/bin/perl

while (<>) {
    s:/:\\:g;
    if ($_ =~ /^(.+).o:(.*$)/) {
	$_ = "$1.obj:$2\n";
    } 
    if ( $_ =~ /^(.*\\$)/ ) {
	print "$1\n";
    } else {
	print "$_\n";
    }
}
