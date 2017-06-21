/*
*   $Id: mkimpdef.lex,v 1.2 1999-07-30 15:37:06 dirk Exp $
*
*   Copyright (C) 1998
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package 
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/
/*
*	History:
*	--------
*	01-Jul-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

%%
.*Debugger.*Data.*\n	{}
"    _"[a-zA-Z0-9_]*	{ fprintf(yyout, "_%s=%s", yytext+5, yytext+5); }
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

