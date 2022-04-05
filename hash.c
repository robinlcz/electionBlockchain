#include "header/hash.h"

HashCell* create_hashcell(Key* key) {
    HashCell* resHashCell = (HashCell*)malloc(sizeof(HashCell));
    resHashCell->val = 0;
    resHashCell->key = key;
    return resHashCell;
}

int hash_function(Key* key ,int size) {
    return (key->keyValue*key->N)%size;
}

int find_position(HashTable *t, Key* key) {
    if(t == NULL) {
        return -1;
    }
    int pos = hash_function(key,t->size);
    int cpt = 0;
    while(key->keyValue != t->tab[pos] && key->N != t->tab[pos]->N) {
        pos++;
        cpt++;
        if (cpt > t->size) {
            return -1;
        }
    }
    return pos;
}

HashTable* create_hashtable(CellKey *keys, int size) {
    if(!size) {return NULL;}
    HashTable *retHashTable = (HashTable*)malloc(sizeof(HashTable));
    retHashTable->size = size;
    retHashTable->tab = (CellKey**)malloc(sizeof(CellKey*));
    CellKey* ptCellKey = keys;
    while(ptCellKey) {
        int pos = hash_function(ptCellKey->data,size);
        while(retHashTable->tab[pos] != NULL) {
            pos++;
        }
        retHashTable->tab[pos] = create_hashcell(ptCellKey->data);
        ptCellKey = ptCellKey->next;
    }
    return retHashTable;
}