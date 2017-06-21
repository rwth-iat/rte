#! /usr/bin/perl

#   $Id: depend_nt.pl,v 1.1 1999-07-19 15:01:52 dirk Exp $
#
#   Copyright (C) 1998
#   Lehrstuhl fuer Prozessleittechnik,
#   D-52056 Aachen, Germany.
#   All rights reserved.
#
#   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
#
#   This file is part of the ACPLT/OV Package 
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
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
