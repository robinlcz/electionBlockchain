#include "header/block.h"
#include "header/protected.h"
#include <openssl/sha.h>
#include <time.h>
#include <string.h>

void fprintblock(FILE *f, Block *block) {
    if(f == NULL) {
        printf("[fprintblock] Fichier en argument invalide \n");
        return;
    }
    if(block == NULL) {
        printf("[fprintblock] Block en argument invalide \n");
        return;
    }
    char *blockstr = block_to_str(block);
    fprintf(f,"%s", blockstr);
    free(blockstr);
}

Block *freadblock(FILE *f) {
    if(f == NULL) {
        printf("[freadblock] Fichier invalide\n");
        return NULL;
    }

    Block *retBlock = (Block *)malloc(sizeof(Block));
    if(retBlock == NULL) {
        printf("[freadblock] Erreur d'allocation mémoire \n");
        return NULL;
    }
    
    char *buff = (char*)calloc(256,sizeof(char));
    if(buff == NULL) {
        printf("[freadblock] Erreur d'allocation buffer\n");
    }
    Key* authorKey;
    unsigned char* previousHash;
    unsigned char* currentHash;
    int nonce;
    CellProtected *CP = NULL;

    // Récupération de la clé de l'auteur
    fgets(buff,256,f);
    buff[strlen(buff)-1] = '\0';
    authorKey = str_to_key(buff);

    while(1) {
        fgets(buff,256,f);
        buff[strlen(buff)-1] = '\n';
        if(fgets(buff,256,f) != NULL) {
            buff[strlen(buff)-1] = '\0';
            Protected *pr = str_to_protected(buff);
            if(pr == NULL) {
                currentHash = (unsigned char*)buff;
                break;
            }

            CP = headInsertCellProtected(pr,CP); 
        }
    }

    // A présent il ne reste plus que le hash du block précédant et le nonce.
    fgets(buff,256,f);
    buff[strlen(buff)-1] = '\0';
    previousHash = (unsigned char*)strdup(buff);
    fgets(buff,256,f);
    sscanf(buff,"%d", &nonce);

    retBlock->author = authorKey;
    retBlock->votes = CP;
    retBlock->hash = strdup(currentHash);
    retBlock->previous_hash = strdup(previousHash);
    retBlock->nonce = nonce;

    free(buff);
    return retBlock;
}

char *block_to_str(Block *block) {
    if(block == NULL) {
        printf("[block_to_str] Argument invalide\n");
        return NULL;
    }
    char *strCP = (char *)calloc(10000,sizeof(char));
    char *strKey = key_to_str(block->author);

    CellProtected *pointerCP = block->votes;

    while(pointerCP) {
        char *temp = protected_to_str(pointerCP->data);
        sprintf(strCP,"%s\n%s", strCP,temp);
        pointerCP = pointerCP->next;
        free(temp);
    }
    char *retStr = (char *)malloc(sizeof(char)*11000);
    
    sprintf(retStr,"%s %s\n %s\n %s\n %d\n", strKey,strCP,block->hash,block->previous_hash,block->nonce);
    free(strKey);
    free(strCP);
    return retStr;
}

char *block_to_str2(Block *block) {
    if(block == NULL) {
        printf("[block_to_str] Argument invalide\n");
        return NULL;
    }
    char *strCP = (char *)calloc(10000,sizeof(char));
    char *strKey = key_to_str(block->author);

    CellProtected *pointerCP = block->votes;

    while(pointerCP) {
        char *temp = protected_to_str(pointerCP->data);
        sprintf(strCP,"%s\n%s", strCP,temp);
        pointerCP = pointerCP->next;
        free(temp);
    }
    char *retStr = (char *)malloc(sizeof(char)*11000);
    
    sprintf(retStr,"%s %s\n %s\n %d\n", strKey,strCP,block->previous_hash,block->nonce);
    free(strKey);
    free(strCP);
    return retStr;
}


void compute_proof_of_work(Block *b, int d) {
    if(b == NULL) {
        printf("[compute_proof_of_work] Adresse du block non valide\n");
    }
    b->nonce = 312000;

    while(true) {
        unsigned char *strCP = (unsigned char*)block_to_str2(b);
        unsigned char *hash = str_to_hash(strCP);
        showHash(strCP);
        if(compte_zeros(hash,d) == true) {
            b->hash = strdup(str_to_hash(hash));
            printf("Hash trouvé : %d \n", b->nonce);
            free(strCP);
            break;
        }
        free(strCP);
        b->nonce++;
    }
}

bool compte_zeros(unsigned char* hash, int d) {
    // True si les d premiers caractère de la représentations hexadécimal de hash sont des 0
    if(hash == NULL) {
        return false;
    } 
    bool flag = true;
    int i = 0;

    while(d>0) {
        if(d == 1) {
            if(hash[i] & 0b11110000) {
                flag = false;
            }
            break;
        }
        if(hash[i]) {
            flag = false;
            break; 
        }

        d -= 2; 
        i++;
    }

    return flag;
}

unsigned char* str_to_hash(const char *hash) {
    unsigned char* str = SHA256(hash,strlen(hash),0);
    unsigned char tmp[8]; 
    unsigned char *ret = (unsigned char *)malloc(256 * sizeof(unsigned char));
    if(ret == NULL) {
        printf("[str_to_hash] Erreur d'allocation mémoire \n");
        return NULL;
    }

    ret[0] = '\0';

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(tmp, "%02x", str[i]);
        strcat(str,tmp);
    }
    strcat(str,ret);
    free(ret);
    return str;
}

void* showHash(const char *hash) {
    unsigned char* str = SHA256(hash,strlen(hash),0);
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        printf("%02x", str[i]);
    }
    putchar('\n');
}

bool verify_block(Block *b, int d) {
    unsigned char* bloStr = block_to_str2(b);
    showHash(bloStr);
    free(bloStr);
    bloStr = SHA256(bloStr,strlen(bloStr),0);
    if(strcmp(bloStr,b->hash) == 0 && compte_zeros(bloStr,d)) {
        return true;
    }
    return false;
}

void delete_block(Block *b) {
    if(b == NULL) {
        printf("[verify_block] Erreur block null");
        return;
    }
    deleteListCellProtected(b->votes);
    if(b->votes != NULL) {
        CellProtected *cour = b->votes;
        CellProtected *pred = NULL;
        while(cour) {
            pred = cour;
            cour = cour->next;
            free(pred);
        }
    }
    free(b->hash);
    free(b->previous_hash);
    free(b);
}