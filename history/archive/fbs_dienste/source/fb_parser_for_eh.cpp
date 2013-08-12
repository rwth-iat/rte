#include <stdlib.h> 
#include <stdio.h>
#include "fb_scanner_for_EH.h"
#include "par_param.h" 

extern "C" void iFBS_SetParserError(int, char*);

//extern __declspec( dllexport) int                                 current_line;
//extern __declspec( dllexport) Dienst_param*         ppar;

__declspec( dllexport) int yydebug;                         


int        yychar;                         
int yynerrs;                         


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

YYSTYPE        yylval;                         

static  char yytranslate[] = {     0,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,    41,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,    36,    37,     2,
38,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
42,     2,    43,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,    39,     2,    40,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
26,    27,    28,    29,    30,    31,    32,    33,    34,    35
};

  
static  short yyprhs[] = {     0,
0,     1,     3,     5,     8,    10,    12,    14,    16,    18,
20,    22,    24,    27,    37,    39,    42,    67,    69,    72,
97,    99,   103,   105,   107,   110,   115,   117,   120,   125,
127,   130,   135,   137,   140,   147,   148,   153,   154,   156,
158,   161,   163,   165,   174,   175,   179,   190,   192,   194,
196,   200,   201,   203,   207,   209,   211,   213,   215
};

static  short yyrhs[] = {    -1,
45,     0,    46,     0,    45,    46,     0,    47,     0,    49,
0,    51,     0,    55,     0,    57,     0,    59,     0,    61,
0,    48,     0,    47,    48,     0,    12,     4,    36,    18,
4,    37,    63,    15,    37,     0,    50,     0,    49,    50,
0,    16,    17,     3,    37,    19,     3,    36,    18,     3,
38,     4,    37,    20,     3,    36,    18,     3,    38,    39,
53,    40,    37,    21,    37,     0,    52,     0,    51,    52,
0,    22,    17,     3,    37,    19,     3,    36,    18,     3,
38,     4,    37,    20,     3,    36,    18,     3,    38,    39,
53,    40,    37,    23,    37,     0,    54,     0,    53,    41,
54,     0,     4,     0,    56,     0,    55,    56,     0,    26,
4,    27,    37,     0,    58,     0,    57,    58,     0,    31,
3,    32,    37,     0,    60,     0,    59,    60,     0,    33,
3,    34,    37,     0,    62,     0,    61,    62,     0,    24,
4,    36,    63,    25,    37,     0,     0,    13,    64,    14,
37,     0,     0,    65,     0,    66,     0,    65,    66,     0,
67,     0,    69,     0,     3,    36,    70,    11,    38,    73,
68,    37,     0,     0,    35,    38,     8,     0,     3,    42,
8,    43,    36,    70,    11,    38,    71,    37,     0,    28,
0,    29,     0,    30,     0,    39,    72,    40,     0,     0,
73,     0,    72,    41,    73,     0,     6,     0,     8,     0,
9,     0,     5,     0,    10,     0
};



  
static  short yyrline[] = { 0,
115,   119,   124,   128,   133,   151,   168,   184,   199,   213,
227,   242,   246,   257,   292,   296,   306,   336,   340,   351,
381,   385,   397,   414,   418,   431,   446,   450,   462,   477,
481,   493,   508,   512,   524,   557,   561,   566,   570,   575,
579,   595,   599,   604,   699,   703,   712,   830,   834,   838,
843,   848,   852,   856,   868,   882,   896,   910,   924
};



    

static  const char * const  yytname[] = {   "$","error","$undefined.","TOK_IDENTIFIER",
"TOK_PATH","TOK_STRINGVALUE","TOK_FLOATVALUE","TOK_DOUBLEVALUE","TOK_INTEGERVALUE",
"TOK_BOOLVALUE","TOK_TIMEVALUE","TOK_DATATYPE","TOK_INSTANCE","TOK_VARIABLE_VALUES",
"TOK_END_VARIABLE_VALUES","TOK_END_INSTANCE","TOK_LINK","TOK_OF_ASSOCIATION",
"TOK_CLASS","TOK_PARENT","TOK_CHILDREN","TOK_END_LINK","TOK_UNLINK","TOK_END_UNLINK",
"TOK_SET","TOK_END_SET","TOK_DELETE","TOK_END_DELETE","TOK_PARAM_PORT","TOK_INPUT_PORT",
"TOK_DUMMY_PORT","TOK_LIBRARY","TOK_END_LIBRARY","TOK_DEL_LIBRARY","TOK_END_DEL_LIBRARY",
"TOK_STATE","':'","';'","'='","'{'","'}'","','","'['","']'","start","blocks",
"block","instance_blocks","instance_block","link_blocks","link_block","unlink_blocks",
"unlink_block","child_paths","child_path","delinstance_blocks","delinstance_block",
"newlibs_blocks","newlibs_block","oldlibs_blocks","oldlibs_block","setinstvar_blocks",
"setinstvar_block","variable_values_block_opt","variable_values_opt","variable_values",
"variable_value","scalar_variable_value","state_opt","vector_variable_value",
"port_types","vector_value","vector_value_list","scalar_value", 0
};


