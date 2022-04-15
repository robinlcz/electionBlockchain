#import "header/cellkey.h"
#import "header/keys.h"
CellKey * create_cell_key(Key* key) {
    // Crée et alloue une cellule de liste chaîne
    if(key == NULL) {
        printf("[create_cell_key] Clé en argument invalide\n");
        return NULL;
    }
    CellKey *res = (CellKey *)malloc(sizeof(CellKey));
    if(res == NULL) {
        printf("[create_cell_key] Erreur d'allocation mémoire pour la cellule \n");
        return NULL;
    }
    res->data = key;
    res->next = NULL;
    return res;
}

CellKey* headInsertCellKey(CellKey* CK, Key* key) {
    // Fait une insertion en tête de liste d'une cellule contenant une clé
    CellKey* tmp = create_cell_key(key);
    if(tmp == NULL) {
        printf("[headInsertCellKey] Erreur dans la création d'une nouvelle cellule");
    }
    tmp->next = CK;
    return tmp;
}

CellKey* read_public_keys(FILE *f) {
    // Créer une liste chainée de clé à partir d'un fichier f
    if(f == NULL) {
        printf("[read_public_keys] Erreur de fichier\n");
    }
    char buff[256];
    CellKey *res = NULL;
    while(fgets(buff,256,f) != NULL) {
        res = headInsertCellKey(res,str_to_key(buff));
    }
    return res;
}

void print_list_keys(CellKey* LCK) {
    // Affiche une liste chainée de clé
    if(LCK == NULL) {
        printf("[print_list_keys] Liste vide il n'y a rien a afficher\n");
    }
    CellKey* ptCK = LCK;
    while(ptCK && ptCK->data) {
        char *strKey = key_to_str(ptCK->data);
        printf("%s\n",strKey);
        free(strKey);
        ptCK = ptCK->next;
    }
}

void delete_cell_key(CellKey* cell) {
    // Détruit une cellule contenant une clé
    free(cell->data);
    free(cell);
}

void delete_list_keys(CellKey* LCK) {
    // Detruit une liste chainée de clé
    CellKey *ptPrec = NULL;
    while(LCK != NULL) {
        ptPrec = LCK;
        LCK = LCK->next;
        delete_cell_key(ptPrec);
    }
}