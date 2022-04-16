#include "header/hash.h"

void generate_random_data(int nv, int nc) {
    if(nv == 0 || nc == 0) {
        printf("Args Error\n");
        return;
    }
    FILE *fv = fopen("keys.txt","w");
    FILE *fc = fopen("candidates.txt","w");
    FILE *fd = fopen("declarations.txt","w");
    int* tabIndice = (int*)calloc(nv,sizeof(int));
    if(tabIndice == NULL) {
        printf("Erreur dans l'allocation d'un tableau d'indice, impossible de générer des candidats. \n");
        return;
    }
    Key* tab_public_keys[nv];
    Key* tab_secret_keys[nv];
    Key* tab_candidate_keys[nc];

    // (public,secret) keys generation
    for(int i = 0; i < nv; i++) {
        Key* pKey = (Key*)malloc(sizeof(Key));
        Key* sKey = (Key*)malloc(sizeof(Key));
        init_pair_keys(pKey,sKey,2,7);
        fprintf(fv,"(%lx,%lx) (%lx,%lx)\n", pKey->keyValue, pKey->N, sKey->keyValue,sKey->N);
        tab_public_keys[i] = pKey;
        tab_secret_keys[i] = sKey;
    }

    // Candidates selection
    for(int i = 0; i < nc; i++) {
        int pos = rand()%nc;
        while(tabIndice[pos] == 1) {
            pos = rand()%nc;
        }
        tab_candidate_keys[i] = tab_public_keys[pos];
        tabIndice[pos] = 1;
        fprintf(fc,"(%lx,%lx)\n", tab_candidate_keys[i]->keyValue, tab_candidate_keys[i]->N);
    }

    // Declarations
    for(int i = 0; i < nv; i++) {
        int idCandidate = rand()%nc;
        char *vote = key_to_str(tab_candidate_keys[idCandidate]);
        Signature *voteSign = sign(vote, tab_secret_keys[i]);
        Protected *voteProtected = init_protected(tab_public_keys[i],vote,voteSign);
        char *declaration = protected_to_str(voteProtected);
        fprintf(fd,"%s\n", declaration);
        freeSignature(voteSign);
        free(voteProtected->mess);
        free(voteProtected);
        free(declaration);
    }
    for(int i = 0; i < nv; i++) {
        free(tab_public_keys[i]);
    }

    for(int i = 0; i < nv; i++) {
        free(tab_secret_keys[i]);
    }
    free(tabIndice);
    fclose(fv);
    fclose(fc);
    fclose(fd);
}

int main(int argc, char** argv){
    if(argc != 3){
        printf("Nombre d'arguments incorrect.\n");
        return 1;
    }
    srand(time(NULL));
    int nv = atoi(argv[1]);
    int nc = atoi(argv[2]);
    generate_random_data(nv, nc);
    CellProtected *cp = read_protectedCell();
    printCellProtected(cp);

    keepValidCellProtected(cp);
    
    FILE *fc = fopen("candidates.txt","r");
    FILE *fv = fopen("keys.txt","r");
    CellKey* cand = read_public_keys(fc);
    CellKey* vot = read_public_keys(fv);

    HashCell* cellule = create_hashcell(cand->data);
    HashTable* table = create_hashtable(cand, 40);

    Key *vainqueur = compute_winner(cp, cand, vot, 16, 36);
    char* strvainq = key_to_str(vainqueur);
    printf("vainqueur : %s\n", strvainq);

    deleteListCellProtected(cp);
    delete_list_keys(cand);
    delete_list_keys(vot);
    delete_hashtable(table);
    free(cellule->key);
    free(cellule);
    free(vainqueur);
    free(strvainq);
    return 0;
}