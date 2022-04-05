#ifndef RSA_H
#define RSA_H
#include <stdio.h>
#include <string.h>
#include "crabe.h"

char *decrypt(long *crypted, int size, long u, long n);
long* encrypt(char *chaine, long s, long n);
void generate_key_values(long p, long q, long* n, long* s, long* u);

#endif
