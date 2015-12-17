#! /usr/bin/perl

#/******************************************************************************
#***                                                                         ***
#***   G O V   -   G e n e r i s c h e   O b j e k t - V e r w a l t u n g   ***
#***   ###################################################################   ***
#***                                                                         ***
#***   Implementierung des Objektmodells ACPLT/OV "Objektverwaltung"         ***
#***   des Lehrstuhls fuer Prozessleittechnik, RWTH Aachen                   ***
#***                                                                         ***
#******************************************************************************/
#/*
#*   Datei
#*   -----
#*   depend_nt.pl - Dependencies fuer Windows NT erzeugen
#*/
#/*
#*   Historie
#*   --------
#*   27-Nov-1998 Dirk Meyer: Erstellung, Lehrstuhl fuer Prozessleittechnik, Aachen
#*/
#/*****************************************************************************/

while (<>) {
	if ($_ =~ /^(.+).o:(.*$)/) {
		$_ = "$1.obj:$2\n";
	} 
	if ( $_ =~ /^(.*\\$)/ ) {
		print "$1\n";
	} else {
		print "$_\n";
	}
}
