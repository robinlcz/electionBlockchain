#ifndef KEYS_H
#define KEYS_H
#include "rsa.h"
#include "crabe.h"

typedef struct _key{
    long keyValue;
    long N;
}Key;

void init_key(Key* key, long val, long n);
void init_pair_keys(Key* pKey,Key* sKey, long low_size, long up_size);
char* key_to_str(Key* key);
Key* str_to_key(char *str);
Key** init_tab_key(int size);

#endif
