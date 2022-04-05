#ifndef PROTECTED_H
#define PROTECTED_H
#include "sign.h"

typedef struct _protected{
    Key* pKey;
    char *mess;
    Signature* sign;
}Protected;

Protected* init_protected(Key* pKey, char* mess,Signature* sgn);
bool verify(Protected* pr);
char *protected_to_str(Protected* pro);
Protected* str_to_protected(char* str);
void freeProtected(Protected* pr);
#endif