static const short yyr1[] = {     0,
44,    44,    45,    45,    46,    46,    46,    46,    46,    46,
46,    47,    47,    48,    49,    49,    50,    51,    51,    52,
53,    53,    54,    55,    55,    56,    57,    57,    58,    59,
59,    60,    61,    61,    62,    63,    63,    64,    64,    65,
65,    66,    66,    67,    68,    68,    69,    70,    70,    70,
71,    72,    72,    72,    73,    73,    73,    73,    73
};

static const short yyr2[] = {     0,
0,     1,     1,     2,     1,     1,     1,     1,     1,     1,
1,     1,     2,     9,     1,     2,    24,     1,     2,    24,
1,     3,     1,     1,     2,     4,     1,     2,     4,     1,
2,     4,     1,     2,     6,     0,     4,     0,     1,     1,
2,     1,     1,     8,     0,     3,    10,     1,     1,     1,
3,     0,     1,     3,     1,     1,     1,     1,     1
};

static const short yydefact[] = {     1,
0,     0,     0,     0,     0,     0,     0,     2,     3,     5,
12,     6,    15,     7,    18,     8,    24,     9,    27,    10,
30,    11,    33,     0,     0,     0,     0,     0,     0,     0,
4,    13,    16,    19,    25,    28,    31,    34,     0,     0,
0,    36,     0,     0,     0,     0,     0,     0,    38,     0,
26,    29,    32,     0,     0,     0,     0,     0,    39,    40,
42,    43,     0,    36,     0,     0,     0,     0,     0,    41,
35,     0,     0,     0,    48,    49,    50,     0,     0,    37,
0,     0,     0,     0,     0,    14,     0,     0,     0,     0,
0,     0,    58,    55,    56,    57,    59,    45,     0,     0,
0,     0,     0,     0,     0,     0,     0,    44,     0,     0,
0,    46,    52,     0,     0,     0,     0,    53,    47,     0,
0,    51,     0,     0,     0,    54,     0,     0,     0,     0,
0,     0,    23,     0,    21,     0,     0,     0,     0,     0,
22,     0,     0,     0,    17,    20,     0,     0,     0
};

static const short yydefgoto[] = {   147,
8,     9,    10,    11,    12,    13,    14,    15,   134,   135,
16,    17,    18,    19,    20,    21,    22,    23,    50,    58,
59,    60,    61,   103,    62,    78,   114,   117,    98
};

static const short yypact[] = {   -12,
-1,    -8,    -4,    13,    14,    21,    25,   -12,-32768,     8,
-32768,    18,-32768,    11,-32768,     9,-32768,     5,-32768,     4,
-32768,    15,-32768,     2,    37,    38,     6,    16,    12,    19,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    27,    10,
20,    33,    22,    23,    24,    44,    30,    31,    48,    29,
-32768,-32768,-32768,    26,    49,    52,   -34,    42,    48,-32768,
-32768,-32768,    28,    33,    32,    34,   -23,    50,    35,-32768,
-32768,    47,    46,    51,-32768,-32768,-32768,    55,    36,-32768,
39,    64,    68,    40,    41,-32768,    43,    45,    17,   -23,
69,    70,-32768,-32768,-32768,-32768,-32768,    53,    71,    54,
56,    57,    59,    60,    65,    66,    67,-32768,    58,    77,
81,-32768,    17,    62,    72,    73,   -25,-32768,-32768,    74,
76,-32768,    17,    84,    86,-32768,    63,    75,    61,    78,
98,    98,-32768,   -11,-32768,    -9,    79,    98,    82,    83,
-32768,    80,    85,    87,-32768,-32768,    90,   105,-32768
};

static const short yypgoto[] = {-32768,
-32768,    99,-32768,    96,-32768,   100,-32768,    97,   -22,   -24,
-32768,   102,-32768,   103,-32768,    95,-32768,   101,    88,-32768,
-32768,    89,-32768,-32768,-32768,    91,-32768,-32768,  -112
};





