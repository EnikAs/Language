#include "backend.h"

#define $ND_CHR node->data.ch
#define $ND_DBL node->data.dbl
#define $ND_STR node->data.str
#define $ND_LNG node->data.lng
#define $ND_TYP node->data_type

#define $FREE_VAR vr_lists->var[vr_lists->free]
#define $FREE     vr_lists->free

#define $R node->right
#define $L node->left

#define $VISIT(node)                              \
if (node)                                         \
    VisitPrintCommands(node, vr_lists, com_file)  

#define $VISIT_NEW_LIST(node, NL)                 \
if (node)                                         \
    VisitPrintCommands(node, NL, com_file)        

#define $PRINT(...) fprintf(com_file, __VA_ARGS__)
//#define $PRINT(...) printf(__VA_ARGS__)

//==========================================================================
int VisitPrintCommands (Node* node, var_lists* vr_lists, FILE* com_file)
{
    switch($ND_TYP)
    {
        case STATEMENT:
        {
            $VISIT($R);
            $VISIT($L);

            break;
        }
        case OPERATOR:
        {
            if ($R->data_type == CONSTANT || $R->data_type == VARIABLE)
                $PRINT("PUSH ");
            $VISIT($R);

            if ($ND_CHR == EQL)
            {
                $PRINT("POP ");
                $VISIT($L);

                return 0;
            }

            if ($L->data_type == CONSTANT || $L->data_type == VARIABLE)
            $PRINT("PUSH ");
            $VISIT($L);

            switch ($ND_CHR)
            {
                case MUL:
                {
                    $PRINT("MUL\n");
                    break;
                }
                case SUB:
                {
                    $PRINT("SUB\n");
                    break;
                }
                case ADD:
                {
                    $PRINT("ADD\n");
                    break;
                }
                case DIV:
                {
                    $PRINT("DIV\n");
                    break;
                }
                case DEG:
                {
                    $PRINT("DEG\n");
                    break;
                }
                default: 
                {
                    assert(0 && "case operator incorrect input");
                    break;
                }
            }
            break;
        }
        case VARIABLE:
        {
            int tmp_var_hash = murmurHash($ND_STR, node->data_lng);
            int tmp_var_num  = FindVariable(vr_lists, tmp_var_hash);

            if (tmp_var_num == -1)
            {
                printf("Variable not found !\n");

                $FREE_VAR.var_hash     = tmp_var_hash;
                $FREE_VAR.var_dx_shift = $FREE;
                tmp_var_num            = $FREE;

                $FREE += 1;
            }

            $PRINT("[dx + %d]\n", vr_lists->var[tmp_var_num].var_dx_shift);

            break;
        }
        case CONSTANT:
        {
            $PRINT("%.1lf\n", $ND_DBL);

            break;
        }
        case DEFINE:
        {
            var_lists vr_lists_new = {};
            vr_lists_new.var = (var_list*) calloc(MAX_STR_SIZE, sizeof(var_list));
            
            $VISIT_NEW_LIST($L, &vr_lists_new);

            PRINT_LINE

            $PRINT("POP ex\n");

            printf("Number of args = %d\n", vr_lists_new.free);
            for (int i = vr_lists_new.free - 1 ; i >= 0 ; i--)
            {
                $PRINT("POP [dx + %d]\n", i);
            }
            $VISIT_NEW_LIST($R, &vr_lists_new);

            free(vr_lists_new.var);

            break;
        }
        case FUNCTION:
        {
            $PRINT("\t::%.*s\n", node->left->data_lng, node->left->data.str);

            $VISIT($R);

            break;
        }
        case PARAMETER:
        {
            printf("I am in parametr !\n");
            int tmp_hash = murmurHash(node->right->data.str, node->right->data_lng);

            $FREE_VAR.var_dx_shift = $FREE;
            $FREE_VAR.var_hash     = tmp_hash;

            $FREE += 1;

            $VISIT($L);

            break;
        }
        case PARAMETER_CALL:
        {
            $PRINT("PUSH ");
            
            $VISIT($R);
            
            $VISIT($L);

            break;
        }
        case CALL:
        {
            if (strncmp(node->left->data.str, "scan", 4) == 0)
            {  
                $PRINT("IN\n");
                $PRINT("POP ");

                int tmp_hash    = murmurHash(node->right->right->data.str, node->right->right->data_lng);
                int tmp_var_num = FindVariable(vr_lists, tmp_hash);
                if (tmp_var_num == -1 )
                    assert(0 && "VARIABLE IN SCAN NOT FOUND");

                $PRINT("[dx + %d]\n", vr_lists->var[tmp_var_num].var_dx_shift);
            }
            else if (strncmp(node->left->data.str, "print", 5) == 0)
            {
                int tmp_hash    = murmurHash(node->right->right->data.str, node->right->right->data_lng);
                int tmp_var_num = FindVariable(vr_lists, tmp_hash);
                if (tmp_var_num == -1 )
                    assert(0 && "VARIABLE IN PRINT NOT FOUND");

                $PRINT("PUSH [dx + %d]\n", vr_lists->var[tmp_var_num].var_dx_shift);
                $PRINT("PRCH\n");
            }
            else
            {
                Move_dx(vr_lists->free, com_file, PLUS);
                $VISIT($R);
                $PRINT("CALL :%.*s\n", node->left->data_lng, node->left->data.str);
                Move_dx(vr_lists->free, com_file, MINUS);
            }

            break;
        }
        case RETURN:
        {
            $PRINT("PUSH ");
            $VISIT($L);

            $PRINT("PUSH ex\nRET\n");

            break;
        }
        case IF:
        {
            $VISIT($L);
            $VISIT($R);

            break;
        }
        case DESISION:
        {
            $PRINT("JNE :itrue%p\n", node);
            $VISIT($R);
            $PRINT("JMP :iend%p\n", node);
            $PRINT("\t::itrue%p\n", node);
            $VISIT($L);
            $PRINT("\t::iend%p\n", node);

            break;
        }
        case REL_OPERATOR:
        {
            if ($R->data_type == CONSTANT || $R->data_type == VARIABLE)
                $PRINT("PUSH ");
            $VISIT($R);
            if ($L->data_type == CONSTANT || $L->data_type == VARIABLE)
                $PRINT("PUSH ");
            $VISIT($L);
            if (strncmp($ND_STR, "==", 2) == 0)
            {
                $PRINT("JE :ifistrue%p\n", node);
            }
            else if (strncmp($ND_STR, "!=", 2) == 0)
            {
                $PRINT("JNE :ifistrue%p\n", node);
            }
            else if (strncmp($ND_STR, ">=", 2) == 0)
            {
                $PRINT("JAE :ifistrue%p\n", node);
            }
            else if (strncmp($ND_STR, "<=", 2) == 0)
            {
                $PRINT("JBE :ifistrue%p\n", node);
            }
            else if (strncmp($ND_STR, ">", 1) == 0)
            {
                $PRINT("JA :ifistrue%p\n", node);
            }
            else if (strncmp($ND_STR, "<", 1) == 0)
            {
                $PRINT("JB :ifistrue%p\n", node);
            }
            else
            {
                assert(0 && "OOooops incorrect relative operator !");
            }
            $PRINT("PUSH 0\n");
            $PRINT("JMP :ifisfalse%p\n",node);
            $PRINT("\t::ifistrue%p\n", node);
            $PRINT("PUSH 1\n");
            $PRINT("\t::ifisfalse%p\n", node);
            $PRINT("PUSH 0\n");

            break;
        }
        case WHILE:
        {
            $PRINT("\t::whilestrt%p\n", node);
            $VISIT($L);
            $PRINT("JNE :wtrue%p\n", node);
            $PRINT("JMP :wfalse%p\n", node);
            $PRINT("\t::wtrue%p\n", node);
            $VISIT($R);
            $PRINT("JMP :whilestrt%p\n", node);
            $PRINT("\t::wfalse%p\n", node);
            
            break;
        }
        defaul:
        {
            assert(0 && "Shit happen(s) (undefined case in VisitPrintCommands switch)");
        }
    }
}
//==========================================================================
int Move_dx (int shift, FILE* com_file, int key)
{
    if (key == PLUS)
    {
        $PRINT("PUSH dx\n");
        $PRINT("PUSH %d\n", shift);
        $PRINT("ADD\n");
        $PRINT("POP dx\n");
    }
    else
    {
        $PRINT("PUSH dx\n");
        $PRINT("PUSH %d\n", shift);
        $PRINT("SUB\n");
        $PRINT("POP dx\n");
    }
}
//==========================================================================
int FindVariable (var_lists* vr_lists, int hash)
{
    for (int i = 0 ; i < VAR_MAX_CUNT ; i++)
    {
        if (vr_lists->var[i].var_hash == hash)
            return i;
    }

    return -1;
}
//==========================================================================
int murmurHash (char * key, unsigned int len)
{
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;

    unsigned int h = seed ^ len;

    const unsigned char* data = (const unsigned char*) key;
    unsigned int k;

    while (len >= 4)
    {
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len)
    {
        case 3:
        h ^= data[2] << 16;
        case 2:
        h ^= data[1] << 8;
        case 1:
        h ^= data[0];
        h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}
//==========================================================================
#undef $ND_CHR
#undef $ND_DBL
#undef $ND_STR
#undef $ND_LNG
#undef $ND_TYP

#undef $FREE_VAR
#undef $FREE

#undef $R
#undef $L

#undef $VISIT
#undef $VISIT_NEW_LIST
#undef $PRINT
/*main(a, b)
word_stress
    a = (b + 2) * (b + 2 );

    zvOnit a;
for_weak_people

main(1, 3);
$*/