#include "header/hash.h"

HashCell* create_hashcell(Key* key) {
    HashCell* resHashCell = (HashCell*)malloc(sizeof(HashCell));
    if(resHashCell == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }
    resHashCell->val = 0;
    resHashCell->key = key;
    return resHashCell;
}

int hash_function(Key* key ,int size) {
    return (key->keyValue*key->N)%size;
}

int find_position(HashTable *t, Key* key) {
    if(t == NULL) {
        return -1;
    }
    int pos = hash_function(key,t->size);
    int cpt = 0;
    while((key->keyValue != ((t->tab[pos])->key)->keyValue && key->N != ((t->tab[pos])->key)->keyValue || (t->tab[pos] == NULL))) {
        ++pos%(t->size);
        cpt++;
        if (cpt > t->size) {
            return -1;
        }
    }
    return pos;
}

HashTable* create_hashtable(CellKey *keys, int size) { // To fix 
    if(!size) {return NULL;}
    HashTable *retHashTable = (HashTable*)malloc(sizeof(HashTable));
    retHashTable->size = size;
    retHashTable->tab = (HashCell**)malloc(sizeof(CellKey*));
    CellKey* ptCellKey = keys;
    while(ptCellKey) {
        int pos = hash_function(ptCellKey->data,size);
        if(pos == -1) {
            return retHashTable;
        }
        HashCell *tempHashCell = create_hashcell(ptCellKey->data);
        (retHashTable->tab)[pos] = tempHashCell;
        ptCellKey = ptCellKey->next;
    }
    return retHashTable;
}

void delete_hashtable(HashTable *t) {
    for(int i = 0; i < t->size; i++) {
        free((t->tab)[i]->key);
        free((t->tab)[i]);
    }
    free(t->tab);
    free(t);
}

Key* compute_winner(CellProtected* dcl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV) {
    HashTable *hashTableCandidates = create_hashtable(candidates,sizeC);
    HashTable *hashTableVoters = create_hashtable(voters,sizeV);
    keepValidCellProtected(dcl); // On élimine les déclarations de vote non valides
    CellProtected *pointerDcl = dcl;
    HashCell *winner;

    // Pour chaque déclaration on récupère la clé du votant et la clé du candidat pour pouvoir itérer dans les tables de hachages.
    while(pointerDcl) {
        char *buffCandidate = (char*)calloc(50,sizeof(char));
        char *buffVoter = (char*)calloc(50,sizeof(char));
        sscanf((pointerDcl->data)->mess,"%s %s",buffVoter,buffCandidate);
        Key *keyCandidate = str_to_key(buffCandidate);
        Key *keyVoter = str_to_key(buffVoter);
        if((hashTableVoters->tab[hash_function(keyVoter,sizeV)])->val == 0) {
            (hashTableVoters->tab[hash_function(keyVoter,sizeV)])->val++;
            (hashTableCandidates->tab[hash_function(keyCandidate,sizeC)])->val++;
        } else {
            printf("Le citoyen (%lx,%lx) à déjà voté \n", keyVoter->keyValue, keyVoter->N);
            continue;
        }
        free(keyCandidate);
        free(keyVoter);
        free(buffCandidate);
        free(buffVoter);
        return winner->key;
    }

    // On cherche dans la table de Hachage des candidats lequel à le plus de vote.
    HashCell **finalTabCandidates = hashTableCandidates->tab;
    winner = finalTabCandidates[0];
    for(int i = 0; i < sizeC; i++) {
        if(winner->val < finalTabCandidates[i]->val) {
            winner = finalTabCandidates[i];
        }
    }
    return winner->key;
}