static const short yytable[] = {     1,
118,    67,    24,     2,    75,    76,    77,    68,    25,     3,
126,     4,    26,     5,   122,   123,    27,    28,     6,     1,
7,    93,    94,    29,    95,    96,    97,    30,   137,   138,
139,   138,     3,     2,     5,     6,     7,    39,     4,    40,
41,    42,    43,    44,    46,    49,    47,    54,    55,    56,
57,    65,    45,    63,    66,    69,    48,    79,    51,    52,
53,    81,    64,    82,    71,    84,    87,    73,    83,    74,
88,    80,   100,   101,   112,    86,    90,    89,    85,   115,
91,   104,    92,   116,   110,   111,   127,   102,   128,   148,
105,   124,   106,   125,   107,   108,   113,   109,   119,   131,
129,   133,   144,   143,   149,    32,    31,   120,   121,   136,
34,    33,   130,   141,    37,   140,   132,    35,   142,     0,
36,   145,    38,   146,     0,     0,     0,     0,     0,     0,
0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
0,     0,     0,     0,     0,     0,     0,    70,     0,     0,
0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
99
};

static const short yycheck[] = {    12,
113,    36,     4,    16,    28,    29,    30,    42,    17,    22,
123,    24,    17,    26,    40,    41,     4,     4,    31,    12,
33,     5,     6,     3,     8,     9,    10,     3,    40,    41,
40,    41,    22,    16,    26,    31,    33,    36,    24,     3,
3,    36,    27,    32,    18,    13,    37,     4,    19,    19,
3,     3,    34,    25,     3,    14,    37,     8,    37,    37,
37,    15,    37,    18,    37,    11,     3,    36,    18,    36,
3,    37,     4,     4,     8,    37,    36,    38,    43,     3,
38,    11,    38,     3,    20,    20,     3,    35,     3,     0,
37,    18,    37,    18,    38,    37,    39,    38,    37,    39,
38,     4,    23,    21,     0,    10,     8,    36,    36,   132,
14,    12,    38,   138,    20,    37,    39,    16,    37,    -1,
18,    37,    22,    37,    -1,    -1,    -1,    -1,    -1,    -1,
-1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
-1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,
-1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
-1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
-1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
90
};


static void
__yy_memcpy (char *to, char *from, int count)
{
register char *f = from;
register char *t = to;
register int i = count;

while (i-- > 0)
*t++ = *f++;
}

int yyerror(char *s)
{
  static char msg[256];
  strcpy(msg, s);
  
  iFBS_SetParserError(current_line+1, msg);
  
  return 1 ;
}


int
yyparse()
{
    register int yystate;
    register int yyn;
    register short *yyssp;
    register YYSTYPE *yyvsp;
    int yyerrstatus;        
    int yychar1 = 0;                

    short        yyssa[200];        
    YYSTYPE yyvsa[200];        

    short *yyss = yyssa;                
    YYSTYPE *yyvs = yyvsa;        

    int yystacksize = 200;

    YYSTYPE yyval;                
    int yylen;

    yystate = 0;
    yyerrstatus = 0;
    yynerrs = 0;
    yychar = -2;                

    yyssp = yyss - 1;
    yyvsp = yyvs;

yynewstate:

    *++yyssp = yystate;

    if (yyssp >= yyss + yystacksize - 1)
    {

        YYSTYPE *yyvs1 = yyvs;
        short *yyss1 = yyss;
        int size = yyssp - yyss + 1;
        
        if (yystacksize >= 10000)
        {
            yyerror("parser stack overflow");
            return 2;
        }
        yystacksize *= 2;
        if (yystacksize > 10000)
            yystacksize = 10000;
        yyss = (short *) malloc (yystacksize * sizeof (*yyssp));
        __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
        yyvs = (YYSTYPE *) malloc (yystacksize * sizeof (*yyvsp));
        __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));

        yyssp = yyss + size - 1;
        yyvsp = yyvs + size - 1;

        if (yyssp >= yyss + yystacksize - 1)
            return(1);
    }

    goto yybackup;

yybackup:
    yyn = yypact[yystate];
    if (yyn == -32768)
        goto yydefault;

    if (yychar == -2)
    {
        yychar = yylex();
    }



    if (yychar <= 0)                
    {
        yychar1 = 0;
        yychar = 0;                
    }
    else
    {
        yychar1 = ((unsigned)(yychar) <= 290 ? yytranslate[yychar] : 74);
    }

    yyn += yychar1;
    if (yyn < 0 || yyn > 181 || yycheck[yyn] != yychar1)
        goto yydefault;

    yyn = yytable[yyn];

    if (yyn < 0)
    {
        if (yyn == -32768)
            goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
    }
    else if (yyn == 0)
        goto yyerrlab;

    if (yyn == 149)
        return(0);

    if (yychar != 0)
        yychar = -2;

    *++yyvsp = yylval;

    if (yyerrstatus) yyerrstatus--;

    yystate = yyn;
    goto yynewstate;


yydefault:

    yyn = yydefact[yystate];
    if (yyn == 0)
        goto yyerrlab;


