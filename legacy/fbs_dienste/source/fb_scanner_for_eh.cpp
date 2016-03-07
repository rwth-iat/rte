#include <stdio.h> 
#include <stdlib.h> 

#include "ks/ks.h"
#include "par_param.h" 

typedef union {
    KS_VAR_TYPE                 datatype;
    char*                                 string;
    VariableItem*                var_item;
    Variables*                        variable;
    Dienst_param*                par;
    InstanceItems*                instans;
    Child*                                ch_path;
    LinksItems*                        link;
    DelInstItems*                delinstans;
    SetInstVarItems*        setinstvar;
    PortType            pt;
} YYSTYPE;


extern YYSTYPE yylval;

#include "fb_scanner_for_EH.h"


typedef unsigned char  YY_CHAR;
typedef int yy_state_type;

static  YY_BUFFER_STATE      yy_current_buffer = 0;
static  char                 yy_hold_char;
static  int                  yy_n_chars;                 
static  char                 *yy_c_buf_p = (char *) 0;
static  int                  yy_init = 1;                 
static  int                  yy_start = 0;         
static  int                  yy_did_buffer_switch_on_eof;
static  yy_state_type        yy_last_accepting_state;
static  char                 *yy_last_accepting_cpos;


char *yytext;
int yyleng;

__declspec( dllexport) FILE   *yyin = (FILE *) 0;
__declspec( dllexport) FILE   *yyout = (FILE *) 0;

__declspec( dllexport) int    current_line;

static PARSER_STACK *pStrStack = 0;



static void *yy_flex_alloc  ( yy_size_t )  ;
static void *yy_flex_realloc  ( void *, yy_size_t )  ;
static void yy_flex_free  ( void * )  ;
static void yyunput  ( int c, char *buf_ptr )  ;
static int  input  ( void )  ;


static yy_state_type yy_get_previous_state  ( void )  ;
static yy_state_type yy_try_NUL_trans  ( yy_state_type current_state )  ;
static int yy_get_next_buffer  ( void )  ;
static void yy_fatal_error  ( const  char msg[] )  ;



static const short int yy_accept[270] =
{   0,
46,   46,    0,    0,   50,   48,   46,   47,   48,   48,
48,   48,   44,   40,   40,   40,   40,   40,   40,   40,
40,   40,   40,   40,   40,   40,   40,   40,    2,    3,
2,   46,    0,   45,    0,    0,   44,   43,    1,   41,
44,   40,   40,   40,   40,   40,   40,   40,   40,   40,
40,   40,   40,   40,   40,   40,   40,   40,   40,   40,
40,   40,   40,    4,   45,    0,    0,   41,   44,   40,
40,   40,   40,   40,   40,   40,   40,   40,   40,    6,
40,   40,   40,   40,   40,   27,   40,   40,   40,   40,
40,   40,   40,   40,    0,   43,   44,    5,   40,   40,

40,   40,   40,   40,   40,   40,   40,   40,   22,   40,
40,   40,   40,   40,   40,   40,   10,   38,    7,   40,
40,    0,   40,   18,   40,   40,   40,   40,   40,   40,
40,   40,   39,   40,   34,   40,   40,   40,   40,   40,
40,   40,   37,   40,   40,   40,   40,    0,   40,   29,
9,   40,   40,   40,   40,   40,   40,   36,   40,   40,
40,   35,   40,   24,    8,   12,   40,   31,   40,    0,
40,   40,   40,   40,   40,   40,   28,   40,   40,   40,
13,   40,   40,   40,   40,    0,   25,   40,   40,   40,
40,   26,   40,   40,   17,   40,   40,   40,   40,    0,

40,   40,   40,   40,   40,   40,   40,   33,   11,   40,
42,   40,   30,   40,   40,   32,   40,   40,   40,    0,
40,   40,   40,   14,   40,   40,   40,    0,   40,   40,
21,   40,   40,   40,    0,   40,   40,   40,   40,   40,
0,   15,   40,   40,   23,   40,    0,   40,   40,   19,
0,   40,   40,    0,   40,   40,    0,   16,   40,   42,
20,    0,    0,    0,    0,    0,    0,   42,    0
} ;

