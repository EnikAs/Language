#ifndef BACKEND_H_INCLUDED
#define BACKEND_H_INCLUDED
#include "tree.h"

struct var_list
{
    int var_hash = 0;
    int var_dx_shift = 0;
};

struct var_lists
{
    var_list* var = NULL;
    int free = 0;
};

#endif