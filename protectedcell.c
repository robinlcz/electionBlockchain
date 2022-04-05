#include "header/protectedcell.h"

CellProtected* create_cell_protected(Protected* pr) {
    // Créer une cellule contenant une déclarations signées
    if(pr == NULL) {
        return NULL;
    }
    CellProtected* resCellProtected = (CellProtected *)malloc(sizeof(CellProtected));
    resCellProtected->data = pr;
    resCellProtected->next = NULL;
    return resCellProtected;
}

CellProtected* headInsertCellProtected(Protected* pr, CellProtected* listProtected) {
    // Insère en tête de la liste listProtected la déclaration signée pr
    if(listProtected == NULL) {
        return create_cell_protected(pr);
    }
    if(pr == NULL) {
        return listProtected;
    }
    CellProtected* toInsert = create_cell_protected(pr);
    toInsert->next = listProtected;
    return toInsert;
}

CellProtected* read_protectedCell() {
    // Créer une liste chainée de déclarations signées à partir du fichier declarations.txt
    FILE *f = fopen("declarations.txt","r");
    Protected* tempProtected;
    CellProtected* resList = NULL;
    if(f == NULL) {
        printf("Erreur dans l'ouverture du fichier !\n");
        return NULL;
    }
    char buff[256];
    while(fgets(buff,256,f) != NULL) {
        tempProtected = str_to_protected(buff);
        resList = headInsertCellProtected(tempProtected,resList);
    }
    fclose(f);
    return resList;
}

void printCellProtected(CellProtected* listProtectedCell) {
    // Affiche une liste chainée de déclaration signée
    if(listProtectedCell == NULL) {
        printf("Liste vide rien à afficher\n");
        return;
    }
    CellProtected *ptProtectedCell = listProtectedCell;
    while(ptProtectedCell != NULL) {
        if(ptProtectedCell->data != NULL) {
            char *str = protected_to_str(ptProtectedCell->data);
            printf("%s\n", str);
            free(str);
        }
        ptProtectedCell = ptProtectedCell->next;
    }
}

void deleteCellProtected(CellProtected* c) {
    // Supprime une cellule de déclaration signée
    freeProtected(c->data);
    free(c);
}

void deleteListCellProtected(CellProtected* listCellProtected) {
    // Supprime une liste de déclarations signées
    CellProtected *ptPrec = NULL;
    while(listCellProtected != NULL) {
        ptPrec = listCellProtected;
        listCellProtected = listCellProtected->next;
        deleteCellProtected(ptPrec);
    }
}

CellProtected *keepValidCellProtected(CellProtected* listCellProtected) {
    // Tri la liste de déclarations signées listCellProtected en ne gardant que les cellule donc la signature est valide
    CellProtected* ptPrec = NULL;
    CellProtected* ptCellProtected = listCellProtected;
    while(ptCellProtected != NULL) {
        if(!verify(ptCellProtected->data)) {
            char *signStr = protected_to_str(ptCellProtected->data);
            printf("Signature invalide : %s\n", signStr);
            free(signStr);
            if(ptPrec == NULL) {
                ptPrec = ptCellProtected;
                ptCellProtected =ptCellProtected->next;
                deleteCellProtected(ptPrec);
                listCellProtected = listCellProtected->next;
                continue;
            } else {
                ptPrec->next = ptCellProtected->next;
                ptPrec = ptCellProtected;
                ptCellProtected = ptCellProtected->next;
                deleteCellProtected(ptPrec);
            }
        } else {
            ptPrec = ptCellProtected;
            ptCellProtected = ptCellProtected->next;
        }
    }
    return listCellProtected;
}