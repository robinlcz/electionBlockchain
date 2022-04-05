#ifndef SIGN_H
#define SIGN_H
#include "keys.h"

typedef struct _signature{
    long *tab;
    int tabSize;
}Signature;

Signature* init_signature(long* content, int size);
Signature* sign(char* mess, Key* sKey);
char* signature_to_str(Signature* sgn);
Signature* str_to_signature(char* str);
void freeSignature(Signature* sgn);

#endif
