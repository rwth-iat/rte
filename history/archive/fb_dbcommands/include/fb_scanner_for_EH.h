/*
   fb_scanner_for_EH.h : MSVC ist Mist!
*/

#ifndef _FB_SCANNER_FOR_EH_H_
#define _FB_SCANNER_FOR_EH_H_

typedef unsigned int   yy_size_t;

struct yy_buffer_state
	{
	FILE *yy_input_file;
	char *yy_ch_buf;		 
	char *yy_buf_pos;		 
	yy_size_t yy_buf_size;
	int yy_n_chars;
	int yy_is_our_buffer;
	int yy_is_interactive;
	int yy_at_bol;
	int yy_fill_buffer;
	int yy_buffer_status;
	};


typedef struct yy_buffer_state  *YY_BUFFER_STATE;


int yylex(void);
int yyerror(char *s);
int yyparse(void);


void yyrestart           ( FILE *input_file )  ;
void yy_switch_to_buffer ( YY_BUFFER_STATE new_buffer )  ;
void yy_load_buffer_state( void )  ;
void yy_init_buffer      ( YY_BUFFER_STATE b, FILE *file )  ;
void yy_flush_buffer     ( YY_BUFFER_STATE b )  ;

void yy_delete_buffer    ( YY_BUFFER_STATE b )  ;
YY_BUFFER_STATE yy_create_buffer  ( FILE *file, int size )  ;
YY_BUFFER_STATE yy_scan_buffer  ( char *base, yy_size_t size )  ;
YY_BUFFER_STATE yy_scan_string  ( const  char *yy_str )  ;
YY_BUFFER_STATE yy_scan_bytes  ( const  char *bytes, int len )  ;



#endif