yyreduce:
    yylen = yyr2[yyn];
    if (yylen > 0)
        yyval = yyvsp[1-yylen]; 

    switch (yyn) {

        case 1:
        {
            break;
        }
        case 2:
        {
            break;
        }
        case 3:
        {
            yyval.par = yyvsp[0].par;
            break;
        }
        case 4:
        {
            yyval.par = yyvsp[0].par;
            break;
        }
        case 5:
        {
            InstanceItems* pII;

            if (ppar->Instance) {

                pII = ppar->Instance;
                while(pII->next) {
                    pII=pII->next;
                }
                pII->next = yyvsp[0].instans;

            } else {

                ppar->Instance = yyvsp[0].instans;
            }
            yyval.par = ppar;
            break;}
        case 6:
        {
            LinksItems* pLI;

            if(ppar->Links) {

                pLI = ppar->Links;
                while(pLI->next) {
                    pLI=pLI->next;
                }
                pLI->next = yyvsp[0].link;
            } else {

                ppar->Links = yyvsp[0].link;
            }
            yyval.par = ppar;
            break;}
        case 7:
        {
            LinksItems* pLI;

            if(ppar->UnLinks) {

                pLI=ppar->UnLinks;
                while(pLI->next) {
                    pLI=pLI->next;
                }
                pLI->next = yyvsp[0].link;
            } else {
                ppar->UnLinks = yyvsp[0].link;
            }
            yyval.par = ppar;
            break;}
        case 8:
        {
            DelInstItems* pDI;

            if(ppar->DelInst) {
                pDI=ppar->DelInst;
                while(pDI->next) {
                    pDI=pDI->next;
                }
                pDI->next = yyvsp[0].delinstans;
            } else {
                ppar->DelInst = yyvsp[0].delinstans;
            }
            yyval.par = ppar;
            break;}
        case 9:
        {
            DelInstItems* pDI;
            if(ppar->NewLibs) {
                pDI=ppar->NewLibs;
                while(pDI->next) {
                    pDI=pDI->next;
                }
                pDI->next = yyvsp[0].delinstans;
            } else {
                ppar->NewLibs = yyvsp[0].delinstans;
            }
            yyval.par = ppar;
            break;}
        case 10:
        {
            DelInstItems* pDI;
            if(ppar->OldLibs) {
                pDI=ppar->OldLibs;
                while(pDI->next) {
                    pDI=pDI->next;
                }
                pDI->next = yyvsp[0].delinstans;
            } else {
                ppar->OldLibs = yyvsp[0].delinstans;
            }
            yyval.par = ppar;
            break;}
        case 11:
        {
            SetInstVarItems* pSI;
            if (ppar->Set_Inst_Var) {
                pSI=ppar->Set_Inst_Var;
                while(pSI->next) {
                    pSI=pSI->next;
                }
                pSI->next = yyvsp[0].setinstvar;
            } else {
                ppar->Set_Inst_Var = yyvsp[0].setinstvar;
            }
            yyval.par = ppar;
            break;}
        case 12:
        {
            yyval.instans = yyvsp[0].instans;
            break;}
        case 13:
        {
            InstanceItems *pInstList = yyvsp[-1].instans;

            while( pInstList->next) {
                pInstList = pInstList->next;
            }
            pInstList->next = yyvsp[0].instans;
            yyval.instans = yyvsp[-1].instans;
            break;}
        case 14:
        {
            Variables      *pVar;
            InstanceItems  *pinst = (InstanceItems*)malloc(sizeof(InstanceItems));

            if( (!yyvsp[-7].string) || (!yyvsp[-4].string) || (!pinst) ) {
                if(pinst) free(pinst);
                pVar = yyvsp[-2].variable;
                while(pVar) {
                    yyvsp[-2].variable = pVar->next;
                    if(pVar->value) {
                        VariableItem *pVal = pVar->value;
                        while(pVal) {
                            pVar->value = pVar->value->next;
                            free(pVal);
                            pVal = pVar->value;
                        }
                    }
                    free(pVar);
                    pVar = yyvsp[-2].variable;
                }
                yyerror("out of memory");
                return 1;
            }
            pinst->next = 0;
            pinst->Inst_name = yyvsp[-7].string;
            pinst->Class_name = yyvsp[-4].string;
            pinst->Inst_var = yyvsp[-2].variable;

            yyval.instans = pinst;
            break;}
        case 15:
        {
            yyval.link = yyvsp[0].link;
            break;}
        case 16:
        {
            LinksItems *pLinkList = yyvsp[-1].link;

            while ( pLinkList->next)
                pLinkList = pLinkList->next;
            pLinkList->next = yyvsp[0].link;
            yyval.link = yyvsp[-1].link;
            break;}
        case 17:
        {
            LinksItems*                 plink;
            Child*              pchild;

            plink = (LinksItems*)malloc(sizeof(LinksItems));
            if( (!plink) || (!yyvsp[-13].string) || (!yyvsp[-10].string) || (!yyvsp[-4].ch_path) ) {
                if(plink) free(plink);
                pchild = yyvsp[-4].ch_path;
                while(pchild) {
                    yyvsp[-4].ch_path = pchild->next;
                    free(pchild);
                    pchild = yyvsp[-4].ch_path;
                }
                yyerror("out of memory");
                return 1;
            }
            plink->next = 0;
            plink->parent_path = yyvsp[-13].string;
            plink->child_role = yyvsp[-10].string;
            plink->children = yyvsp[-4].ch_path;

            yyval.link = plink;
            break;}
        case 18:
        {
            yyval.link = yyvsp[0].link;
            break;}
        case 19:
        {
            LinksItems *pLinkList = yyvsp[-1].link;

            while ( pLinkList->next) {
                pLinkList = pLinkList->next;
            }
            pLinkList->next = yyvsp[0].link;
            yyval.link = yyvsp[-1].link;
            break;}
        case 20:
        {
            LinksItems*                 plink;
            Child*              pchild;

            plink = (LinksItems*)malloc(sizeof(LinksItems));
            if( (!plink) || (!yyvsp[-13].string) || (!yyvsp[-10].string) || (!yyvsp[-4].ch_path) ) {
                if(plink) free(plink);
                pchild = yyvsp[-4].ch_path;
                while(pchild) {
                    yyvsp[-4].ch_path = pchild->next;
                    free(pchild);
                    pchild = yyvsp[-4].ch_path;
                }
                yyerror("out of memory");
                return 1;
            }
            plink->next = 0;
            plink->parent_path = yyvsp[-13].string;
            plink->child_role = yyvsp[-10].string;
            plink->children = yyvsp[-4].ch_path;

            yyval.link=plink;
            break;}
        case 21:
        {
            yyval.ch_path = yyvsp[0].ch_path;
            break;}
        case 22:
        {
            Child* pchild;
            pchild = yyvsp[-2].ch_path;

            while(pchild->next) {
                pchild = pchild->next;
            }
            pchild->next = yyvsp[0].ch_path;
            yyval.ch_path = yyvsp[-2].ch_path;
            break;}
        case 23:
        {
            Child* pchild;
            if(!yyvsp[0].string) {
                yyerror("out of memory");
                return 1;
            }
            pchild = (Child*)malloc(sizeof(Child));
            if(!pchild) {
                yyerror("out of memory");
                return 1;
            }
            pchild->next = 0;
            pchild->child_path = yyvsp[0].string;
            yyval.ch_path = pchild;
            break;}
        case 24:
        {
            yyval.delinstans = yyvsp[0].delinstans;
            break;}
        case 25:
        {
            DelInstItems* pdelinst;
            pdelinst = yyvsp[-1].delinstans;
            while ( pdelinst->next) {
                pdelinst = pdelinst->next;
            }
            pdelinst->next = yyvsp[0].delinstans;
            yyval.delinstans = yyvsp[-1].delinstans;
            break;}
        case 26:
        {
            DelInstItems *pd = (DelInstItems*)malloc(sizeof(DelInstItems));
            if((!pd) || (!yyvsp[-2].string)) {
                if(pd) free(pd);
                yyerror("out of memory");
                return 1;
            }
            pd->next = 0;
            pd->Inst_name = yyvsp[-2].string;

            yyval.delinstans = pd;
            break;}
        case 27:
        {
            yyval.delinstans = yyvsp[0].delinstans;
            break;}
        case 28:
        {
            DelInstItems* pdelinst;
            pdelinst = yyvsp[-1].delinstans;

            while ( pdelinst->next) {
                pdelinst = pdelinst->next;
            }
            pdelinst->next = yyvsp[0].delinstans;
            yyval.delinstans = yyvsp[-1].delinstans;
            break;}
        case 29:
        {
            DelInstItems* pd = (DelInstItems*)malloc(sizeof(DelInstItems));
            if( (!pd) || (!yyvsp[-2].string) ) {
                if(pd) free(pd);
                yyerror("out of memory");
                return 1;
            }
            pd->next = 0;
            pd->Inst_name = yyvsp[-2].string;

            yyval.delinstans = pd;
            break;}
        case 30:
        {
            yyval.delinstans = yyvsp[0].delinstans;
            break;}
        case 31:
        {
            DelInstItems* pdelinst;
            pdelinst = yyvsp[-1].delinstans;

            while ( pdelinst->next) {
                pdelinst = pdelinst->next;
            }
            pdelinst->next = yyvsp[0].delinstans;
            yyval.delinstans = yyvsp[-1].delinstans;
            break;}
        case 32:
        {
            DelInstItems *pd = (DelInstItems*)malloc(sizeof(DelInstItems));
            if( (!pd) || (!yyvsp[-2].string) ) {
                if(pd) free(pd);
                yyerror("out of memory");
                return 1;
            }
            pd->next = 0;
            pd->Inst_name = yyvsp[-2].string;

            yyval.delinstans = pd;
            break;}
        case 33:
        {
            yyval.setinstvar = yyvsp[0].setinstvar;
            break;}
        case 34:
        {
            SetInstVarItems* psiv;
            psiv = yyvsp[-1].setinstvar;

            while ( psiv->next) {
                psiv = psiv->next;
            }
            psiv->next = yyvsp[0].setinstvar;
            yyval.setinstvar = yyvsp[-1].setinstvar;
            break;}
        case 35:
        {
            Variables       *pVar;
            SetInstVarItems *ps = (SetInstVarItems*)malloc(sizeof(SetInstVarItems));
            if( (!ps) || (!yyvsp[-4].string) ) {
                if(ps) free(ps);
                pVar = yyvsp[-2].variable;
                while(pVar) {
                    yyvsp[-2].variable = pVar->next;
                    if(pVar->value) {
                        VariableItem *pVal = pVar->value;
                        while(pVal) {
                            pVar->value = pVar->value->next;
                            free(pVal);
                            pVal = pVar->value;
                        }
                    }
                    free(pVar);
                    pVar = yyvsp[-2].variable;
                }
                yyerror("out of memory");
                return 1;
            }
            ps->next = 0;
            ps->Inst_name = yyvsp[-4].string;
            ps->Inst_var = yyvsp[-2].variable;

            yyval.setinstvar=ps;
            ps = 0;
            break;}
        case 36:
        {
            yyval.variable = 0;
            break;}
        case 37:
        {
            yyval.variable = yyvsp[-2].variable;
            break;}
        case 38:
        {
            yyval.variable = 0;
            break;}
        case 39:
        {
            yyval.variable = yyvsp[0].variable;
            break;}
        case 40:
        {
            yyval.variable = yyvsp[0].variable;
            break;}
        case 41:
        {
            Variables* pVarList = yyvsp[-1].variable;

            if(pVarList) {

                while ( pVarList->next)
                    pVarList = pVarList->next;
                pVarList->next = yyvsp[0].variable;
                yyval.variable=yyvsp[-1].variable;
            } else {

                yyval.variable = yyvsp[0].variable;
            }
            break;}
        case 42:
        {
            yyval.variable = yyvsp[0].variable;
            break;}
        case 43:
        {
            yyval.variable = yyvsp[0].variable;
            break;}
        case 44:
        {
            Variables*                 pvar;

            if(yyvsp[-5].pt) {

                pvar = (Variables*)malloc(sizeof(Variables));
                if( (!pvar) || (!yyvsp[-7].string) ) {
                    if(pvar) free(pvar);
                    free(yyvsp[-2].var_item);
                    yyerror("out of memory");
                    return 1;
                }
                pvar->next = 0;
                pvar->var_name = yyvsp[-7].string;
                pvar->len = 1;
                pvar->value = yyvsp[-2].var_item;
                pvar->port_typ = yyvsp[-5].pt;
                pvar->var_typ = yyvsp[-4].datatype;
                if(yyvsp[-1].string) {
                    pvar->state =  (int)((int) atol(yyvsp[-1].string));
                } else {
                    pvar->state = 0;
                }
                pvar->vector = 0;

                switch(yyvsp[-4].datatype) {        
                case ((KS_VAR_TYPE)0x02):
                    if(pvar->value->value_type != DT_BOOLIAN) {
                        yyerror("Bad variable type");
                        return 1;
                    }
                    break;
                case ((KS_VAR_TYPE)0x10):
                    if(pvar->value->value_type != DT_GANZZAHL) {
                        yyerror("Bad variable type");
                        return 1;
                    }
                    break;
                case ((KS_VAR_TYPE)0x11):
                    if(pvar->value->value_type != DT_GANZZAHL) {
                        yyerror("Bad variable type");
                        return 1;
                    }
                    break;
                case ((KS_VAR_TYPE)0x20):
                    if( (pvar->value->value_type != DT_FLIESSCOMMA) &&
                        (pvar->value->value_type != DT_GANZZAHL) ) {
                        yyerror("Bad variable type");
                        return 1;
                    }
                    break;
                case ((KS_VAR_TYPE)0x21):
                    if( (pvar->value->value_type != DT_FLIESSCOMMA) &&
                        (pvar->value->value_type != DT_GANZZAHL) ) {
                        yyerror("Bad variable type");
                        return 1;
                    }
                    break;
                case ((KS_VAR_TYPE)0x31):
                    if(pvar->value->value_type != DT_TIMESTRUCT) {
                        yyerror("Bad variable type");
                        return 1;
                    }
                    break;
                case ((KS_VAR_TYPE)0x32):
                    if(pvar->value->value_type != DT_FLIESSCOMMA) {
                        yyerror("Bad variable type");
                        return 1;
                    }
                    break;
                case ((KS_VAR_TYPE)0x30):
                    if(pvar->value->value_type != DT_ZEICHEN) {
                        yyerror("Bad variable type");
                        return 1;
                    }
                    break;
                default:
                    yyerror("Unknown variable type");
                    return 1;
                }

                pvar->value->value_type = (DataType)yyvsp[-4].datatype;

                yyval.variable = pvar;

            } else {

                free(yyvsp[-2].var_item);
                yyval.variable = 0;
            }
            break;}
        case 45:
        {
            yyval.string = 0;
            break;}
        case 46:
        {
            if(!yyvsp[0].string) {
                yyerror("out of memory");
                return 1;
            }
            yyval.string = yyvsp[0].string;
            break;}
        case 47:
        {
            Variables*                 pvar;
            VariableItem*         pvar_item;
            int             count;

            if(yyvsp[-4].pt) {

                pvar = (Variables*)malloc(sizeof(Variables));
                if(( !pvar) || (!yyvsp[-9].string) || (!yyvsp[-7].string) ) {
                    if(pvar) free(pvar);
                    pvar_item = yyvsp[-1].var_item;
                    while(pvar_item) {
                        yyvsp[-1].var_item = pvar_item->next;
                        free(pvar_item);
                        pvar_item = yyvsp[-1].var_item;
                    }
                    yyerror("out of memory");
                    return 1;
                }

                count = (int)((int) atol(yyvsp[-7].string));

                pvar->next = 0;
                pvar->var_name = yyvsp[-9].string;
                pvar->len = count;
                pvar->value = yyvsp[-1].var_item;
                pvar->port_typ = yyvsp[-4].pt;
                pvar->var_typ = yyvsp[-3].datatype;
                pvar->state = 0;
                pvar->vector = 1;


                if(pvar->value) {
                    count = 0;
                    pvar_item = pvar->value;

                    while(pvar_item) {
                        switch(pvar->var_typ) {        
                        case ((KS_VAR_TYPE)0x02):
                            if(pvar_item->value_type != DT_BOOLIAN) {
                                yyerror("Bad variable type");
                                return 1;
                            }
                            break;
                        case ((KS_VAR_TYPE)0x10):
                            if(pvar_item->value_type != DT_GANZZAHL) {
                                yyerror("Bad variable type");
                                return 1;
                            }
                            break;
                        case ((KS_VAR_TYPE)0x20):
                            if( (pvar_item->value_type != DT_FLIESSCOMMA) &&
                                (pvar->value->value_type != DT_GANZZAHL) ) {
                                yyerror("Bad variable type");
                                return 1;
                            }
                            break;
                        case ((KS_VAR_TYPE)0x21):
                            if( (pvar_item->value_type != DT_FLIESSCOMMA) &&
                                (pvar->value->value_type != DT_GANZZAHL) ) {
                                yyerror("Bad variable type");
                                return 1;
                            }
                            break;
                        case ((KS_VAR_TYPE)0x31):
                            if(pvar_item->value_type != DT_TIMESTRUCT) {
                                yyerror("Bad variable type");
                                return 1;
                            }
                            break;
                        case ((KS_VAR_TYPE)0x32):
                            if(pvar_item->value_type != DT_FLIESSCOMMA) {
                                yyerror("Bad variable type");
                                return 1;
                            }
                        break;
                        case ((KS_VAR_TYPE)0x30):
                            if(pvar_item->value_type != DT_ZEICHEN) {
                                yyerror("Bad variable type");
                                return 1;
                            }
                            break;
                        default:
                            yyerror("Unknown variable type");
                            return 1;
                        }
                        pvar_item->value_type = (DataType)yyvsp[-3].datatype;
                        pvar_item = pvar_item->next;
                        count++;
                    } 
                }   

                if(count > pvar->len) {
                    yyerror("Too many arguments for initialization.");
                    return 1;
                }
                if(count < pvar->len) {
                    yyerror("Too little arguments for initialization.");
                    return 1;
                }

                yyval.variable = pvar;

            } else {

                pvar_item = yyvsp[-1].var_item;
                while(pvar_item) {
                    yyvsp[-1].var_item = pvar_item->next;
                    free(pvar_item);
                    pvar_item = yyvsp[-1].var_item;
                }
                yyval.variable = 0;
            }
            break;}
        case 48:
        {
            yyval.pt = PARAM_PORT;
            break;}
        case 49:
        {
            yyval.pt = INPUT_PORT;
            break;}
        case 50:
        {
            yyval.pt = DUMMY_PORT;
            break;}
        case 51:
        {
            yyval.var_item = yyvsp[-1].var_item;
            break;}
        case 52:
        {
            yyval.var_item = 0;
            break;}
        case 53:
        {
            yyval.var_item = yyvsp[0].var_item;
            break;}
        case 54:
        { 
            VariableItem* pvar_item = yyvsp[-2].var_item;

            while(pvar_item->next) {
                pvar_item=pvar_item->next;
            }
            pvar_item->next = yyvsp[0].var_item;
            yyval.var_item = yyvsp[-2].var_item;
            break;}
        case 55:
        {
            VariableItem* pvar_item = (VariableItem*)malloc(sizeof(VariableItem));
            if( (!pvar_item) || (!yyvsp[0].string) ) {
                if(pvar_item) free(pvar_item);
                yyerror("out of memory");
                return 1;
            }
            pvar_item->next = 0;
            pvar_item->value_type = DT_FLIESSCOMMA;
            pvar_item->val = yyvsp[0].string;

            yyval.var_item = pvar_item;
            break;}
        case 56:
        {
            VariableItem* pvar_item = (VariableItem*)malloc(sizeof(VariableItem));
            if((!pvar_item) || (!yyvsp[0].string) ) {
                if(pvar_item) free(pvar_item);
                yyerror("out of memory");
                return 1;
            }
            pvar_item->next = 0;
            pvar_item->value_type = DT_GANZZAHL;
            pvar_item->val = yyvsp[0].string;

            yyval.var_item = pvar_item;
            break;}
        case 57:
        {
            VariableItem* pvar_item = (VariableItem*)malloc(sizeof(VariableItem));
            if((!pvar_item) || (!yyvsp[0].string) ) {
                if(pvar_item) free(pvar_item);
                yyerror("out of memory");
                return 1;
            }
            pvar_item->next = 0;
            pvar_item->value_type = DT_BOOLIAN;
            pvar_item->val = yyvsp[0].string;

            yyval.var_item = pvar_item;
            break;}
        case 58:
        {
            VariableItem* pvar_item = (VariableItem*)malloc(sizeof(VariableItem));
            if((!pvar_item) || (!yyvsp[0].string) ) {
                if(pvar_item) free(pvar_item);
                yyerror("out of memory");
                return 1;
            }
            pvar_item->next = 0;
            pvar_item->value_type = DT_ZEICHEN;

            pvar_item->val = yyvsp[0].string;
            yyval.var_item = pvar_item;
            break;}
        case 59:
        {
            VariableItem* pvar_item = (VariableItem*)malloc(sizeof(VariableItem));
            if((!pvar_item) || (!yyvsp[0].string) ) {
                if(pvar_item) free(pvar_item);
                yyerror("out of memory");
                return 1;
            }
            pvar_item->next = 0;
            pvar_item->value_type = DT_TIMESTRUCT;
            pvar_item->val = yyvsp[0].string;

            yyval.var_item = pvar_item;
            break;}
    }


    yyvsp -= yylen;
    yyssp -= yylen;

    *++yyvsp = yyval;
    
    yyn = yyr1[yyn];

    yystate = yypgoto[yyn - 44] + *yyssp;
    if (yystate >= 0 && yystate <= 181 && yycheck[yystate] == *yyssp)
        yystate = yytable[yystate];
    else
        yystate = yydefgoto[yyn - 44];

    goto yynewstate;

yyerrlab:   
    
    if (! yyerrstatus)
    {
        ++yynerrs;
        yyerror("parse error");
    }

    goto yyerrlab1;
yyerrlab1:   

    if (yyerrstatus == 3)
    {
        if (yychar == 0)
            return(1);
        yychar = -2;
    }

    yyerrstatus = 3;                
    goto yyerrhandle;

yyerrdefault:  

yyerrpop:   

    if (yyssp == yyss) return(1);
    yyvsp--;
    yystate = *--yyssp;
 
yyerrhandle:

    yyn = yypact[yystate];
    if (yyn == -32768)
        goto yyerrdefault;

    yyn += 1;
    if (yyn < 0 || yyn > 181 || yycheck[yyn] != 1)
        goto yyerrdefault;

    yyn = yytable[yyn];
    if (yyn < 0)
    {
        if (yyn == -32768)
            goto yyerrpop;
        yyn = -yyn;
        goto yyreduce;
    }
    else if (yyn == 0)
            goto yyerrpop;

    if (yyn == 149)
        return(0);

    *++yyvsp = yylval;

    yystate = yyn;
    goto yynewstate;
}
