#include "calc.h"

const char* s = NULL;

int main()
{
    char str[1000]= "";
    scanf("%s", str);
    int ans = GetG(str);
    printf ("Answer = %d", ans);
}