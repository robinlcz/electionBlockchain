#include "header/blockchain.h"
#include <math.h>
#include <dirent.h>

CellTree* create_node(Block *b) {
    CellTree *retCT = (CellTree*)malloc(sizeof(CellTree));
    if(retCT == NULL) {
        printf("[create_node] Erreur d'allocation mémoire");
        return NULL;
    }
    retCT->block = b;
    retCT->father = NULL;
    retCT->firstChild = NULL;
    retCT->nextBro = NULL;
    retCT->height = 0;
}

int update_height(CellTree *father, CellTree *child) {
    if(father == NULL || child == NULL) {
        return 0;
    }
    if(father->height < child->height+1) {
        father->height = child->height+1;
        return 1;
    }
    return 0;
}

void add_child(CellTree *father, CellTree* child) {
    if(father == NULL || child == NULL) { return; }
    CellTree* pointerFather = father;
    CellTree* pointerCT = child;
    child->father = father;
    if(father->firstChild == NULL) {
        father->firstChild = child;
    } else {
        // On cherche le premier frère libre
        pointerCT = father->firstChild;
        while(pointerCT != NULL) {
            pointerCT = pointerCT->nextBro;
        }
        pointerCT = child;
    }
    // On va ensuite mettre à jour récursivement tous les father
    while(pointerFather != NULL) {
        update_height(pointerFather,pointerCT);
        pointerCT = pointerFather;
        pointerFather = pointerFather->father;
    }
}

void print_tree(CellTree *racine) {
    // On suppose qu'on affiche la racine , ses frères puis sa descendance
    if(racine == NULL) {
        return;
    }

    // On affiche le noeud courant 
    printf("Height : %d, \t Hash : ", racine->height);
    unsigned char* tempBlock = block_to_str2(racine->block);
    showHash(tempBlock);
    free(tempBlock);

    // On fait un appel récursif sur nextBro et firstChild
    print_tree(racine->firstChild);
    print_tree(racine->nextBro);
}

void delete_node(CellTree* node) {
    if(node == NULL) {
        printf("[delete_node] node is null \n");
        return;
    }
    if(node->father != NULL) {
        node->father->firstChild = node->nextBro;
    }
    delete_block(node->block);
    free(node);
}

void delete_tree(CellTree* racine,bool father) {
    if(racine == NULL) {
        return;
    }
    if(!father) {
        delete_tree(racine->nextBro,false);
    }
    delete_tree(racine->firstChild,false);
    delete_node(racine);
}

CellTree* highest_child(CellTree* cell) {
    // On prend les deux premiers fils de cell
    CellTree* retCellTree = cell->firstChild;
    CellTree* compCellTree = cell->firstChild->nextBro;

    // On compare en gardant le fils avec la plus grande hauteur a chaque itération
    while(compCellTree != NULL) {
        if(compCellTree->height > retCellTree->height) {
            retCellTree = compCellTree;
        }
        compCellTree = compCellTree->nextBro;
    }
    return retCellTree;
}

CellTree *last_node(CellTree* tree) {
    CellTree *lastCellTree = highest_child(tree);
    while(lastCellTree->height != 0) {
        lastCellTree = highest_child(lastCellTree);
    }
    return lastCellTree;
}

void submit_vote(Protected *p) {
    if(p == NULL) {
        printf("[submit_vote] Erreur argument non initialisé\n");
        return;
    }
    FILE *pending_vote = fopen("Pending_votes.txt","a");
    char *str = protected_to_str(p);
    fprintf(pending_vote, "%s\n", str);
    free(str);
    fclose(pending_vote);
}

void create_block(CellTree* tree, Key* author, int d){
    Block* newBlock=(Block*)malloc(sizeof(Block));
    CellProtected* cp= read_protectedCell("Pending_votes.txt");
    remove("Pending_votes.txt");
    printCellProtected(cp);
    newBlock->votes= cp;
    Key* auteur= (Key*)malloc(sizeof(Key));
    init_key(auteur,author->keyValue,author->N);
    newBlock->author=auteur;
    Block* lastBlock;
    if (tree==NULL){
        lastBlock= NULL;
        newBlock->previous_hash=(unsigned char*)"0";
        newBlock->hash = (unsigned char*)"0";
        compute_proof_of_work(newBlock,d);
        printf("HASH HASH HASH : %s\n", newBlock->hash);
        tree= create_node(newBlock);
    }else{
        CellTree* t = last_node(tree);
        lastBlock=t->block;
        newBlock->previous_hash=lastBlock->hash;
        compute_proof_of_work(newBlock,d);
        add_child(t,create_node(newBlock));
    }

    FILE* f= fopen("Pending_block.txt","w");
    fprintblock(f,newBlock);
    fclose(f);
}

void add_block(int d , char* name){
    FILE* f= fopen("Pending_block.txt","r");
    Block* block= freadblock(f);
    char nomfic[256];
    fclose(f);
    // remove("Pending_block.txt");
    if(block){
        if(verify_block(block,d)){
            strcpy(nomfic,"Blockchain/");
            strcat(nomfic,name);
            // on suppose que le repertoire BlockChain existe deja
            FILE* name= fopen(nomfic,"w");
            if (name!=NULL){
                printf("Ya");
                fprintblock(name,block);
                fclose(name);
            } else {printf("Yo\n");}
        } else {
            printf("Block non vérifié\n");
        }
        // delete_block(block);
    }
}

CellTree* read_tree(){
    // on suppose que la taille du tableau est superieur au nombre de fichier lu dans Blockchain
    CellTree** T = (CellTree**)calloc(256,sizeof(CellTree*));
    DIR *rep = opendir("./Blockchain/");
    char nomfic[256];
    FILE* f;
    int i =0;
    Block* b;
    if (rep !=NULL){
        struct dirent * dir;
        while (( dir == readdir(rep))) {
            if (strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..") != 0){
                printf("Chemin du fichier : ./Blockchain/%s \n", dir->d_name);
                strcpy(nomfic,"./Blockchain/");
                strcat(nomfic,dir->d_name);
                f= fopen(nomfic,"r");
                if (f!=NULL){
                    b= freadblock(f);
                    T[i]= create_node(b);
                    i++;
                }
            }
            closedir(rep);


        }
    }
    int taille= i+1;
    int j;
    for( i=0; i<taille; i++){
        for (j=0; i<taille; j++){
            if( i!=j && strcmp(T[j]->block->previous_hash,T[i]->block->hash) == 0 ){
                add_child(T[i],T[j]);
            }
            j++;
        }
        j=0;

    }
    for (int k=0; k<taille; k++){
        if (T[k]->father==NULL){
            return T[k];
        }
    }
    printf("il y a une erreur quelque part\n");
    return NULL;

}
Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){

    CellProtected* declarations= NULL;
    CellTree* T = highest_child(tree);
    while(T!=NULL){
        declarations=fusion(declarations,T->block->votes);
        T=T->firstChild;
    }
    declarations=keepValidCellProtected(declarations);
    return compute_winner(declarations,candidates,voters,sizeC,sizeV);
}