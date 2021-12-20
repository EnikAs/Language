#include "frontend.h"

static void    CheckPtr    (void* ptr, const char* error);

static void    space_skip  (buffer* buf);

static int     get_bracket (buffer* buf, tkn_arr* tk_array);

static int     get_op      (buffer* buf, tkn_arr* tk_array);

static int     get_num     (buffer* buf, tkn_arr* tk_array);

static int     get_word    (buffer* buf, tkn_arr* tk_array);

static int     isnumber    (char value);

static int     isoperator  (char value);

static int     isbracket   (char value);

static int     iscomma     (char value);


//TODO: function for VALUE_IS_OPERATOR (OR NOT)

#define $VALUE_IS_OPERATOR (value == '-' || value == '+' || value == '*' || value == '/' || value == '^' || value == '=' || value == '<' || value == '>' || value == ';'|| value == '!')

#define $BUF_CUR_ELEM buf->data[buf->pos]

#define $CUR_TKN_DATA_LNG tk_array->node[tk_array->current_node].data_lng 
#define $CUR_TKN_DATA_STR tk_array->node[tk_array->current_node].data.str
#define $CUR_TKN_DATA_DBL tk_array->node[tk_array->current_node].data.dbl
#define $CUR_TKN_DATA_CHR tk_array->node[tk_array->current_node].data.ch
#define $CUR_TKN_DATA_TYP tk_array->node[tk_array->current_node].data_type

#define $CURRENT_TOKEN tk_array->node[tk_array->current_node]
#define $CURRENT tk_array->current_node
#define $300$ printf("I was in function %s\n", __func__);

#define $Require(ch) Require(ch, tk_array)

