#include "header/block.h"
#include "header/protected.h"
#include <openssl/sha.h>
#include <time.h>
#include <string.h>

void fprintblock(FILE *f, Block *block) {
    // Le fichier passé en paramètre est supposé ouvert.
    if(f == NULL) {
        printf("[fprintblock] Fichier en argument invalide \n");
        return;
    }
    if(block == NULL) {
        printf("[fprintblock] Block en argument invalide \n");
        return;
    }

    // On convertit notre bloc en chaine de caractère puis nous l'écrivons dans notre fichier en paramètre.
    char *blockstr = block_to_str(block);
    fprintf(f,"%s", blockstr);
    free(blockstr);
}

Block *freadblock(FILE *f) {
    // On suppose le fichier f ouvert avant l'appel de freadblock
    if(f == NULL) {
        printf("[freadblock] Fichier invalide\n");
        return NULL;
    }

    // On alloue en mémoire le block que l'on souhaite retourné.
    Block *retBlock = (Block *)malloc(sizeof(Block));
    if(retBlock == NULL) {
        printf("[freadblock] Erreur d'allocation mémoire \n");
        return NULL;
    }
    
    // Lecture dans le fichier :
    /* Nous avons choisi un format particulier pour nos blocks ainsi vous avons les champs qui sont : 
    - Auteur
    - Liste de déclaration
    - Hash courant
    - Hash du bloc précédant
    - nonce
    Ce qui fait que notre lecture est un peu capillotractée */
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

    /* On fait une boucle qui tourne tant que nous detectons dans une ligne du fichier le format d'une déclaration signée, lorsque nous avons vu toute les déclarations 
    Nous avons dans notre buffer le hash courant, pour savoir que nous en sommes au hash courant, nous utilisons la sortie anticipée de notre fonction str_to_protected qui renvoie null
    lorsque le format d'un protected n'est pas détecté */
    while(1) {
        fgets(buff,256,f);
        buff[strlen(buff)-1] = '\n';
        if(fgets(buff,256,f) != NULL) {
            buff[strlen(buff)-1] = '\0';
            Protected *pr = str_to_protected(buff);
            if(pr == NULL) {  // Nous détectons ici le hash courant.
                currentHash = (unsigned char*)buff;
                break;
            }
            CP = endInsertCellProtected(pr,CP); 
        }
    }

    // A présent il ne reste plus que le hash du block précédant et le nonce.
    fgets(buff,256,f);
    buff[strlen(buff)-1] = '\0';
    previousHash = (unsigned char*)strdup(buff);
    fgets(buff,256,f);
    sscanf(buff,"%d", &nonce);

    // Enfin nous finissons d'initialiser les champs du bloc.
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

    sprintf(retStr,"%s %s\n %s\n %s\n %d\n", strKey,strCP,(unsigned char*)block->hash,(unsigned char*)block->previous_hash,block->nonce);
    free(strKey);
    free(strCP);
    return retStr;
}

char *block_to_str2(Block *block) {
    // Cette version de la fonction est utilisée pour récuperer un string contenant les données nécéssaires pour hacher un bloc.
    if(block == NULL) {
        printf("[block_to_str] Argument invalide\n");
        return NULL;
    }
    char *strCP = (char *)calloc(10000,sizeof(char));
    char *strKey = key_to_str(block->author);

    CellProtected *pointerCP = block->votes;

    // On parcourt tous les éléments de la liste de votes qu'on ajoute à la chaine strCP.
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
    // On calcul le hash du block tant qu'on trouve pas de hash dont la représentation héxadécimal ne commence pas par d zéros.
    while(true) {
        unsigned char *strCP = (unsigned char*)block_to_str2(b);
        unsigned char *hash = (unsigned char*)str_to_hash(strCP);
        showHash(strCP);
        if(compte_zeros(hash,d)) { // Si hash comporte d zeros alors compte_zeros retourne true et on a trouvé notre hash
            b->hash = (unsigned char*)str_to_hash(strCP);
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
    // On utilise une comparaison bit à bit pour savoir si nous avons le bon nombre de 0,
    if(hash == NULL) {
        return false;
    } 
    bool flag = true;
    int i = 0;

    while(d>0) { // Tant que d est positif on itère : 
        if(d == 1) {
            if(hash[i] & 0b11110000) { // Si d == 1 on cherche un zéro, donc à ce que les bits de poids forts de hash[i] soient nuls, ainsi la représentation hexadécimal de hash[i] commence par 0
                flag = false;
            }
            break;
        }
        if(hash[i]) {  // Si d > 1 alors on souhaite avoir deux zéros pour hash[i] et donc que la totalité des bits de hash[i] soient nuls.
            flag = false;
            break; 
        }

        d -= 2; 
        i++;
    }
    // Si flag == false alors on a détecté un caractère différent de zéros dans les d premiers caractères.
    return flag;
}

unsigned char* str_to_hash(const char *hash) {
    unsigned char* str = SHA256(hash,strlen(hash),0);
    unsigned char tmp[8]; 
    unsigned char *ret = (unsigned char *)malloc(256 * sizeof(unsigned char));
    if(ret == NULL) {
        printf("[str_to_hash] Erreur d'allocation mémoire \n");
        exit(EXIT_FAILURE);
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
    // On souhaite afficher la représentation hexadecimal de hash. 
    unsigned char* str = SHA256(hash,strlen(hash),0);
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        printf("%02x", str[i]);
    }
    putchar('\n');
}

bool verify_block(Block *b, int d) {
    // On vérifie la validité d'un bloc, pour cela on compare la valeur hash stocké dans le block avec la valeur obtenu en le hachant avec le bon nonce, vérifiant également la condition sur d.
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
    // Si b est null -> rien a faire 
    if(b == NULL) {
        printf("[verify_block] Erreur block null");
        return;
    }

    // Dans un premier temps on supprime tous les votes puis on supprime tous les champs et la structure.
    deleteListCellProtected(b->votes);
    free(b->previous_hash);
    free(b);
}
