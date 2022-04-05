#include "header/sign.h"

Signature* init_signature(long* content, int size) {
    Signature* retSign = malloc(sizeof(Signature));
    retSign->tab = content;
    retSign->tabSize = size;
    return retSign;
}

Signature* sign(char* mess, Key* sKey) {
    long* content = encrypt(mess,sKey->keyValue,sKey->N);
    return init_signature(content,strlen(mess));
}

char* signature_to_str(Signature* sgn) {
    char* result = malloc((10*sgn->tabSize*sizeof(char))+1);
    result[0] = '#';
    int pos = 1;
    char *buffer = (char *)calloc(156,sizeof(char));
    for(int i = 0; i < sgn->tabSize; i++) {
        sprintf(buffer,"%lx", sgn->tab[i]);
        for(int j = 0; j < strlen(buffer); j++) {
            result[pos] = buffer[j];
            pos++;
        }
        result[pos] = '#';
        pos++;
    }
    free(buffer);
    result[pos] = '\0';
    result = realloc(result, (pos+1)*sizeof(char));
    return result;
}

Signature* str_to_signature(char* str) {
    int len = strlen(str);
    long* content = (long*)malloc(sizeof(long)*len);
    int num = 0;
    char buffer[256];
    int pos = 0;
    for(int i = 0; i < len; i++) {
        if(str[i] != '#') {
            buffer[pos] = str[i];
            pos++;
        } else {
            if(pos!= 0) {
                buffer[pos] = '\0';
                sscanf(buffer, "%lx", &(content[num]));
                num++;
                pos = 0;
            }
        }
    }
    content = realloc(content,num*sizeof(long));
    return init_signature(content,num);
}

void freeSignature(Signature* sgn) {
    if(sgn == NULL) {
        return;
    }
    if(sgn->tab != NULL) {
        free(sgn->tab);
    }
    free(sgn);
}
