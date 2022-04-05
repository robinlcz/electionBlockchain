#include "header/rsa.h"

void generate_key_values(long p, long q, long* n, long* s, long* u) {
    // Generate key values from two prime number p and q
    *n = p*q;
    long t = (p - 1)*(q - 1);
    long tmp,v = 1;
    while(1) {
        tmp = rand_long(3,t);
        if(extended_gcd(tmp,t,u,&v) == 1 && (*u >0)) {
            *s = tmp;
            break;
        }
    }
}

long* encrypt(char *chaine, long s, long n) {
    // encrypt the string called chaine using the public key s
    if(chaine == NULL) {
        return NULL;
    }
    char *tmp = chaine;
    long *res = malloc(sizeof(long)*(strlen(chaine)+1));
    int i = 0;
    while(*tmp) {
        res[i] = modpow(*tmp,s,n);
        i++;
        tmp++;
        if(*tmp  == '\0') {
            break;
        }
    }
    return res;
}

char *decrypt(long *crypted, int size, long u, long n) {
    // decrypt de crypted string using the private key u
    if(crypted == NULL) {
        return NULL;
    }
    char *res = malloc(sizeof(char)*size+1);
    for(int i = 0; i < size; i++) {
        res[i] = (char)modpow(crypted[i],u,n);
    }
    res[size] = '\0';
    return res;
}