static const int yy_ec[256] =
{   0,
1,    1,    1,    1,    1,    1,    1,    1,    2,    3,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    2,    1,    4,    1,    1,    1,    1,    1,    1,
1,    5,    6,    1,    7,    8,    9,   10,   10,   10,
10,   10,   10,   10,   10,   10,   10,   11,    1,    1,
1,    1,    1,    1,   12,   13,   14,   15,   16,   17,
18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
21,   28,   29,   30,   31,   32,   21,   21,   33,   21,
1,   34,    1,    1,   35,    1,   21,   21,   21,   21,

36,   21,   21,   21,   21,   21,   21,   21,   21,   21,
21,   21,   21,   21,   21,   21,   21,   21,   21,   21,
21,   21,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,

1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1
} ;

static const int yy_meta[37] =
{   0,
1,    1,    1,    1,    2,    1,    1,    1,    3,    4,
1,    5,    5,    5,    5,    5,    5,    5,    5,    5,
5,    5,    5,    5,    5,    5,    5,    5,    5,    5,
5,    5,    5,    1,    5,    5
} ;

static const short int yy_base[275] =
{   0,
0,    0,   34,   35,  313,  314,  310,  314,   37,   34,
301,  305,   35,    0,  283,   27,   31,  283,  295,  286,
280,  284,   31,  291,   33,   31,   35,  290,  314,  314,
292,  298,   48,  314,   50,  289,   48,   51,  314,  289,
56,    0,  271,  276,  283,  271,  262,  277,  268,  275,
43,   52,  254,  258,  259,  256,  260,   57,  260,  252,
257,  258,  252,  314,   64,   68,  274,  269,   71,  254,
253,  246,  258,  260,  237,  242,  255,  238,  238,    0,
239,  244,  253,  237,   64,    0,  245,  232,  241,  244,
243,  228,  237,  236,  245,  244,   81,    0,  238,  223,

221,  227,   84,  233,  232,  217,  234,  233,    0,  215,
212,  218,  216,  217,  223,  213,  202,    0,    0,  211,
223,  224,  205,    0,  216,  215,  214,  204,  208,  211,
201,  213,    0,  199,    0,  198,  194,  192,  190,  203,
188,  201,    0,  198,  186,  192,  200,  202,  195,  175,
0,  186,  179,   70,  177,  183,  177,    0,  190,  170,
176,    0,  171,    0,    0,    0,  173,    0,  176,  191,
172,  173,  179,  164,  165,  170,    0,  171,  170,  173,
0,  174,  171,  174,  169,  174,    0,  163,  152,  169,
168,    0,  154,  166,    0,  157,  148,  150,  139,  163,

159,  155,  145,  141,  146,  154,  154,    0,    0,  133,
162,  135,  127,  147,  127,    0,  136,  128,  145,   84,
144,  132,  138,    0,  137,  132,  128,  140,  121,  128,
0,  112,  120,  114,  133,  110,  129,  109,  115,  123,
128,    0,  109,  124,    0,  100,  118,  102,   89,    0,
100,   82,   78,   98,   73,   89,   93,    0,   73,   93,
0,   90,   87,   86,   83,   82,   80,  314,  314,  116,
121,  125,  127,  130
} ;

static const short int yy_def[275] =
{   0,
269,    1,  270,  270,  269,  269,  269,  269,  271,  269,
269,  272,  269,  273,  273,  273,  273,  273,  273,  273,
273,  273,  273,  273,  273,  273,  273,  273,  269,  269,
269,  269,  271,  269,  271,  269,  269,  269,  269,  274,
269,  273,  273,  273,  273,  273,  273,  273,  273,  273,
273,  273,  273,  273,  273,  273,  273,  273,  273,  273,
273,  273,  273,  269,  271,  269,  272,  274,  269,  273,
273,  273,  273,  273,  273,  273,  273,  273,  273,  273,
273,  273,  273,  273,  273,  273,  273,  273,  273,  273,
273,  273,  273,  273,  269,  269,  269,  273,  273,  273,

273,  273,  273,  273,  273,  273,  273,  273,  273,  273,
273,  273,  273,  273,  273,  273,  273,  273,  273,  273,
273,  269,  273,  273,  273,  273,  273,  273,  273,  273,
273,  273,  273,  273,  273,  273,  273,  273,  273,  273,
273,  273,  273,  273,  273,  273,  273,  269,  273,  273,
273,  273,  273,  273,  273,  273,  273,  273,  273,  273,
273,  273,  273,  273,  273,  273,  273,  273,  273,  269,
273,  273,  273,  273,  273,  273,  273,  273,  273,  273,
273,  273,  273,  273,  273,  269,  273,  273,  273,  273,
273,  273,  273,  273,  273,  273,  273,  273,  273,  269,

273,  273,  273,  273,  273,  273,  273,  273,  273,  273,
269,  273,  273,  273,  273,  273,  273,  273,  273,  269,
273,  273,  273,  273,  273,  273,  273,  269,  273,  273,
273,  273,  273,  273,  269,  273,  273,  273,  273,  273,
269,  273,  273,  273,  273,  273,  269,  273,  273,  273,
269,  273,  273,  269,  273,  273,  269,  273,  273,  269,
273,  269,  269,  269,  269,  269,  269,  269,    0,  269,
269,  269,  269,  269
} ;

