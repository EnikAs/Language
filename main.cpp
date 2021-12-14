#include "lang.h"

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
}