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
void compute_proof_of_work(Block *b, int d);
char *block_to_str2(Block *block);
unsigned char* str_to_hash(const char *hash);
void* showHash(const char *hash);
bool compte_zeros(unsigned char* hash, int d);
bool verify_block(Block *b, int d);
void delete_block(Block *b);
#endif