static const short int yy_nxt[351] =
{   0,
6,    7,    8,    9,    6,   10,   10,   11,   12,   13,
6,   14,   15,   16,   17,   18,   19,   14,   20,   21,
14,   14,   22,   14,   14,   23,   24,   14,   25,   26,
27,   28,   14,    6,   14,   14,   30,   30,   31,   31,
34,   36,   36,   37,   41,   44,   46,   53,   56,   45,
59,   34,   57,   65,   61,   36,   47,   37,   60,   62,
38,   54,   58,   36,   81,   69,   66,   34,   88,   78,
35,   79,   80,   95,   95,  112,   82,   96,   36,  113,
97,   35,  175,   35,   89,  220,   66,  122,   36,  268,
37,  267,  266,  228,  176,  265,  264,   35,  127,  263,

262,  261,  260,  128,  259,  258,  129,  257,  256,  255,
254,  253,  130,  252,  131,  132,   29,   29,   29,   29,
29,   33,   33,   33,   33,   33,   40,  251,  250,   40,
42,   42,   68,   68,   68,  249,  248,  247,  246,  245,
244,  243,  242,  241,  240,  239,  238,  237,  236,  235,
234,  233,  232,  231,  230,  229,  227,  226,  225,  224,
223,  222,  221,  220,  219,  218,  217,  216,  215,  214,
213,  212,  211,  210,  209,  208,  207,  206,  205,  204,
203,  202,  201,  200,  199,  198,  197,  196,  195,  194,
193,  192,  191,  190,  189,  188,  187,  186,  185,  184,

183,  182,  181,  180,  179,  178,  177,  174,  173,  172,
171,  170,  169,  168,  167,  166,  165,  164,  163,  162,
161,  160,  159,  158,  157,  156,  155,  154,  153,  152,
151,  150,  149,  148,  147,  146,  145,  144,  143,  142,
141,  140,  139,  138,  137,  136,  135,  134,  133,  126,
125,  124,  123,   96,   96,  121,  120,  119,  118,  117,
116,  115,  114,  111,  110,  109,  108,  107,  106,  105,
104,  103,  102,  101,  100,   99,   98,   67,  269,   94,
93,   92,   91,   90,   87,   86,   85,   84,   83,   77,
76,   75,   74,   73,   72,   71,   70,   67,   38,   32,

64,   63,   55,   52,   51,   50,   49,   48,   43,   39,
38,   32,  269,    5,  269,  269,  269,  269,  269,  269,
269,  269,  269,  269,  269,  269,  269,  269,  269,  269,
269,  269,  269,  269,  269,  269,  269,  269,  269,  269,
269,  269,  269,  269,  269,  269,  269,  269,  269,  269
} ;

