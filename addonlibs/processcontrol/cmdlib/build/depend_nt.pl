#! /usr/bin/perl

#   $Id: depend_nt.pl,v 1.1.1.1 2002-08-16 13:55:23 michaelh Exp $
#
#   Copyright (C) 1998-1999
#   Lehrstuhl fuer Prozessleittechnik,
#   RWTH Aachen, D-52056 Aachen, Germany.
#   All rights reserved.
#
#   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
#
#   This file is part of the ACPLT/OV Package which is licensed as open
#   source under the Artistic License; you can use, redistribute and/or
#   modify it under the terms of that license.
#
#   You should have received a copy of the Artistic License along with
#   this Package; see the file ARTISTIC-LICENSE. If not, write to the
#   Copyright Holder.
#
#   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
#   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
#   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#	History:
#	--------
#	27-Nov-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
#	16-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.



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
