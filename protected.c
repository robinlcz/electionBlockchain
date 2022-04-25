#include "header/protected.h"


Protected* init_protected(Key* pKey, char* mess,Signature* sgn) {
    // Initialise une déclaration signée à partir d'un clé, un message et une signature
    Protected* retProtected = (Protected*)malloc(sizeof(Protected));
    if(retProtected == NULL) {
        printf("Erreur dans l'allocation mémoire d'un protected\n");
        return NULL;
    }
    
    if(pKey == NULL) {
        printf("Clé publique invalide \n");
        return NULL;
    }
    if(mess == NULL) {
        printf("Déclaration de vote invalide \n");
        return NULL;
    }
    retProtected->pKey = pKey;
    retProtected->mess = mess;
    retProtected->sign = sgn;
    return retProtected;
}

bool verify(Protected* pr) {
    // Vérifie la validité de la signature de la déclaration signée pr
    char *strtemp = decrypt(pr->sign->tab,pr->sign->tabSize,pr->pKey->keyValue,pr->pKey->N);
    bool ret = strcmp(strtemp,pr->mess);
    free(strtemp);
    return !ret;
}

char* protected_to_str(Protected* pro) {
    // Renvoie la représentation chaine de caractère d'une déclaration signée
    char* buff = (char*)calloc(256,sizeof(char));
    if(buff == NULL) {
        printf("Erreur dans l'allocation mémoire d'un string\n");
        return NULL;
    }
    char *tempKeyToStr = key_to_str(pro->pKey);
    char *tempSignToStr = signature_to_str(pro->sign);
    sprintf(buff,"%s %s %s", tempKeyToStr, pro->mess,tempSignToStr);
    char *retStr = (char*)malloc(sizeof(char)*strlen(buff)+1);
    if(retStr == NULL) {
        printf("Erreur dans l'allocation mémoire d'un string\n");
        return NULL;
    }
    strcpy(retStr,buff);
    free(buff);
    free(tempSignToStr);
    free(tempKeyToStr);
    return retStr;
}

Protected* str_to_protected(char* str) {
    // Génère une déclaration signée à partir de sa représentation chaine de caractere
    Protected* retPro = (Protected*)malloc(sizeof(Protected));
    if(retPro == NULL) {
        printf("Erreur dans l'allocation mémoire d'un protected\n");
        return NULL;
    }
    char strkey[30];
    char mess[30];
    char strsign[50];
    if(sscanf(str,"%s %s %s", strkey,mess,strsign) != 3) {
        printf("[str_to_protected] Erreur dans le format de la chaine de caractère \n");
        return NULL;
    }
    retPro->pKey = str_to_key(strkey);
    retPro->mess = strdup(mess);
    retPro->sign = str_to_signature(strsign);
    return retPro;
}

void freeProtected(Protected* pr) {
    // On libère une déclaration signée, en libérant tous les champs s'ils sont non null
    if(pr == NULL) {
        return;
    }
    if(pr->pKey != NULL) {
        free(pr->pKey);
    }
    if(pr->mess != NULL) {
        free(pr->mess);
    }
    if(pr->sign != NULL) {
        freeSignature(pr->sign);
    }
    free(pr);
}