static const short int yy_chk[351] =
{   0,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    3,    4,    3,    4,
9,   10,   13,   10,   13,   16,   17,   23,   25,   16,
26,   33,   25,   35,   27,   37,   17,   37,   26,   27,
38,   23,   25,   41,   52,   41,   38,   65,   58,   51,
9,   51,   51,   66,   66,   85,   52,   66,   69,   85,
69,   33,  154,   35,   58,  220,   38,   97,   97,  267,
97,  266,  265,  220,  154,  264,  263,   65,  103,  262,

260,  259,  257,  103,  256,  255,  103,  254,  253,  252,
251,  249,  103,  248,  103,  103,  270,  270,  270,  270,
270,  271,  271,  271,  271,  271,  272,  247,  246,  272,
273,  273,  274,  274,  274,  244,  243,  241,  240,  239,
238,  237,  236,  235,  234,  233,  232,  230,  229,  228,
227,  226,  225,  223,  222,  221,  219,  218,  217,  215,
214,  213,  212,  211,  210,  207,  206,  205,  204,  203,
202,  201,  200,  199,  198,  197,  196,  194,  193,  191,
190,  189,  188,  186,  185,  184,  183,  182,  180,  179,
178,  176,  175,  174,  173,  172,  171,  170,  169,  167,

163,  161,  160,  159,  157,  156,  155,  153,  152,  150,
149,  148,  147,  146,  145,  144,  142,  141,  140,  139,
138,  137,  136,  134,  132,  131,  130,  129,  128,  127,
126,  125,  123,  122,  121,  120,  117,  116,  115,  114,
113,  112,  111,  110,  108,  107,  106,  105,  104,  102,
101,  100,   99,   96,   95,   94,   93,   92,   91,   90,
89,   88,   87,   84,   83,   82,   81,   79,   78,   77,
76,   75,   74,   73,   72,   71,   70,   68,   67,   63,
62,   61,   60,   59,   57,   56,   55,   54,   53,   50,
49,   48,   47,   46,   45,   44,   43,   40,   36,   32,

31,   28,   24,   22,   21,   20,   19,   18,   15,   12,
11,    7,    5,  269,  269,  269,  269,  269,  269,  269,
269,  269,  269,  269,  269,  269,  269,  269,  269,  269,
269,  269,  269,  269,  269,  269,  269,  269,  269,  269,
269,  269,  269,  269,  269,  269,  269,  269,  269,  269
} ;






int yywrap()
{
    return 1;
}

char* fb_parser_getstring(
    const char*                    string,
    const unsigned int  length
) {
    PARSER_STACK *pentry = (PARSER_STACK*)malloc(sizeof(PARSER_STACK));
    if(!pentry) {

        return 0;
    }

    pentry->string = (char*)malloc(length+1);
    if(!(pentry->string)) {

        free(pentry);
        return 0;
    }
    pentry->pnext = pStrStack;
    pStrStack = pentry;

    strncpy(pentry->string, string, length);
    pentry->string[length] = 0;

    return pentry->string;
}


void fb_parser_freestrings(void) {

    PARSER_STACK *pentry;

    while(pStrStack) {
        pentry = pStrStack;
        pStrStack = pStrStack->pnext;
        if(pentry->string) free(pentry->string);
        free(pentry);
    }
}

