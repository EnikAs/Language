#include "calc.h"

int GetG(const char* str)
{
    s = str;
    int val = GetE();
    if      (*s == '$') s++;
    else    SyntaxERROR();

    return val; 
}

int GetE()
{
    int val = GetT();
    while(*s == '+' || *s == '-')
    {
        char op = *s;
        s++;
        int val2 = GetT();
        if (op == '+') val += val2;
        else           val -= val2;
    }

    return val;
}

int GetT()
{
    int val = GetP();
    while(*s == '*' || *s == '/')
    {
        char op = *s;
        s++;
        int val2 = GetP();
        if (op == '*') val *= val2;
        else           val /= val2;
    }

    return val;
}

int GetP()
{
    if (*s == '(')
    {
        s++;
        int val = GetE();
        s++;
        return val;
    }
    else 
    {
        int val2 = GetN();
        return val2;
    }
}

int GetN()
{
    int val = 0;
    const char* olds = s;
    while ('0' <= *s && *s <= '9')
    {
        val = val * 10 + (*s - '0');
        s++;
    }
    if (olds == s)
        SyntaxERROR();

    return val;
}

void SyntaxERROR()
{
    printf("Ti dolboeb");
    assert(0);
}