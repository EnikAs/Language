#include "frontend.h"
#include "backend.h"

int main()
{
    FILE* inputfile = fopen("input.txt", "r");

    tkn_arr* tk_array = GetAllTokens(inputfile);
    printf("number of tokens = %d\n", tk_array->n_cunt);

    PrintAllTokens(tk_array);
    
    Tree* tree = (Tree*) calloc(1, sizeof(Tree));
    
    tree->peak = (Node*) calloc(1, sizeof(Node));
    
    tree->peak = GetG(tk_array);
    
    TreeDump(tree->peak);

    var_lists* vr_lists = (var_lists*) calloc(1, sizeof(var_lists));

    vr_lists->var = (var_list*) calloc(VAR_MAX_CUNT, sizeof(var_list));

    FILE* com_file = fopen("asm_file.txt", "w");

    VisitPrintCommands(tree->peak, vr_lists, com_file);
}