int yylex ( void )
{
    register yy_state_type yy_current_state;
    register char *yy_cp, *yy_bp;
    register int yy_act;

    if ( yy_init )
    {
        yy_init = 0;
        if ( ! yy_start )
            yy_start = 1;        

        if ( ! yyin )
            yyin = stdin;

        if ( ! yyout )
            yyout = stdout;

        if ( ! yy_current_buffer )
            yy_current_buffer =
                yy_create_buffer( yyin, 16384 );

        yy_load_buffer_state();
    }

    while ( 1 )                
    {
    yy_cp = yy_c_buf_p;
    *yy_cp = yy_hold_char;
    yy_bp = yy_cp;

    yy_current_state = yy_start;
yy_match:
    do
    {
        register YY_CHAR yy_c = yy_ec[((unsigned int) (unsigned char) *yy_cp)];
        if ( yy_accept[yy_current_state] )
        {
            yy_last_accepting_state = yy_current_state;
            yy_last_accepting_cpos = yy_cp;
        }
        while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
        {
            yy_current_state = (int) yy_def[yy_current_state];
            if ( yy_current_state >= 270 )
                yy_c = yy_meta[(unsigned int) yy_c];
        }
        yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
        ++yy_cp;
    }
    while ( yy_base[yy_current_state] != 314 );

yy_find_action:
    yy_act = yy_accept[yy_current_state];
    if ( yy_act == 0 )
    { 
        yy_cp = yy_last_accepting_cpos;
        yy_current_state = yy_last_accepting_state;
        yy_act = yy_accept[yy_current_state];
    }

    yytext = yy_bp;
    yyleng = (int) (yy_cp - yy_bp); 
    yy_hold_char = *yy_cp;
    *yy_cp = '\0'; 
    yy_c_buf_p = yy_cp;


do_action:        

    switch ( yy_act )
    { 
    case 0: 

        *yy_cp = yy_hold_char;
        yy_cp = yy_last_accepting_cpos;
        yy_current_state = yy_last_accepting_state;
        goto yy_find_action;

    case 1:
        yy_start = 1 + 2 *(1);
        break;
    case 2:
        break;
    case 3:
        current_line++;
        break;
    case 4:
        yy_start = 1 + 2 *(0);
        break;
    case 5:
        {
            yylval.datatype = ((KS_VAR_TYPE)0x02);
            return 266;
        }
        break;
    case 6:
        {
            yylval.datatype = ((KS_VAR_TYPE)0x10);
            return 266;
        }
        break;
    case 7:
        {
            yylval.datatype = ((KS_VAR_TYPE)0x11);
            return 266;
        }
        break;
    case 8:
        {
            yylval.datatype = ((KS_VAR_TYPE)0x20);
            return 266;
        }
        break;
    case 9:
        {
            yylval.datatype = ((KS_VAR_TYPE)0x21);
            return 266;
        }
        break;
    case 10:
        { 
            yylval.datatype = ((KS_VAR_TYPE)0x31);
            return 266;
        }
        break;
    case 11:
        { 
            yylval.datatype = ((KS_VAR_TYPE)0x32);
            return 266; 
        }
        break;
    case 12:
        { 
            yylval.datatype = ((KS_VAR_TYPE)0x30);
            return 266;
        }
        break;
    case 13:


        return 286;
        break;
    case 14:


        return 287;
        break;
    case 15:


        return 288;
        break;
    case 16:


        return 289;
        break;
    case 17:


        return 267;
        break;
    case 18:


        return 273;
        break;
    case 19:


        return 268;
        break;
    case 20:


        return 269;
        break;
    case 21:


        return 270;
        break;
    case 22:


        return 271;
        break;
    case 23:


        return 272;
        break;
    case 24:


        return 274;
        break;
    case 25:
        return 275;
        break;
    case 26:
        return 276;
        break;
    case 27:
        return 279;
        break;
    case 28:
        return 280;
        break;
    case 29:
        return 281;
        break;
    case 30:
        return 282;
        break;
    case 31:
        return 277;
        break;
    case 32:
        return 278;
        break;
    case 33:
        return 283;
        break;
    case 34:
        return 284;
        break;
    case 35:
        return 285;
        break;
    case 36:
        return 285;
        break;
    case 37:
        return 290;
        break;
    case 38:
        {
            yylval.string = fb_parser_getstring("TRUE", 4);
            return 264;                                                
        }
        break;
    case 39:
        {
            yylval.string = fb_parser_getstring("FALSE", 5);
            return 264;                                                
        }
        break;
    case 40:
        {
            yylval.string = fb_parser_getstring(yytext, yyleng);
            return 258;
        }
        break;
    case 41:
        {
            yylval.string = fb_parser_getstring(yytext, yyleng);
            return 259;
        }
        break;
    case 42:
        {
            yylval.string = fb_parser_getstring(yytext, yyleng);
            return 265;
        }
        break;
    case 43:
        { 
            yylval.string = fb_parser_getstring(yytext, yyleng);
            return 261; 
        }
        break;
    case 44:
        { 
            yylval.string = fb_parser_getstring(yytext, yyleng);
            return 263; 
        }
        break;
    case 45:
        {
            yylval.string = fb_parser_getstring(yytext+1, yyleng-2);
            return 260;
        }
        break;
    case 46:
        break;
    case 47:
        current_line++;
        break;
    case 48:
        return *yytext;
        break;

    case 49:
        (void) fwrite( yytext, yyleng, 1, yyout );
        break;

    case (50 + 0 + 1):
    case (50 + 1 + 1):
        return 0;

    case 50:
    {
        int yy_amount_of_matched_text = (int) (yy_cp - yytext) - 1;

        *yy_cp = yy_hold_char;

        if ( yy_current_buffer->yy_buffer_status == 0 )
        {
            yy_n_chars = yy_current_buffer->yy_n_chars;
            yy_current_buffer->yy_input_file = yyin;
            yy_current_buffer->yy_buffer_status = 1;
        }
        
        if ( yy_c_buf_p <= &yy_current_buffer->yy_ch_buf[yy_n_chars] )
        { 
            yy_state_type yy_next_state;

            yy_c_buf_p = yytext + yy_amount_of_matched_text;

            yy_current_state = yy_get_previous_state();

            yy_next_state = yy_try_NUL_trans( yy_current_state );

            yy_bp = yytext + 0;

            if ( yy_next_state )
            {
                yy_cp = ++yy_c_buf_p;
                yy_current_state = yy_next_state;
                goto yy_match;
            }
            else
            {
                yy_cp = yy_c_buf_p;
                goto yy_find_action;
            }
        }
        else switch ( yy_get_next_buffer() )
            {
                case 1:
                    {
                        yy_did_buffer_switch_on_eof = 0;

                        if ( yywrap() )
                        {
                            yy_c_buf_p = yytext + 0;

                            yy_act = (50 + ((yy_start - 1) / 2) + 1);
                            goto do_action;
                        }

                        else
                        {
                            if ( ! yy_did_buffer_switch_on_eof )
                                yyrestart( yyin );
                        }
                    break;
                    }

                case 0:
                    yy_c_buf_p =
                        yytext + yy_amount_of_matched_text;

                    yy_current_state = yy_get_previous_state();

                    yy_cp = yy_c_buf_p;
                    yy_bp = yytext + 0;
                    goto yy_match;

                case 2:
                    yy_c_buf_p =
                        &yy_current_buffer->yy_ch_buf[yy_n_chars];

                    yy_current_state = yy_get_previous_state();

                    yy_cp = yy_c_buf_p;
                    yy_bp = yytext + 0;
                    goto yy_find_action;
                }
            break;
            }

            default:
                    yy_fatal_error( "fatal flex scanner internal error--no action found" );
        } 
    } 
} 

