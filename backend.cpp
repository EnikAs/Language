#include "backend.h"

#define $ND_CHR node->data.ch
#define $ND_DBL node->data.dbl
#define $ND_STR node->data.str
#define $ND_LNG node->data.lng
#define $ND_TYP node->data_type

#define $R node->right
#define $L node->left

#define $VISIT(node)            \
if (node)                       \
    VisitPrintCommands(node);   \

int VisitPrintCommands (Node* node)
{
    switch($ND_TYP)
    {
        case STATEMENT:
        {
            $VISIT($R)
            $VISIT($L)
        }
        case OPERATOR:
        {
            
        }
    }
}














