#ifndef BLOCK_H
#define BLOCK_H
#include "hash.h"

typedef struct block{
    Key* author;
    CellProtected * votes;
    unsigned char* hash;
    unsigned char* previous_hash;
    int nonce;
}Block;

void fprintblock(FILE *f, Block* block);
Block *freadblock(FILE *f);
char *block_to_str(Block* block); 
int compute_proof_of_work(Block *b, int d);

#endif