/*************************************************/
static int yy_get_next_buffer()
{
    register char *dest = yy_current_buffer->yy_ch_buf;
    register char *source = yytext;
    register int number_to_move, i;
    int ret_val;

    if ( yy_c_buf_p > &yy_current_buffer->yy_ch_buf[yy_n_chars + 1] )
        yy_fatal_error( "fatal flex scanner internal error--end of buffer missed" );

    if ( yy_current_buffer->yy_fill_buffer == 0 )
    { 
        if ( yy_c_buf_p - yytext - 0 == 1 )
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }

    number_to_move = (int) (yy_c_buf_p - yytext) - 1;

    for ( i = 0; i < number_to_move; ++i )
        *(dest++) = *(source++);

    if ( yy_current_buffer->yy_buffer_status == 2 )
        yy_current_buffer->yy_n_chars = yy_n_chars = 0;

    else
        {
            int num_to_read =
                yy_current_buffer->yy_buf_size - number_to_move - 1;

            while ( num_to_read <= 0 )
            { 
                YY_BUFFER_STATE b = yy_current_buffer;

                int yy_c_buf_p_offset =
                    (int) (yy_c_buf_p - b->yy_ch_buf);

                if ( b->yy_is_our_buffer )
                {
                    int new_size = b->yy_buf_size * 2;

                    if ( new_size <= 0 )
                        b->yy_buf_size += b->yy_buf_size / 8;
                    else
                        b->yy_buf_size *= 2;

                    b->yy_ch_buf = (char *)
                        yy_flex_realloc( (void *) b->yy_ch_buf,
                                        b->yy_buf_size + 2 );
                }
                else
                    b->yy_ch_buf = 0;

                if ( ! b->yy_ch_buf )
                    yy_fatal_error( "fatal error - scanner input buffer overflow" );

                yy_c_buf_p = &b->yy_ch_buf[yy_c_buf_p_offset];

                num_to_read = yy_current_buffer->yy_buf_size -
                              number_to_move - 1;
            }

            if ( num_to_read > 8192 )
                num_to_read = 8192;

/*
*           if ( yy_current_buffer->yy_is_interactive ) {
*                int c = '*', n;
*                for ( n = 0; n < num_to_read && (c = ((--((yyin)->level) >= 0) ? (unsigned char)(*(yyin)->curp++) : _fgetc (yyin))) != (-1) && c != '\n'; ++n )
*                    (&yy_current_buffer->yy_ch_buf[number_to_move])[n] = (char) c;
*        
*                if ( c == '\n' )
*                    (&yy_current_buffer->yy_ch_buf[number_to_move])[n++] = (char) c;
*            
*                if ( c == (-1) && ((yyin)->flags & 0x0010) )
*                    yy_fatal_error( "input in flex scanner failed" );
*            
*                yy_n_chars = n;
*            } else
*                if ( ((yy_n_chars = fread( (&yy_current_buffer->yy_ch_buf[number_to_move]), 1, num_to_read, yyin )) == 0) && ((yyin)->flags & 0x0010) )
*                    yy_fatal_error( "input in flex scanner failed" );
*/
        yy_current_buffer->yy_n_chars = yy_n_chars;
    }

    if ( yy_n_chars == 0 )
    {
        if ( number_to_move == 0 )
        {
            ret_val = 1;
            yyrestart( yyin );
        }
        else
        {
            ret_val = 2;
            yy_current_buffer->yy_buffer_status =
                    2;
        }
    }
    else
        ret_val = 0;

    yy_n_chars += number_to_move;
    yy_current_buffer->yy_ch_buf[yy_n_chars] = 0;
    yy_current_buffer->yy_ch_buf[yy_n_chars + 1] = 0;

    yytext = &yy_current_buffer->yy_ch_buf[0];

    return ret_val;
}