//==========================================================================
void CheckPtr(void* ptr, const char* error)
{
    if (ptr == NULL)
    {
        printf("%s", error);
        assert(0);
    }
}
//==========================================================================
tkn_arr* GetAllTokens(FILE* inputfile)
{
    assert(inputfile);

    buffer* buf = (buffer*) calloc(1 + KOSTYL, sizeof(buffer)); 
    CheckPtr(buf, "buffer callocation error!");

    buf->pos = 0;
    buf->size = scanf_file_size(inputfile);
    printf("%d ", buf->size);
    buf->data = (char*) calloc(buf->size + 1000, sizeof(char));
    CheckPtr(buf->data, "Buffer callocation error!");

    fread(buf->data, sizeof(char), buf->size, inputfile);

    tkn_arr* tk_array = (tkn_arr*) calloc(1, sizeof(tkn_arr));
    CheckPtr(tk_array, "token tk_array callocation error!");

    tk_array->node = (Node*) calloc(MAX_TKN_CUNT, sizeof(Node));

    GetTokens(buf, tk_array);

    return tk_array;
}
//==========================================================================
void GetTokens (buffer* buf, tkn_arr* tk_array)
{
    while( $BUF_CUR_ELEM != '$')
    {
        if      (isalpha($BUF_CUR_ELEM))
        {
            $CUR_TKN_DATA_LNG = get_word(buf, tk_array);
            $CURRENT += 1;

            tk_array->n_cunt += 1;
        }
        else if (isnumber($BUF_CUR_ELEM))
        {
            $CUR_TKN_DATA_LNG = get_num(buf, tk_array);
            $CURRENT += 1;

            tk_array->n_cunt += 1;
        }
        else if (isoperator($BUF_CUR_ELEM))
        {
            $CUR_TKN_DATA_LNG = get_op(buf, tk_array);
            $CURRENT += 1;

            tk_array->n_cunt += 1;
        }
        else if (isbracket($BUF_CUR_ELEM))
        {
            $CUR_TKN_DATA_LNG = get_bracket(buf, tk_array);
            $CURRENT += 1;

            tk_array->n_cunt += 1;
        }
        else if (iscomma($BUF_CUR_ELEM))
        {
            $CUR_TKN_DATA_LNG = 1;
            $CUR_TKN_DATA_CHR = ',';
            $CUR_TKN_DATA_TYP = COMMA;

            $CURRENT += 1;
            
            buf->pos += 1;

            tk_array->n_cunt += 1;
        }
        else if (isspace($BUF_CUR_ELEM))
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
//==========================================================================
void    space_skip  (buffer* buf)
{
    while(isspace($BUF_CUR_ELEM))
    {
        buf->pos += 1;
    }
}
//==========================================================================
int     get_bracket (buffer* buf, tkn_arr* tk_array)
{
    $CUR_TKN_DATA_CHR = $BUF_CUR_ELEM;

    buf->pos += 1;

    $CUR_TKN_DATA_TYP = BRACKET;

    return 1;
}
//==========================================================================
int     get_op      (buffer* buf, tkn_arr* tk_array)
{
    int cunt = 0;
    char* op = NULL;
    op = &($BUF_CUR_ELEM);

    $CUR_TKN_DATA_CHR = $BUF_CUR_ELEM;

    while(isoperator($BUF_CUR_ELEM))
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
//==========================================================================
int     get_num     (buffer* buf, tkn_arr* tk_array)
{
    int cunt = 0;
    double tmp_dbl = 0;

    while(isnumber($BUF_CUR_ELEM))
    {
        cunt += 1;

        tmp_dbl *= 10;
        tmp_dbl += $BUF_CUR_ELEM - '0';

        buf->pos += 1;
    }

    $CUR_TKN_DATA_DBL = tmp_dbl;

    $CUR_TKN_DATA_TYP = CONSTANT;

    return cunt;
}
//==========================================================================
int     get_word    (buffer* buf, tkn_arr* tk_array)
{
    int cunt = 0;

    $CUR_TKN_DATA_STR = &($BUF_CUR_ELEM);
    
    while (isalpha($BUF_CUR_ELEM) || $BUF_CUR_ELEM == '_')
    {
        cunt += 1;
        buf->pos += 1;
    }

    if (strncmp($CUR_TKN_DATA_STR, "for_weak_people", 15) == EQUAL)
        {
            $CUR_TKN_DATA_CHR = '}';
            $CUR_TKN_DATA_TYP = BRACKET;
            cunt = 1;

            return cunt;
        }
    
    else if (strncmp($CUR_TKN_DATA_STR, "word_stress", 11) == EQUAL)
        {
            $CUR_TKN_DATA_CHR = '{';
            $CUR_TKN_DATA_TYP = BRACKET;
            cunt = 1;
            
            return cunt;
        }

    else if (strncmp($CUR_TKN_DATA_STR, "tortbl", 6) == EQUAL)
        $CUR_TKN_DATA_TYP = IF;

    else if (strncmp($CUR_TKN_DATA_STR, "krasivEe", 8) == EQUAL)
        $CUR_TKN_DATA_TYP = WHILE;
    
    else if (strncmp($CUR_TKN_DATA_STR, "lattE", 5) == EQUAL)
        $CUR_TKN_DATA_TYP = ELSE;

    else if (strncmp($CUR_TKN_DATA_STR, "zvOnit", 6) == EQUAL)
        $CUR_TKN_DATA_TYP = RETURN;

    else
        $CUR_TKN_DATA_TYP = VARIABLE;

    return cunt;
}
//==========================================================================
int     isnumber    (char value)
{
    if (value <= '9' && value >= '0')
        return 1;
    
    else 
        return 0;
}
//==========================================================================
int     isoperator  (char value)
{
    if ($VALUE_IS_OPERATOR)
        return 1;

    else
        return 0;

}
//==========================================================================
int     isbracket   (char value)
{
    if (value == '(' || value == ')' || value == '{' || value == '}')
        return 1;

    else
        return 0;
}
//==========================================================================
int     iscomma (char value)
{
    if (value == ',')
    {
        return 1;
    }
    else 
        return 0;
}
//==========================================================================
void PrintAllTokens (tkn_arr* tk_array)
{
    for(int i = 0 ; i < tk_array->n_cunt ; i++)
    {
        assert(i < tk_array->n_cunt);
        assert(i >= 0);

        if      (tk_array->node[i].data_type == CONSTANT)
            printf("%d) data = %.1lf || type = CONSTANT\n", i, tk_array->node[i].data.dbl);

        else if (tk_array->node[i].data_type == IF)
            printf("%d) data = %.*s || type = KEYWORD\n", i, tk_array->node[i].data_lng, tk_array->node[i].data.str);
        
        else if (tk_array->node[i].data_type == WHILE)
            printf("%d) data = %.*s || type = KEYWORD\n", i, tk_array->node[i].data_lng, tk_array->node[i].data.str);
        
        else if (tk_array->node[i].data_type == ELSE)
            printf("%d) data = %.*s || type = KEYWORD\n", i, tk_array->node[i].data_lng, tk_array->node[i].data.str);
        
        else if (tk_array->node[i].data_type == RETURN)
            printf("%d) data = %.*s || type = KEYWORD\n", i, tk_array->node[i].data_lng, tk_array->node[i].data.str);
        
        else if (tk_array->node[i].data_type == OPERATOR)
            printf("%d) data = %c || type = OPERATOR\n", i, tk_array->node[i].data.ch);

        else if (tk_array->node[i].data_type == COMMA)
            printf("%d) data = %c || type = COMMA\n", i, tk_array->node[i].data.ch);

        else if (tk_array->node[i].data_type == REL_OPERATOR)
            printf("%d) data = %.*s || type = REL_OPERATOR\n", i, tk_array->node[i].data_lng, tk_array->node[i].data.str);

        else if (tk_array->node[i].data_type == VARIABLE)
            printf("%d) data = %.*s || type = VARIABLE\n", i, tk_array->node[i].data_lng, tk_array->node[i].data.str);

        else if (tk_array->node[i].data_type == BRACKET)
            printf("%d) data = %c || type = BRACKET\n", i, tk_array->node[i].data.ch);

        else if (tk_array->node[i].data_type == DOLLAR)
            printf("%d) data = %c || type = DOLLAR\n", i, tk_array->node[i].data.ch);
    }
}
//==========================================================================
Node* GetG (tkn_arr* tk_array)
{$300$
    tk_array->current_node = 0;

    Tree* tree = (Tree*) calloc(1, sizeof(Tree));
    
    tree->peak = GetStmts(tk_array);$300$
        
    if ($CUR_TKN_DATA_CHR == '$')printf("IT IS THE END!!!");
    
    else
    {   
        printf("It is not '$', it is %c", $CUR_TKN_DATA_CHR);
        SyntaxERROR(__func__);
    }
    if (tree->peak == NULL)
    {
        assert(ERRORR && "Something goes wrong, tree->peak = NULL");
    }   
    else 
    {
        printf("Correct end of GetG !\n");
    }

    return tree->peak; 
}
//==========================================================================
Node* GetStmts (tkn_arr* tk_array)
{$300$
    if ($CUR_TKN_DATA_CHR == '$' || $CUR_TKN_DATA_CHR == '}')
    {
        return NULL;
    }
    else 
    {
        Node* stmt_node = CreateNode(STATEMENT);
		
		Node* stmt  = GetStmt(tk_array);$300$
		Node* stmts_after = GetStmts(tk_array);$300$

        stmt_node->left  = stmts_after;
        stmt_node->right = stmt;

		return stmt_node;
    }
    assert(0);
    return NULL;
}
//==========================================================================
Node* GetStmt (tkn_arr* tk_array)
{$300$
    switch($CUR_TKN_DATA_TYP)
    {
        case VARIABLE:
        {
            int start_current = $CURRENT;
            $CURRENT += 1;
            if ($CUR_TKN_DATA_CHR == '(')
            {
                while ($CUR_TKN_DATA_CHR != ')')
                    $CURRENT += 1;

                $CURRENT += 1;

                if ($CUR_TKN_DATA_CHR == '{')
                {
                    $CURRENT = start_current;
                    Node* define_node = CreateNode(DEFINE);
                    Node* func_node = CreateNode(FUNCTION);

                    define_node->left = func_node;

                    func_node->left = &($CURRENT_TOKEN);
                    
                    $CURRENT += 1;
                    $Require('(');
                    func_node->right = GetArgs(tk_array);$300$
                    $Require(')');

                    $Require('{');
                    define_node->right = GetStmts(tk_array);$300$
                    $Require('}');

                    return define_node;
                }
                else 
                {
                    $CURRENT = start_current;

                    Node* call_node = CreateNode(CALL);

                    call_node->left = &($CURRENT_TOKEN);

                    $CURRENT += 1;

                    $Require('(');
                    call_node->right = GetArgsCall(tk_array);$300$
                    $Require(')');
                    $Require(';');

                    return call_node;
                    //$CURRENT = start_current;
                }
                    
            }
            else 
                $CURRENT = start_current;
        }
        case CONSTANT:
        {
            Node* left_part_stmt = GetE(tk_array);$300$

            Node* op_node = &($CURRENT_TOKEN);

            if ($CUR_TKN_DATA_CHR != '=' && $CUR_TKN_DATA_TYP != REL_OPERATOR)
                SyntaxERROR(__func__);

            $CURRENT += 1;

            op_node->right = GetE(tk_array);$300$
            op_node->left = left_part_stmt;

            if ($CUR_TKN_DATA_CHR == ')');
            else
                $Require(';');

            return op_node;
            break;
        }
        case IF:
        {
            Node* if_node = &($CURRENT_TOKEN);
            $CURRENT += 1;

            $Require('(');
            if_node->left = GetStmt(tk_array);$300$
            $Require(')');

            $Require('{');
            Node* desision = CreateNode(DESISION);
            if_node->right = desision;
            desision->left = GetStmts(tk_array);$300$
            $Require('}');

            if ($CUR_TKN_DATA_TYP == ELSE)           
            {
                $CURRENT += 1;

                $Require('{');
                desision->right = GetStmts(tk_array);$300$
                $Require('}');
            }

            return if_node;
            break;
        }
        case WHILE:
        { 
            Node* while_node = &($CURRENT_TOKEN);

            $CURRENT += 1;

            $Require('(');
            while_node->left = GetStmt(tk_array);$300$
            $Require(')');

            $Require('{');
            while_node->right = GetStmts(tk_array);$300$
            $Require('}');

            return while_node;
            break;
        }
        case RETURN:
        {
            Node* return_node = &($CURRENT_TOKEN);
            $CURRENT += 1;
            return_node->left = GetE(tk_array);$300$
            $Require(';');

            return return_node;
        }
        default:
        {
            assert(0);
            return 0;
        }
    }
    assert(0);
    return NULL;
}
//==========================================================================
Node* GetE (tkn_arr* tk_array)
{$300$
    Node* val = GetT(tk_array);$300$
    
    Node* op = val;
    Node* old_op = val;

    while($CUR_TKN_DATA_TYP == OPERATOR && ($CUR_TKN_DATA_CHR == '+' || $CUR_TKN_DATA_CHR == '-'))
    {
        old_op = op;
        op = &($CURRENT_TOKEN);

        $CURRENT += 1;
        
        Node* val2 = GetT(tk_array);$300$
        
        
        op->left = old_op; // TODO bind_l_r(old_op, op, val2)
        
        op->right = val2;
    }
    
    return op;
}
//==========================================================================
Node* GetT (tkn_arr* tk_array)
{$300$
    Node* val = GetP(tk_array);
    $300$
    Node* op = val;
    Node* old_op = val;

    printf("In func GetT char = %c\n", $CUR_TKN_DATA_CHR);
    while($CUR_TKN_DATA_TYP == OPERATOR && ($CUR_TKN_DATA_CHR == '*' || $CUR_TKN_DATA_CHR == '/'))
    {
        printf("It is * |||\n");
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
//==========================================================================
Node* GetP (tkn_arr* tk_array)
{$300$
    if ($CUR_TKN_DATA_CHR == '(')
    {
        printf("skobka ");
        $CURRENT += 1;
        printf("after skobka skip %c\n", $CUR_TKN_DATA_CHR);
        Node* val = GetE(tk_array);$300$
        $Require(')');

        printf("\tAftr skobki it is %c\n", $CUR_TKN_DATA_CHR);

        return val;
    }
    else 
    {
        //printf("In func GetP there is a synt error, %c %lf\n", $CUR_TKN_DATA_CHR, $CUR_TKN_DATA_DBL);
        Node* val2 = GetN(tk_array);$300$
        
        return val2;
    }
}
//==========================================================================
Node* GetN (tkn_arr* tk_array)
{$300$
    Node* val = NULL;

    if ($CUR_TKN_DATA_TYP == DOLLAR)
        return val;

    if ($CUR_TKN_DATA_TYP == CONSTANT)
    {
        val = &($CURRENT_TOKEN);
        $CURRENT += 1;
    }

    else if ($CUR_TKN_DATA_TYP == VARIABLE)
    {
        val = GetV(tk_array);$300$
    }
    
    else
    {
        printf("In func GetN there is a synt error, %c %lf\n", $CUR_TKN_DATA_CHR, $CUR_TKN_DATA_DBL);
        SyntaxERROR(__func__);
    }

    return val;
}
//==========================================================================
Node* GetV (tkn_arr* tk_array)
{$300$
    Node* val = &($CURRENT_TOKEN);

    $CURRENT += 1;

    if ($CUR_TKN_DATA_CHR == '(')
    {
        printf("it is function)))\n");
        $CURRENT += 1;

        Node* call_node = CreateNode(CALL);

        call_node->left = val;

        if ($CUR_TKN_DATA_CHR == ')')
        {
            printf("but without args!\n");
            $CURRENT += 1;

            return call_node;
        }

        else
        {
            call_node->right = GetArgs(tk_array);$300$
            $Require(')');
            return call_node;
        }
    }
    
    return val;
}
//==========================================================================
Node* GetArgs (tkn_arr* tk_array)
{$300$
    if ($CUR_TKN_DATA_CHR == ')')
    {
        return NULL;
    }

    else
    {
        if ($CUR_TKN_DATA_CHR == ',')
            $CURRENT += 1;
        
        Node* param_node = CreateNode(PARAMETER);
		
		Node* param  = GetArg(tk_array);$300$
		Node* params_after = GetArgs(tk_array);$300$

        param_node->left  = params_after;
        param_node->right = param;

		return param_node;
    }

}
//==========================================================================
Node* GetArgsCall (tkn_arr* tk_array)
{$300$
    if ($CUR_TKN_DATA_CHR == ')')
    {
        return NULL;
    }

    else
    {
        if ($CUR_TKN_DATA_CHR == ',')
            $CURRENT += 1;
        
        Node* param_node = CreateNode(PARAMETER_CALL);
		
		Node* param  = GetArg(tk_array);$300$
		Node* params_after = GetArgsCall(tk_array);$300$

        param_node->left  = params_after;
        param_node->right = param;

		return param_node;
    }

}
//==========================================================================
Node* GetArg (tkn_arr* tk_array)
{$300$
    Node* var = GetE(tk_array);$300$
    
    return var;
}
//==========================================================================
[[noreturn]] void SyntaxERROR (const char* s)
{
    printf("Ha ha oshibsya in function %s\n", s);
    assert(0);
}
//==========================================================================
void Require (char ch, tkn_arr* tk_array)
{
    if ($CUR_TKN_DATA_CHR == ch)
    {
        $CURRENT += 1;
    }
    else
    {
        printf("Incorrect input, expected '%c' but have '%c'\n", ch, $CUR_TKN_DATA_CHR);
        assert(0);
    }
}
//==========================================================================
Node* CreateNode (int node_type)
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    node->data_type = node_type;
    
    return node;
}
//==========================================================================
#undef $VALUE_IS_OPERATOR

#undef $BUF_CUR_ELEM

#undef $CUR_TKN_DATA_LNG
#undef $CUR_TKN_DATA_STR
#undef $CUR_TKN_DATA_DBL
#undef $CUR_TKN_DATA_CHR
#undef $CUR_TKN_DATA_TYP

#undef $CURRENT_TOKEN
#undef $CURRENT
#undef $300$

#undef $Require

// zvOnit = retuen
// krasivEe = while
// tortbl = if
//  
//      
//  
//  
//  
//  