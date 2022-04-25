#include "header/protectedcell.h"
#include <stdio.h>

CellProtected* create_cell_protected(Protected* pr) {
    // Créer une cellule contenant une déclaration signée passé en paramètre
    if(pr == NULL) {
        return NULL;
    }

    CellProtected* resCellProtected = (CellProtected *)malloc(sizeof(CellProtected));
    if(resCellProtected == NULL) {
        printf("Erreur dans l'allocation d'une cellule de protected\n");
        return NULL;
    }

    // On initialise le champ data avec notre déclaration, puis on fixe l'élément suivant à NULL
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
    // On Initialise une cellule contenant la déclaration pr, puis on relie cette dernière au premier élément de listProtected.
    CellProtected* toInsert = create_cell_protected(pr);
    toInsert->next = listProtected;
    return toInsert;
}

CellProtected* read_protectedCell(char *fic) {
    // Créer une liste chainée de déclarations signées à partir du fichier declarations.txt
    FILE *f = fopen(fic,"r");
    Protected* tempProtected;
    CellProtected* resList = NULL;
    if(f == NULL) {
        printf("Erreur dans l'ouverture du fichier !\n");
        return NULL;
    }
    // Initialisation d'un buffer pour récupérer les données de fic.
    char buff[256];

    // Tant qu'il y a des lignes dans notre fichier, on détecte le format d'une déclaration signée dans chaque ligne, laquelle on insère en tête de notre liste.
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

    // On parcourt les éléments de la liste à l'aide d'un pointeur local, puis si la cellule contient une déclaration non nulle, on affiche cette dernière.
    while(ptProtectedCell != NULL) {
        if(ptProtectedCell->data != NULL) {
            char *str = protected_to_str(ptProtectedCell->data);
            printf("%s\n", str);
            free(str);
        }
        ptProtectedCell = ptProtectedCell->next;
    }
}

void fprintCellProtected(FILE *f,CellProtected* listProtectedCell) {
    // Affiche une liste chainée de déclaration signée
    if(listProtectedCell == NULL) {
        printf("Liste vide rien à afficher\n");
        return;
    }

    char *str; 
    CellProtected *ptProtectedCell = listProtectedCell;

    /* On parcourt les éléments de la liste à l'aide d'un pointeur local, puis si la cellule contient une déclaration non nulle, on écrit cette dernière dans un fichier f en paramètre. 
    */
    while(ptProtectedCell != NULL) {
        if(ptProtectedCell->data != NULL) {
            str = protected_to_str(ptProtectedCell->data);
            if(str == NULL) {
                printf("[fprintCellProtected] Erreur de chaine de caractère\n");
                return;
            }
            fprintf(f,"%s\n", str);
            free(str);
        }
        ptProtectedCell = ptProtectedCell->next;
    }
    printf("\n");
}

void deleteCellProtected(CellProtected* c) {
    // Supprime une cellule de déclaration signée
    freeProtected(c->data);
    free(c);
}

void deleteListCellProtected(CellProtected* listCellProtected) {
    // Supprime une liste de déclarations signées
    CellProtected *ptPrec = NULL;
    // On parcourt les éléments de listCellProtected un à un et on libère à chaque fois l'élément qui précède le courant.
    while(listCellProtected != NULL) {
        ptPrec = listCellProtected;
        listCellProtected = listCellProtected->next;
        deleteCellProtected(ptPrec);
    }
}

CellProtected *keepValidCellProtected(CellProtected* listCellProtected) {
    // Tri la liste de déclarations signées listCellProtected en ne gardant que les cellule dont la signature est valide
    CellProtected* ptPrec = NULL;
    CellProtected* ptCellProtected = listCellProtected;

    // On parcourt la liste de déclaration a l'aide d'un pointeur local.
    while(ptCellProtected != NULL) {
        // Si la déclration est invalide alors, on enlève de la liste cette dernière, en s'adaptant à sa place dans la liste.
        if(!verify(ptCellProtected->data)) {
            char *signStr = protected_to_str(ptCellProtected->data);
            printf("Signature invalide : %s\n", signStr);
            free(signStr);
            // Si on enlève le premier élément alors le début de la liste devient le second élément.
            if(ptPrec == NULL) {
                ptPrec = ptCellProtected;
                ptCellProtected =ptCellProtected->next;
                deleteCellProtected(ptPrec);
                listCellProtected = listCellProtected->next;
                continue;
            } else { // Si ce n'est pas le premier élément alors celui qui le précède pointe maintenant sur celui qui le suit.
                ptPrec->next = ptCellProtected->next;
                ptPrec = ptCellProtected;
                ptCellProtected = ptCellProtected->next;
                deleteCellProtected(ptPrec);
            }
        } else { // Si la déclaration est valide alors on passe à la suivant.
            ptPrec = ptCellProtected;
            ptCellProtected = ptCellProtected->next;
        }
    }
    return listCellProtected;
}


CellProtected *fusion(CellProtected* listCellProtected, CellProtected* listCellProtected2) {
    // On va raccorder le dernier élément de listCellProtected avec le premier de listCellProtected2 de listCellProtected
    CellProtected* cour = listCellProtected;
    CellProtected* pred = NULL;
    while(cour != NULL) {
        pred = cour;
        cour = cour->next;
    }
    // Ici nous avons l'adresse de dernier element stockées dans pred
    pred->next = listCellProtected2;
    return listCellProtected;
    // L'opération pourrait être fait en O(1) avec une liste doublement chainées
    // Nous sommes actuellement en O(len(listCellProtected))
}

CellProtected* endInsertCellProtected(Protected* pr, CellProtected* listProtected){
    // Insere en queue de la liste listProtected la déclaration signée pr
    if(listProtected == NULL) {
        return create_cell_protected(pr);
    }
    if(pr == NULL) {
        return listProtected;
    }
     /* On initialise un pointeur qui précède l'élément courant, et un pointeur local sur la liste, puis on créer une cellule pour la déclaration que l'on souhaite ajouter */
     CellProtected* cour = listProtected;
     CellProtected* pred = NULL;
     CellProtected* toInsert = create_cell_protected(pr);

     // On cherche le dernier élément, puis on le relie à notre nouvelle déclaration.
     while(cour!=NULL){
         pred = cour; 
         cour = cour->next;
     }
     pred->next=toInsert;
     return listProtected;
}