static yy_state_type yy_get_previous_state()
{
register yy_state_type yy_current_state;
register char *yy_cp;

yy_current_state = yy_start;

for ( yy_cp = yytext + 0; yy_cp < yy_c_buf_p; ++yy_cp )
{
register YY_CHAR yy_c = (*yy_cp ? yy_ec[((unsigned int) (unsigned char) *yy_cp)] : 1);
if ( yy_accept[yy_current_state] )
{
yy_last_accepting_state = yy_current_state;
yy_last_accepting_cpos = yy_cp;
}
while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
{
yy_current_state = (int) yy_def[yy_current_state];
if ( yy_current_state >= 270 )
yy_c = yy_meta[(unsigned int) yy_c];
}
yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
}

return yy_current_state;
}


static yy_state_type yy_try_NUL_trans( yy_state_type yy_current_state )
{
register int yy_is_jam;
register char *yy_cp = yy_c_buf_p;

register YY_CHAR yy_c = 1;
if ( yy_accept[yy_current_state] )
{
yy_last_accepting_state = yy_current_state;
yy_last_accepting_cpos = yy_cp;
}
while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
{
yy_current_state = (int) yy_def[yy_current_state];
if ( yy_current_state >= 270 )
yy_c = yy_meta[(unsigned int) yy_c];
}
yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
yy_is_jam = (yy_current_state == 269);

return yy_is_jam ? 0 : yy_current_state;
}




static void yyunput( int c, register char *yy_bp )
{
register char *yy_cp = yy_c_buf_p;


*yy_cp = yy_hold_char;

if ( yy_cp < yy_current_buffer->yy_ch_buf + 2 )
{ 

register int number_to_move = yy_n_chars + 2;
register char *dest = &yy_current_buffer->yy_ch_buf[
yy_current_buffer->yy_buf_size + 2];
register char *source =
&yy_current_buffer->yy_ch_buf[number_to_move];

while ( source > yy_current_buffer->yy_ch_buf )
*--dest = *--source;

yy_cp += (int) (dest - source);
yy_bp += (int) (dest - source);
yy_current_buffer->yy_n_chars =
yy_n_chars = yy_current_buffer->yy_buf_size;

if ( yy_cp < yy_current_buffer->yy_ch_buf + 2 )
yy_fatal_error( "flex scanner push-back overflow" );
}

*--yy_cp = (char) c;


yytext = yy_bp;
yy_hold_char = *yy_cp;
yy_c_buf_p = yy_cp;
}


static int input()
{
    int c;

    *yy_c_buf_p = yy_hold_char;

    if ( *yy_c_buf_p == 0 ) {

        if ( yy_c_buf_p < &yy_current_buffer->yy_ch_buf[yy_n_chars] )

            *yy_c_buf_p = '\0';

        else { 
            int offset = yy_c_buf_p - yytext;
            ++yy_c_buf_p;

            switch ( yy_get_next_buffer() ) {
                case 2:
                    yyrestart( yyin );
                case 1:
                        {
                            if ( yywrap() )
                                return (-1);

                            if ( ! yy_did_buffer_switch_on_eof )
                                yyrestart( yyin );

                            return input();

                        }

                case 0:
                    yy_c_buf_p = yytext + offset;
                    break;
            }
        }
    }

    c = *(unsigned char *) yy_c_buf_p;        
    *yy_c_buf_p = '\0';        
    yy_hold_char = *++yy_c_buf_p;

    return c;
}



void yyrestart( FILE *input_file )
{
if ( ! yy_current_buffer )
yy_current_buffer = yy_create_buffer( yyin, 16384 );

yy_init_buffer( yy_current_buffer, input_file );
yy_load_buffer_state();
}



void yy_switch_to_buffer( YY_BUFFER_STATE new_buffer )
{
if ( yy_current_buffer == new_buffer )
return;

if ( yy_current_buffer )
{

*yy_c_buf_p = yy_hold_char;
yy_current_buffer->yy_buf_pos = yy_c_buf_p;
yy_current_buffer->yy_n_chars = yy_n_chars;
}

yy_current_buffer = new_buffer;
yy_load_buffer_state();
yy_did_buffer_switch_on_eof = 1;
}



