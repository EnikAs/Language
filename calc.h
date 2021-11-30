#ifndef CALC_H_INCLUDED
#define CALC_H_INCLUDED
#include <iostream>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cassert>
#include <sys\stat.h>

extern const char* s;

int         GetG            (const char* str);

int         GetE            ();

int         GetT            ();

int         GetP            ();

int         GetN            ();

void        SyntaxERROR     ();

#endif