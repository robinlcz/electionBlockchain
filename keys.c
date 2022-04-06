#include "header/keys.h"

void init_key(Key *key, long val, long n) {
    // Initialise une clé (val,n)
    key->keyValue = val;
    key->N = n;
}

void init_pair_keys(Key* pKey,Key* sKey, long low_size, long up_size) {
    // Initialise une paire de clé à partir d'un intervalle d'entier
    long primeP = random_prime_number(low_size,up_size,10);
    long primeQ = random_prime_number(low_size,up_size,10);
    while(primeQ == primeP) {
        primeQ = random_prime_number(low_size,up_size,10);
    }
    long n,s,u;
    generate_key_values(primeP,primeQ,&n,&s,&u);
    init_key(pKey,s,n);
    init_key(sKey,u,n);
}

char* key_to_str(Key* key) {
    // Renvoie la représentation en chaine de caractère d'une clé
    char *buff = (char *)calloc(256,sizeof(char));
    if(buff == NULL) {
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }
    char*keyHex;
    if(sprintf(buff,"(%lx,%lx)", key->keyValue, key->N) < 0) {
        printf("Erreur d'ecriture \n");
        return NULL;
    }
    keyHex = (char *)malloc(sizeof(char)*strlen(buff)+1);
    if(keyHex == NULL) {
        printf("Erreur d'allocation mémoire.");
    }
    strcpy(keyHex, buff);
    free(buff);
    return keyHex;
}

Key* str_to_key(char* str) {
    // Génère une clé à partir de sa représentation chaine de caractère
    Key *resKey = (Key*)malloc(sizeof(Key));
    if(resKey == NULL) {
        printf("Erreur dans l'allocation mémoire d'une Key\n");
        return NULL;
    }
    long tmpVal, tmpN;
    if(sscanf(str,"(%lx,%lx)", &tmpVal, &tmpN) != 2) {
        printf("Erreur de format dans la chaine de caractère\n");
        return NULL;
    }
    init_key(resKey,tmpVal,tmpN);
    return resKey;
}

Key** init_tab_key(int size) {
    Key** tab = (Key**)malloc(sizeof(Key*)*size);
    if(tab == NULL) {
        printf("Erreur dans l'allocation d'un tableau de Key\n");
    }
    return tab;
}