void yy_load_buffer_state( void )
{
yy_n_chars = yy_current_buffer->yy_n_chars;
yytext = yy_c_buf_p = yy_current_buffer->yy_buf_pos;
yyin = yy_current_buffer->yy_input_file;
yy_hold_char = *yy_c_buf_p;
}



YY_BUFFER_STATE yy_create_buffer( FILE *file, int size )
{
YY_BUFFER_STATE b;

b = (YY_BUFFER_STATE) yy_flex_alloc( sizeof( struct yy_buffer_state ) );
if ( ! b )
yy_fatal_error( "out of dynamic memory in yy_create_buffer()" );

b->yy_buf_size = size;




b->yy_ch_buf = (char *) yy_flex_alloc( b->yy_buf_size + 2 );
if ( ! b->yy_ch_buf )
yy_fatal_error( "out of dynamic memory in yy_create_buffer()" );

b->yy_is_our_buffer = 1;

yy_init_buffer( b, file );

return b;
}



void yy_delete_buffer( YY_BUFFER_STATE b )
{
if ( ! b )
return;

if ( b == yy_current_buffer )
yy_current_buffer = (YY_BUFFER_STATE) 0;

if ( b->yy_is_our_buffer )
yy_flex_free( (void *) b->yy_ch_buf );

yy_flex_free( (void *) b );
}


void yy_init_buffer( YY_BUFFER_STATE b, FILE *file )
{
yy_flush_buffer( b );

b->yy_input_file = file;
b->yy_fill_buffer = 1;

// FUER EH KEINE FILES        b->yy_is_interactive = file ? (isatty( ((file)->fd) ) > 0) : 0;
        b->yy_is_interactive = 0;
}



void yy_flush_buffer( YY_BUFFER_STATE b )
{
if ( ! b )
return;

b->yy_n_chars = 0;





b->yy_ch_buf[0] = 0;
b->yy_ch_buf[1] = 0;

b->yy_buf_pos = &b->yy_ch_buf[0];

b->yy_at_bol = 1;
b->yy_buffer_status = 0;

if ( b == yy_current_buffer )
yy_load_buffer_state();
}




YY_BUFFER_STATE yy_scan_buffer( char *base, yy_size_t size )
{
YY_BUFFER_STATE b;

if ( size < 2 ||
base[size-2] != 0 ||
base[size-1] != 0 )

return 0;

b = (YY_BUFFER_STATE) yy_flex_alloc( sizeof( struct yy_buffer_state ) );
if ( ! b )
yy_fatal_error( "out of dynamic memory in yy_scan_buffer()" );

b->yy_buf_size = size - 2;        
b->yy_buf_pos = b->yy_ch_buf = base;
b->yy_is_our_buffer = 0;
b->yy_input_file = 0;
b->yy_n_chars = b->yy_buf_size;
b->yy_is_interactive = 0;
b->yy_at_bol = 1;
b->yy_fill_buffer = 0;
b->yy_buffer_status = 0;

yy_switch_to_buffer( b );

return b;
}

YY_BUFFER_STATE yy_scan_string( const char *yy_str )
{
int len;
for ( len = 0; yy_str[len]; ++len )
;

return yy_scan_bytes( yy_str, len );
}

YY_BUFFER_STATE yy_scan_bytes( const char *bytes, int len )
{
YY_BUFFER_STATE b;
char *buf;
yy_size_t n;
int i;


n = len + 2;
buf = (char *) yy_flex_alloc( n );
if ( ! buf )
yy_fatal_error( "out of dynamic memory in yy_scan_bytes()" );

for ( i = 0; i < len; ++i )
buf[i] = bytes[i];

buf[len] = buf[len+1] = 0;

b = yy_scan_buffer( buf, n );
if ( ! b )
yy_fatal_error( "bad buffer in yy_scan_bytes()" );




b->yy_is_our_buffer = 1;

return b;
}

static void yy_fatal_error( const char msg[] )
{
fprintf( stdout, "%s\n", msg );
exit( 2 );
}

static void *yy_flex_alloc( yy_size_t size )
{
return (void *) malloc( size );
}


static void *yy_flex_realloc( void *ptr, yy_size_t size )
{
return (void *) realloc( (char *) ptr, size );
}


static void yy_flex_free( void *ptr )
{
free( ptr );
}