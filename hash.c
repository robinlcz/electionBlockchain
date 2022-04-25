#include "header/hash.h"

CellProtected* sup_invalid_decla(CellProtected* LCP){
    CellProtected* tmp;
    // Si la liste passé en paramètre est nulle -> rien à faire
    if(LCP== NULL){
        return NULL;
    }

    // On vérifie si les premières déclarations son valide afin de déterminer le premier élément valide de la liste.
    while(!verify(LCP->data)){
        tmp=LCP;
        LCP=LCP->next;
        deleteCellProtected(tmp);
    }

    // On parcourt la liste à l'aide d'un poiteur local afin d'éliminer les déclarations non valides.
    CellProtected* list = LCP;
    while(list->next!=NULL){
        if(!verify(list->next->data)){
            tmp=list->next;
            list->next=list->next->next;
            deleteCellProtected(tmp);
        }
        list=list->next;
    }
    // On renvoie l'adresse du premier élément valide.
    return LCP;
}


HashCell* create_hashcell(Key* key){
    // Initialisation d'une cellule à partir d'une clé.
    HashCell* HC= (HashCell*)malloc(sizeof(HashCell));
    if(HC == NULL) {
        printf("[create_hashcell] Erreur d'allocation mémoire\n");
    }
    HC->key=key;
    HC->val= 0;
    return HC;
}


int hash_function(Key* key, int size){
    // Fonction de hachage : on fait la somme clépublique+cléprivé+1 le tout modulo la taille de notre table de hachage
    long v1= key->keyValue;
    long v2= key->N;
    int pos= (int)((v1+v2)+1)%size;
    return pos;
}

int find_position(HashTable* t , Key* key){
    // retourne -1 si la cle n'a pas ete trouvé et qu'il n'y a plus de places dans le tableau, la position de key si elle est presnte dans la table de hashage, la position ou devrait etre la cle sinon
    int pos= hash_function(key,t->size);
    int i =0;
    while(i<t->size){
        // Si la clé n'est pas présente : 
        if(t->tab[((pos+i)%t->size)]==NULL){
            return ((pos+i)%t->size);
        }

        // Si elle l'est alors sa position théorique :
        if(t->tab[((pos+i)%t->size)]->key->N== key->N && t->tab[((pos+i)%t->size)]->key->keyValue == key->keyValue){
            return ((pos+i)%t->size);
        }
        i=i+1;
    }
    printf("la cle (%lx,%lx) n'est pas présente dans la table de hashage et il est impossible de l'inserer car le tableau est plein\n",key->N, key->keyValue);
    return -1;
}


HashTable* create_hashtable(CellKey* keys, int size){
    HashTable* t = (HashTable*)malloc(sizeof(HashTable));
    HashCell** tab = (HashCell**)malloc(sizeof(HashCell*)*size);
    if(tab == NULL || t == NULL) {
        printf("[create_hashtable] Erreur d'allocation mémoire\n");
    }
    CellKey* tmp= keys;
    int pos;
    t->size=size;
    t->tab=tab;
    // initialisation des elements du tableau à NULL
    for(int i=0;i<size;i++){
        t->tab[i]=NULL;
    }

    // Pour chaque clé de la liste, on détermine sa position dans la table de hachage que l'on crée. Ensuite on initialise une cellule contenant la clé à la bonne position.
    while(tmp!=NULL){
        pos=find_position(t,tmp->data);
        if (pos!=-1){
            t->tab[pos]=create_hashcell(tmp->data);
        }
        tmp=tmp->next; 
    }

    return t;
}


void delete_hashtable(HashTable *t ){
    // On libère toute les cellules du tableau, puis on libère la mémoire allouée pour le tableau et enfin la structure de la table de hachage.
    for( int i=0;i<t->size;i++){
        if(t->tab[i]!=NULL){
            free(t->tab[i]);
        }
    }
    free(t->tab);
    free(t);
}


Key* compute_winner( CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
    // On initialise deux tables de hachages, une pour les candidats l'autre pour les votants : 
    HashTable* Hc = create_hashtable(candidates,sizeC);
    HashTable* Hv= create_hashtable(voters,sizeV);
    int posV;
    int posC;
    Key* pkeyc; 
    Key* pkeyv;
    int nbvotes=-1;
    Key* gagnant;
    // On parcourt la liste des déclarations de votes : 
    while (decl !=NULL){
         pkeyv= decl->data->pKey;
         posV= find_position(Hv,pkeyv);
            // On verifie que le candidat n'a pas deja voté 
         if(Hv->tab[posV]->val==0){
             pkeyc= str_to_key(decl->data->mess);
             posC= find_position(Hc,pkeyc);
             free(pkeyc);
            // On verifie que le candidat pour lequel a voté le citoyen existe, si il existe alors : le citoyen à voté et le candidat gagne une voix. 
             if(Hc->tab[posC]!=NULL){
                 Hv->tab[posV]->val=1;
                 Hc->tab[posC]->val=1+Hc->tab[posC]->val;
             }
        }        
        decl= decl->next;
    }

    // On parcourt la table de hachage, dans laquelle on cherche le candidat ayant reçu le plus de voix.
    for(int i=0; i<sizeC; i++){
        if ( Hc->tab[i]!=NULL && Hc->tab[i]->val >= nbvotes){
            nbvotes=Hc->tab[i]->val;
            gagnant=Hc->tab[i]->key;
        }
    }
    // On récupère le gagnant de l'election. 
    Key *retGagnant = (Key *)malloc(sizeof(Key));
    init_key(retGagnant,gagnant->keyValue, gagnant->N);
    delete_hashtable(Hc);
    delete_hashtable(Hv);
    return retGagnant;
}