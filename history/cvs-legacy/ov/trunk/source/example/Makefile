all: ov_expression_parser.c ov_expression_parser.h ov_expression_scanner.c
clean:
	rm -f ov_expression_parser.c  ov_expression_parser.h ov_expression_scanner.c

.SUFFIXES:
.SUFFIXES: .y .lex .h .c

ov_expression_parser.c ov_expression_parser.h: ov_expression_parser.y
	bison -t -d -pov_expression_ -o$@ $<

ov_expression_scanner.c: ov_expression_scanner.lex
	flex -Pov_expression_ -o$@ $<


