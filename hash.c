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
        printf("[find_position] Erreur Hashtable invalide\n");
        return -1;
    }
    if(key == NULL) {
        printf("[find_position] Erreur clé invalide\n");
        return -1;
    }
    int pos = hash_function(key,t->size);
    int cpt = 0;
    while(t->tab[pos] != NULL && ( key->keyValue != t->tab[pos]->key->keyValue && key->N != t->tab[pos]->key->N )) {
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
    if(retHashTable == NULL) {
        printf("[create_hashtable] Erreur d'allocation d'une table de hachage\n");    
        return NULL;
    }
    retHashTable->size = size;
    retHashTable->tab = (HashCell**)malloc(sizeof(CellKey*));
    while(keys) {
        int pos = find_position(retHashTable,keys->data);
        if(pos == -1) {
            return retHashTable;
        }
        retHashTable->tab[pos] = create_hashcell(keys->data);      
        keys = keys->next;
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
    HashCell *winner;
    if(hashTableVoters == NULL || hashTableCandidates == NULL) {
        printf("[compute_winner] Erreur d'allocation d'un table de hachage\n");
    }


    // Pour chaque déclaration on récupère la clé du votant et la clé du candidat pour pouvoir itérer dans les tables de hachages.
    while(dcl) {
        char *buffCandidate = (char*)calloc(50,sizeof(char));
        char *buffVoter = (char*)calloc(50,sizeof(char));
        sscanf((dcl->data)->mess,"%s %s",buffVoter,buffCandidate);
        Key *keyCandidate = str_to_key(buffCandidate);
        Key *keyVoter = str_to_key(buffVoter);

        if((hashTableVoters->tab[hash_function(keyVoter,sizeV)])->val == 0) {
            (hashTableVoters->tab[hash_function(keyVoter,sizeV)])->val++;
            (hashTableCandidates->tab[hash_function(keyCandidate,sizeC)])->val++;
        } else {
            printf("Le citoyen (%lx,%lx) à déjà voté \n", keyVoter->keyValue, keyVoter->N);
        }


        free(keyCandidate);
        free(keyVoter);
        free(buffCandidate);
        free(buffVoter);
        dcl = dcl->next;
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