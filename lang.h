#ifndef LANG_H_INCLUDED
#define LANG_H_INCLUDED

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cassert>
#include <sys\stat.h>

#include "tree.h"

const int MAX_TKN_CUNT = 100;

enum strncmp
{
    EQUAL = 0
};

struct tkn_arr
{
    Node* node = NULL;
    int n_cunt = 0;
    int current_node  = 0;
};

struct buffer
{
    char* data = NULL;
    int size = 0;
    int pos = 0;
};

tkn_arr*    GetAllTokens    (FILE* inputfile);

void        GetTokens       (buffer* buf, tkn_arr* array);

void        PrintAllTokens  (tkn_arr* array);

Node*       GetG            (tkn_arr* tk_array);

Node*       GetE            (tkn_arr* tk_array);

Node*       GetT            (tkn_arr* tk_array);

Node*       GetP            (tkn_arr* tk_array);

Node*       GetN            (tkn_arr* tk_array);

Node*       GetV            (tkn_arr* tk_array);

void        SyntaxERROR     (const char* s);

#endif