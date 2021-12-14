#include "lang.h"

static void    CheckPtr    (void* ptr, const char* error);

static void    space_skip  (buffer* buf);

static int     get_bracket (buffer* buf, tkn_arr* tk_array);

static int     get_op      (buffer* buf, tkn_arr* tk_array);

static int     get_num     (buffer* buf, tkn_arr* tk_array);

static int     get_word    (buffer* buf, tkn_arr* tk_array);

static int     isnumber    (char value);

static int     isoperator  (char value);

static int     isbracket   (char value);

//TODO: function for VALUE_IS_OPERATOR (OR NOT)

#define $VALUE_IS_OPERATOR (value == '-' || value == '+' || value == '*' || value == '/' || value == '^' || value == '=' || value == '<' || value == '>' || value == ';'|| value == '!')

#define $BUF_ELEM buf->data[buf->pos]

#define $CUR_TKN_DATA_LNG tk_array->node[tk_array->current_node].data_lng 
#define $CUR_TKN_DATA_STR tk_array->node[tk_array->current_node].data.str
#define $CUR_TKN_DATA_DBL tk_array->node[tk_array->current_node].data.dbl
#define $CUR_TKN_DATA_CHR tk_array->node[tk_array->current_node].data.ch
#define $CUR_TKN_DATA_TYP tk_array->node[tk_array->current_node].data_type

#define $CURRENT_TOKEN tk_array->node[tk_array->current_node]
#define $CURRENT tk_array->current_node

#define $300$ printf("I was in function %s\n", __func__);

void CheckPtr(void* ptr, const char* error)
{
    if (ptr == NULL)
    {
        printf("%s", error);
        assert(0);
    }
}

tkn_arr* GetAllTokens(FILE* inputfile)
{
    assert(inputfile);

    buffer* buf = (buffer*) calloc(1, sizeof(buffer)); 
    CheckPtr(buf, "buffer callocation error!");

    buf->pos = 0;
    buf->size = scanf_file_size(inputfile);

    buf->data = (char*) calloc(buf->size, sizeof(char));
    CheckPtr(buf->data, "Buffer callocation error!");

    fread(buf->data, sizeof(char), buf->size, inputfile);

    tkn_arr* tk_array = (tkn_arr*) calloc(1, sizeof(tkn_arr));
    CheckPtr(tk_array, "token tk_array callocation error!");

    tk_array->node = (Node*) calloc(MAX_TKN_CUNT, sizeof(Node));

    GetTokens(buf, tk_array);

    return tk_array;
}

void GetTokens (buffer* buf, tkn_arr* tk_array)
{
    while( $BUF_ELEM != '$')
    {
        if      (isalpha($BUF_ELEM))
        {
            $CUR_TKN_DATA_LNG = get_word(buf, tk_array);
            tk_array->n_cunt += 1;
            tk_array->current_node += 1;
        }
        else if (isnumber($BUF_ELEM))
        {
            $CUR_TKN_DATA_LNG = get_num(buf, tk_array);
            tk_array->n_cunt += 1;
            tk_array->current_node += 1;
        }
        else if (isoperator($BUF_ELEM))
        {
            $CUR_TKN_DATA_LNG = get_op(buf, tk_array);
            tk_array->n_cunt += 1;
            tk_array->current_node += 1;
        }
        else if (isbracket($BUF_ELEM))
        {
            $CUR_TKN_DATA_LNG = get_bracket(buf, tk_array);
            tk_array->n_cunt += 1;
            tk_array->current_node += 1;
        }
        else if (isspace($BUF_ELEM))
        {
            space_skip(buf);
        }
        else 
            assert(0 && "INCORRECT INPUT bitch!");
    }

    $CUR_TKN_DATA_CHR = '$';
    $CUR_TKN_DATA_TYP = DOLLAR;
    tk_array->n_cunt += 1;

}

void    space_skip  (buffer* buf)
{
    while(isspace($BUF_ELEM))
    {
        buf->pos += 1;
    }
}

int     get_bracket (buffer* buf, tkn_arr* tk_array)
{
    $CUR_TKN_DATA_CHR = $BUF_ELEM;

    buf->pos += 1;

    $CUR_TKN_DATA_TYP = BRACKET;

    return 1;
}

int     get_op      (buffer* buf, tkn_arr* tk_array)
{
    int cunt = 0;
    char* op = NULL;
    op = &($BUF_ELEM);

    $CUR_TKN_DATA_CHR = $BUF_ELEM;

    while(isoperator($BUF_ELEM))
    {
        cunt += 1;
        buf->pos += 1;
    }

    if (cunt > 1 || $CUR_TKN_DATA_CHR == '<' || $CUR_TKN_DATA_CHR == '>')
    {
        $CUR_TKN_DATA_TYP = REL_OPERATOR;
        $CUR_TKN_DATA_STR = op;
    }

    else
        $CUR_TKN_DATA_TYP = OPERATOR;

    return cunt;
}

int     get_num     (buffer* buf, tkn_arr* tk_array)
{
    int cunt = 0;
    double tmp_dbl = 0;

    while(isnumber($BUF_ELEM))
    {
        cunt += 1;

        tmp_dbl *= 10;
        tmp_dbl += $BUF_ELEM - '0';

        buf->pos += 1;
    }

    $CUR_TKN_DATA_DBL = tmp_dbl;

    $CUR_TKN_DATA_TYP = CONSTANT;

    return cunt;
}

