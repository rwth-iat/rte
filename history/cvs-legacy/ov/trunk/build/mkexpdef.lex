/*
*   $Id: mkexpdef.lex,v 1.1 1999-07-19 15:01:52 dirk Exp $
*
*   Copyright (C) 1998-1999
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package which is licensed as open
*   source under the Artistic License; you can use, redistribute and/or
*   modify it under the terms of that license.
*
*   You should have received a copy of the Artistic License along with
*   this Package; see the file ARTISTIC-LICENSE. If not, write to the
*   Copyright Holder.
*
*   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
*   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
/*
*	History:
*	--------
*	01-Jul-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

%%
.*Debugger.*Data.*\n	{}
"    _"[a-zA-Z_]*		{ fprintf(yyout, "%s=_%s", yytext+5, yytext+5); }
@.*\n					{ fprintf(yyout, "\n"); }
[ \t]*\n				{ fprintf(yyout, "\n"); }
%%

/*
*	Main program -- take a *.def file as input and a new *.def file as output
*/
int main(int argc, char **argv) {
	int exit_status;
	if(argc !=3) {
		return 1;
	}
	yyin = fopen(argv[1], "r");
	if(!yyin) {
		return 1;
	}
	yyout = fopen(argv[2], "w");
	if(!yyout) {
		return 1;
	}
	exit_status = yylex();
	fclose(yyin);
	fclose(yyout);
	return exit_status;
}

/*
*	replacement for libfl
*/
int yywrap(void) {
	return 1;
}

