#include "header/blockchain.h"
#include <math.h>
#include <dirent.h>

CellTree* create_node(Block *b) {
    // On initialise un noeud et tous ses champs.
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
    // Si father ou child est null, alors il n'y a rien à faire. 
    if(father == NULL || child == NULL) {
        return 0;
    }
    // Sinon on compare la hauteur du père avec celle du fils+1, si la hauteur du père est plus grande rien ne change sinon elle devient celle du fils+1.
    if(father->height < child->height+1) {
        father->height = child->height+1;
        return 1;
    }
    return 0;
}

void add_child(CellTree *father, CellTree* child) {
    // Si father ou child est null il n'y a rien a faire.
    if(father == NULL || child == NULL) { return; }
    CellTree* pointerFather = father;
    CellTree* pointerCT = child;
    child->father = father; 
    // Si father n'a pas de fils alors child devient le premier fils de father.
    if(father->firstChild == NULL) {
        father->firstChild = child;
    } else {
        // Sinon on cherche le premier frère libre dans la liste de la fraterie
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
    if(cell == NULL) {
        return NULL;
    }
    CellTree* retCellTree = cell->firstChild;
    // Vérifie si l'arbre possède un fils, si non il est la racine la plus haute
    if(retCellTree == NULL) {
        return cell; 
    }
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
    if (newBlock==NULL){
        printf (" [creat_block] erreur lors du malloc de nexBlock\n");
        return ;
    }

    CellProtected* cp= read_protectedCell("Pending_votes.txt");
    remove("Pending_votes.txt");
    // printCellProtected(cp);


    newBlock->votes= cp;
    // on fait une copie de l'auteur que l'on insere dans le nouveau block
    Key* auteur= (Key*)malloc(sizeof(Key));
    init_key(auteur,author->keyValue,author->N);
    newBlock->author=auteur;

    CellTree* t = last_node(tree);
    Block* lastBlock= t->block;
   
    if (lastBlock==NULL){
        newBlock->previous_hash=strdup((unsigned char*)"0");
        newBlock->hash = strdup((unsigned char*)"0");
        compute_proof_of_work(newBlock,d);
    }else{
        newBlock->previous_hash=lastBlock->hash;
        compute_proof_of_work(newBlock,d);
    }
    add_child(t,create_node(newBlock));
   

    FILE* f= fopen("Pending_block.txt","w");
    fprintblock(f,newBlock);
    fclose(f);
}

void add_block(int d, char* name){
    FILE *f1 = fopen("Pending_block.txt","r");
    Block *b =freadblock(f1);
    fclose(f1);
    if(verify_block(b,d)){
        char str[256];
        strcpy(str, "Blockchain/");
        strcat(str, name);
        FILE *f = fopen(str,"w");
        fprintblock(f, b);
        fclose(f);
    }
    delete_block(b);
    remove("Pending_block.txt");
}


CellTree* read_tree(){
    DIR *repertoire = opendir("./Blockchain/");
  int cpt = 0, i = 0;

  if (repertoire != NULL) {
    struct dirent *dir;

    //on parcourt pour connaitre le nombre de fichier
    while (dir = readdir(repertoire)){
      if ((strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, ".."))){
          cpt++;
      }
    }
    printf("cpt=%d\n",cpt);

    // On réinitialise la position du pointeur du flux dir au debut du repertoire
    rewinddir(repertoire);

    // On alloue de l'espace mémoire pour les noeuds de l'arbre

    CellTree **tree = (CellTree **)malloc(cpt * sizeof(CellTree *));

    if (tree == NULL){
      printf("erreur lors du malloc\n");
      closedir(repertoire);
      return NULL;
    }

    Block *block;
    CellTree *t;
    int ind = 0;
    char name[256];

    while (dir = readdir(repertoire)) {
      if ((strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, ".."))) {
          strcpy(name, "./Blockchain/");
          strcat(name, dir->d_name);
          FILE* fic = fopen(name,"r");
          block = freadblock(fic);
          t = create_node(block);
          tree[ind++] = t;
        //   remove(name);
          fclose(fic);
        }
     
    }

    //on parcourt le tableau tree pour relier les enfants avec leur père
    for (int i = 0; i < cpt; i++)  {
      for (int j = 0; j < cpt; j++) {
        //dans le cas où tree[j] est un enfant de tree[i]
        if ((((tree[j])->block)->previous_hash != NULL) && (strcmp(((tree[j])->block)->previous_hash, ((tree[i])->block)->hash) == 0)) {
          add_child(tree[i], tree[j]);
        }
      }
    }

    // Dans le cas où on trouve la racine de l'arbre
    CellTree *temp;
    int i;
    for (i = 0; i < cpt; i++) {
      if ((tree[i])->father == NULL) {
        temp = tree[i];
      }
    }

    closedir(repertoire);
    free(tree);
    return temp;
  }
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