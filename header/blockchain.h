#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include "block.h"

typedef struct _block_tree_cell{
    Block *block;
    struct _block_tree_cell *father;
    struct _block_tree_cell *firstChild;
    struct _block_tree_cell *nextBro;
    int height;
}CellTree;

CellTree* create_node(Block *b);
int update_height(CellTree *father, CellTree *child);
void add_child(CellTree *father, CellTree* child);
void print_tree(CellTree *racine);
void delete_node(CellTree* node);
CellTree *last_node(CellTree* tree);
void delete_tree(CellTree* racine, bool father);
CellTree* highest_child(CellTree* cell);
void submit_vote(Protected *p);

#endif