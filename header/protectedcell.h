#ifndef PROTECTEDCELL_H
#define PROTECTEDCELL_H
#include "protected.h"

typedef struct cellProtected{
    Protected *data;
    struct cellProtected *next;
}CellProtected;

void fprintCellProtected(FILE *f,CellProtected* listProtectedCell);
CellProtected* create_cell_protected(Protected* pr);
CellProtected* read_protectedCell();
void printCellProtected(CellProtected* listCellProtected);
void deleteCellProtected(CellProtected* c);
void deleteListCellProtected(CellProtected* listCellProtected);
CellProtected *headInsertCellProtected(Protected* pr, CellProtected* listProtected);
CellProtected *keepValidCellProtected(CellProtected* listCellProtected);


#endif
