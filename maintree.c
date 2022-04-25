#include "header/blockchain.h"

int main() {
    FILE *fb = fopen("temp.txt","r");
    FILE *fb2 = fopen("temp.txt","r");
    if(fb == NULL || fb2 == NULL) {
        printf("Erreur dans l'ouverture du fichier temp.txt\n");
        exit(EXIT_FAILURE);
    }
    
    Block *block = freadblock(fb);
    Block *block2 = freadblock(fb2);

    // Interet : on montre que deux bloques supposés identiques ont un hash différent par le système de blockchain
    CellTree *tree = create_node(block);
    compute_proof_of_work(block,2);
    block2->previous_hash = strdup(block->hash);
    CellTree *child = create_node(block2);
    compute_proof_of_work(block2,2);
    char *temp = block_to_str(block2);
    printf("block : %s\n",temp);
    print_tree(tree);
    add_child(tree,child);
    print_tree(tree);

    printf("highest child \n");
    print_tree(highest_child(tree));

    printf("last node \n");
    print_tree(last_node(tree));

    printCellProtected(tree->block->votes);

    free(tree->block->author);
    free(child->block->author);
    delete_tree(tree,true);
    fclose(fb);
}