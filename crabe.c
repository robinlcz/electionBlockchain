#include "header/crabe.h"

bool is_prime_naive(long p) {
    // naive algorithm to check if p is a prime number
    for(int i = 3; i < p/2; i++) {
        if(p%i == 0) {
            return false;
        }
    }
    return true;
}

bool is_prime_miller(long p, int k) {
    // Test de primalité de Miller-Rabin
    if(p==2) {
        return true;
    }
    if (!(p & 1) || p <= 1) { // on verifie que p est impair et different de 1
        return false;
    }
    // On determine b et d :
    long b = 0;
    long d = p-1;
    while(!(d & 1)) { // Tant que d n'est pas impair
        d = d/2;
        b++;
    }
    // On genere k valeurs pour a , et on teste si c'est un temoin :
    long a;
    int i;
    for(i = 0; i < k; i++) {
        a = rand_long(2,p-1);
        if(witness(a,b,d,p)) {
            return false;
        }
    }
    return true;
}

long modpow_naive(long a, long m, long n) {
    // Algorithme d'exponentiation naif
    long cour = a;
    if(m == 0) {
        return 1;
    }
    long p = 1;
    while(p<m) {
        cour = cour*a%n;
        p++;
    }
    return cour;
}

long modpow(long a,long m,long n) {
    // Algorithme d'exponentiation rapide
    long tmp;
    if(m == 0) return 1;
    if(m == 1) return a%n;
    if(m%2 == 0) {
        tmp = modpow(a,m/2,n);
        return (tmp*tmp)%n;
    } else {
        tmp = modpow(a,(long)floorl(m/2),n);
        return (a*tmp*tmp)%n;
    }
}

bool witness(long a, long b, long d, long p) {
    long x = modpow(a,d,p);
    if(x == 1) {
        return false;
    }
    for(long i = 0; i < b; i++) {
        if(x == p-1) {
            return false;
        }
        x = modpow(x,2,p);
    }
    return true;
}

long rand_long(long low, long up) {
    return rand()%(up - low+1)+low;
}

long random_prime_number(int low_size, int up_size, int k) {
    long tmp;
    while(true) {
        tmp = rand_long(1<<low_size,(1<<up_size)-1);
        if(is_prime_miller(tmp,k) == true) {
            break;
        }
    }
    return tmp;
}

long extended_gcd(long s,long t,long* u, long* v) {
    if(s == 0) {
        *u = 0;
        *v = 1;
        return t;
    }
    long uPrim, vPrim;
    long gcd = extended_gcd(t%s,s,&uPrim,&vPrim);
    *u = vPrim-(t/s)*uPrim;
    *v = uPrim;
    return gcd;
}