int     get_word    (buffer* buf, tkn_arr* tk_array)
{
    int cunt = 0;

    $CUR_TKN_DATA_STR = &($BUF_ELEM);
    
    while (isalpha($BUF_ELEM))
    {
        cunt += 1;
        buf->pos += 1;
    }

    if (strncmp($CUR_TKN_DATA_STR, "if", 2) == EQUAL)
        $CUR_TKN_DATA_TYP = IF;

    else if (strncmp($CUR_TKN_DATA_STR, "while", 5) == EQUAL)
        $CUR_TKN_DATA_TYP = WHILE;

    else
        $CUR_TKN_DATA_TYP = VARIABLE;

    return cunt;
}

int     isnumber    (char value)
{
    if (value <= '9' && value >= '0')
        return 1;
    
    else 
        return 0;
}

int     isoperator  (char value)
{
    if ($VALUE_IS_OPERATOR)
        return 1;

    else
        return 0;

}

int     isbracket   (char value)
{
    if (value == '(' || value == ')' || value == '{' || value == '}')
        return 1;

    else
        return 0;
}

void PrintAllTokens (tkn_arr* tk_array)
{
    for(int i = 0 ; i < tk_array->n_cunt ; i++)
    {
        if      (tk_array->node[i].data_type == CONSTANT)
            printf("%d) data = %.1lf || type = CONSTANT\n", i, tk_array->node[i].data.dbl);

        else if (tk_array->node[i].data_type == IF)
            printf("%d) data = %.*s || type = KEYWORD\n", i, tk_array->node[i].data_lng, tk_array->node[i].data.str);
        
        else if (tk_array->node[i].data_type == WHILE)
            printf("%d) data = %.*s || type = KEYWORD\n", i, tk_array->node[i].data_lng, tk_array->node[i].data.str);
        
        else if (tk_array->node[i].data_type == REL_OPERATOR)
            printf("%d) data = %.*s || type = OPERATOR\n", i, tk_array->node[i].data_lng, tk_array->node[i].data.str);

        else if (tk_array->node[i].data_type == VARIABLE)
            printf("%d) data = %.*s || type = VARIABLE\n", i, tk_array->node[i].data_lng, tk_array->node[i].data.str);

        else if (tk_array->node[i].data_type == BRACKET)
            printf("%d) data = %c || type = BRACKET\n", i, tk_array->node[i].data.ch);

        else if (tk_array->node[i].data_type == DOLLAR)
            printf("%d) data = %c || type = DOLLAR\n", i, tk_array->node[i].data.ch);
    }
}

Node* GetG (tkn_arr* tk_array)
{$300$
    tk_array->current_node = 0;

    Tree* tree = (Tree*) calloc(1, sizeof(Tree));
    
    tree->peak = GetE(tk_array);
    $300$    
    if ($CUR_TKN_DATA_CHR == '$') $CURRENT += 1;
    
    else
    {   
        SyntaxERROR(__func__);
    }
    printf("HRSH HRSH HRSH !!, %d!", tree->peak);

    return tree->peak; 
}
/*
Node* GetStmts (tkn_arr* tk_array)
{

}

Node* GetStmt (tkn_arr* tk_array)
{
    switch($CUR_TKN_DATA_TYP)
    {
        case VARIABLE:

            break;
        case IF:

            break;
        case WHILE:
        
            break;
    }
}
*/
Node* GetE (tkn_arr* tk_array)
{$300$
    Node* val = GetT(tk_array);
    $300$
    Node* op = val;
    Node* old_op = val;
    while($CUR_TKN_DATA_TYP == OPERATOR && ($CUR_TKN_DATA_CHR == '+' || $CUR_TKN_DATA_CHR == '-'))
    {
        old_op = op;
        op = &($CURRENT_TOKEN);

        $CURRENT += 1;
        
        Node* val2 = GetT(tk_array);
        $300$
        
        op->left = old_op; // TODO bind_l_r(old_op, op, val2)
        
        op->right = val2;
    }
    
    return op;
}

Node* GetT (tkn_arr* tk_array)
{$300$
    Node* val = GetP(tk_array);
    $300$
    Node* op = val;
    Node* old_op = val;

    while($CUR_TKN_DATA_TYP == OPERATOR && ($CUR_TKN_DATA_CHR == '*' || $CUR_TKN_DATA_CHR == '/'))
    {
        old_op = op;
        op = &($CURRENT_TOKEN);

        $CURRENT += 1;

        Node* val2 = GetP(tk_array);

        $300$
        op->left = old_op; // TODO bind_l_r(old_op, op, val2)
        op->right = val2;
    }
    
    
    return op;
}

Node* GetP (tkn_arr* tk_array)
{$300$
    if ($CUR_TKN_DATA_CHR == '(')
    {
        $CURRENT += 1;
        Node* val = GetE(tk_array);
        $300$
        $CURRENT += 1;
        return val;
    }
    else 
    {
        Node* val2 = GetN(tk_array);
        $300$
        return val2;
    }
}

Node* GetN (tkn_arr* tk_array)
{$300$
    Node* val = &($CURRENT_TOKEN);

    if ($CUR_TKN_DATA_TYP == CONSTANT)
        $CURRENT += 1;
    
    else if ($CUR_TKN_DATA_TYP == VARIABLE)
    {
        Node* val = GetV(tk_array);
    }
    
    else
    {
        SyntaxERROR(__func__);
    }

    return val;
}

Node* GetV (tkn_arr* tk_array)
{$300$
    Node* val = &($CURRENT_TOKEN);

    $CURRENT += 1;

    if ($CUR_TKN_DATA_TYP == BRACKET)
    ; // TODO function as variable !!!

    return val;
}

void SyntaxERROR (const char* s)
{$300$
    printf("Ha ha oshibsya in function %s\n", s);
    assert(0);
}
