#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include "block.h"

typedef struct _block_tree_cell{
    Block *block;
    struct block_tree_cell *father;
    struct block_tree_cell *firstChild;
    struct block_tree_cell *nextBro;
    int height;
}CellTree;

CellTree* create_note(Block *b);
int update_height(CellTree *father, CellTree *child);
void add_child(CellTree *father; CellTree* child);
void print_tree(CellTree *racine);
void delete_node(CellTree* node);
void delete_tree(CellTree* racine);
CellTree* highest_child(CellTree* cell);
void submit_vote(Protected *p);

#endif