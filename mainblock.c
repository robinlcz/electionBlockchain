#include "header/block.h"

int main() {
    FILE *f = fopen("temp.txt","r");
    printf("0\n");
    Block *BlockTest = freadblock(f);
    printf("1\n");
    printf("%s\n", block_to_str(BlockTest));
    printf("2\n");
    compute_proof_of_work(BlockTest,4);
}