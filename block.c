#include "header/block.h"
#include "header/protected.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

void fprintblock(FILE *f, Block *block) {
    if(f == NULL) {
        printf("[fprintblock] Fichier en argument invalide \n");
        return;
    }
    if(block == NULL) {
        printf("[fprintblock] Block en argument invalide \n");
        return;
    }
    printf("(%lx,%lx)\n)",block->author->keyValue, block->author->N);
    fprintCellProtected(f,block->votes);
    printf("%s\n", block->hash);
    printf("%s\n", block->previous_hash);
    printf("%d\n", block->nonce);
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
    char *buffKeyVoter = (char*)calloc(256,sizeof(char));
    char *buffKeyCandidate = (char*)calloc(256,sizeof(char));
    char *buffSign = (char*)calloc(256,sizeof(char));
    CellProtected *CP = NULL;

    // Récupération de la clé de l'auteur
    fgets(buff,256,f);
    buff[strlen(buff)-1] = '\0';
    retBlock->author = str_to_key(buff);

    // Récupération des déclarations de votes signées : 
    while(1) {
        fgets(buff,256,f);
        buff[strlen(buff)-1] = '\0';
        if(sscanf("%s %s %s", buffKeyVoter,buffKeyCandidate,buffSign) == 3) {
            Protected *pr = init_protected(str_to_key(buffKeyVoter),buffKeyCandidate,str_to_signature(buffSign));
            headInsertCellProtected(pr,CP);
        } else {
            // Si le fichier a le bon format en cas de non vérification de la condition sur le sscanf, cela implique que nous avons récuperer le premier hash
            retBlock->hash = strdup((unsigned char*)buff);
            break;
        }
    }

    // A présent il ne reste plus que le hash du block précédant et le nonce.
    fgets(buff,256,f);
    buff[strlen(buff)-1] = '\0';
    retBlock->previous_hash = strdup((unsigned char*)buff);

    fgets(buff,256,f);
    buff[strlen(buff)-1] = '\0';
    sscanf(buff, "%d", retBlock->nonce);

    free(buff);
    free(buffKeyVoter);
    free(buffKeyCandidate);
    free(buffSign);
    return retBlock;
}

char *block_to_str(Block *block) {
    if(block == NULL) {
        printf("[block_to_str] Argument invalide\n");
        return NULL;
    }
    char *strCP = (char *)malloc(sizeof(char)*10000);
    CellProtected *pointerCP = block->votes;
    while(pointerCP) {
        char *temp = protected_to_str(pointerCP->data);
        strcat(strCP,temp);
        free(temp);
        pointerCP = pointerCP->next;
    }
    char *retStr = (char *)malloc(sizeof(char)*256);
    char *buff = key_to_str(block->author);
    sprintf(retStr,"%s %s %s %d\n", buff, block->previous_hash,strCP,block->nonce);
    free(buff);
    return retStr;
}




int compute_proof_of_work(Block *b, int d) {
    b->nonce = 0;
    int cpt = 0; 
    while(1) {
        unsigned char *strCP = block_to_str(b);
        unsigned char *hash = SHA256(strCP,strlen(strCP),0);
        unsigned char temp[strlen(hash)];
        strcpy(temp,hash);
        temp[4] = '\0';
        if(strcmp(temp,"0000") == 0 || cpt == 150) {
            printf("Hash trouvé : %s\n", hash);
            break;
        }
        b->nonce++;
        free(strCP);
    }
    return b->nonce;
}