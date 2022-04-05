#include <time.h>
#include "header/rsa.h"
#include "header/keys.h"
#include "header/protected.h"
#include "header/sign.h"
#include <stdbool.h>

int main(int argc, char** argv) {
    FILE *f = fopen("temps/sortie_vitesse_modpow.txt", "w");
    if(f == NULL) {
        printf("Erreur d ouverture du fichier de mesure.\n");
        return -1;
    }
    clock_t temps_initial;
    clock_t temps_final;
    int i;
    double temps_cpu;
    for(i = 1000; i < 30000; i = i + 200) {
        temps_initial = clock();
        modpow_naive(100,i,15);
        temps_final = clock();
        temps_cpu = ((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;
        fprintf(f,"%d %f",i,temps_cpu);
        temps_initial = clock();
        modpow(100,i,15);
        temps_final = clock();
        temps_cpu = ((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;
        fprintf(f," %f\n",temps_cpu);
    }
}
