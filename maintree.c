#include "header/blockchain.h"

int main() {
    FILE *fb = fopen("temp.txt","r");
    Block *block = freadblock(fb);


    CellTree *tree = create_node(block);
    CellTree *child = create_node(block);

    print_tree(tree);
    add_child(tree,child);
    print_tree(tree);

    printf("highest child \n");
    print_tree(highest_child(tree));

    printf("last node \n");
    print_tree(last_node(tree));

    printListCellProtected(tree->block->votes);
    delete_tree(tree,true);
}