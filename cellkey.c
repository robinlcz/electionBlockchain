#include "header/cellkey.h"
#include "header/keys.h"
CellKey * create_cell_key(Key* key) {
    // Crée et alloue une cellule de liste chaîne, on vérifie que key est non puis que notre malloc fonctionne bien avant d'initialiser les champs de ce dernier
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

    // On créer une cellule  qu'on va inserer dans la liste    
    CellKey* toInsert = create_cell_key(key);
    if(toInsert == NULL) {
        printf("[headInsertCellKey] Erreur dans la création d'une nouvelle cellule");
    }

    // On place la cellule en début de la liste en la liant au premier élément de cette dernière
    toInsert->next = CK;
    return toInsert;
}

CellKey* read_public_keys(char* f){
    CellKey* LCK=NULL;
    FILE* fic = fopen(f,"r");

    // On initialise les buffers dans lesquels nous allons récupérer les données du fichier passé en paramètre.
    char buffer[256];
    char virgule[10];
    char cpkey[120];
    char sskey[120];
    Key* pkey;

    /* On lit les lignes du fichier une à une, en essayant de récupérer des formats de clés. (clé publique) pour les candidats, (clé public),(cléprivé) pour les votants, on insère
    ensuite la clé publique en tête de liste*/
    while(fgets(buffer,256,fic)!=NULL){
        if(strcmp(f,"candidates.txt")==0){
            sscanf(buffer, "%s",cpkey);
        }else if(strcmp(f,"keys.txt")==0){
            sscanf(buffer, "%s , %s", cpkey,sskey);
        }else{
            printf("Erreur lors du saisie du nom du fichier\n");
            return NULL;
        }
        pkey= str_to_key(cpkey);
        LCK=headInsertCellKey(LCK,pkey);  
    }
    fclose(fic);
    return LCK;


}

void print_list_keys(CellKey* LCK) {
    // Affiche une liste chainée de clé
    if(LCK == NULL) {
        printf("[print_list_keys] Liste vide il n'y a rien a afficher\n");
    }

    // On initialise un pointeur local à la fonction pour parcourir la liste, nous affichons uns à uns les éléments de cette dernière
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

    // On initialise un pointeur local sur la liste chainées de clés puis on libère les clés une à une.
    while(LCK != NULL) {
        ptPrec = LCK;
        LCK = LCK->next;
        delete_cell_key(ptPrec);
    }
}