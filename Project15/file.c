#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

#define FILE_NAME "results.txt"
static Rezultat rezultati[100];
static int brojRezultata = 0;

void spremiRezultat(const char* ime) {
    ucitajRezultate();
    int postoji = 0;
    for (int i = 0; i < brojRezultata; i++) {
        if (strcmp(rezultati[i].ime, ime) == 0) {
            rezultati[i].pobjede++;
            postoji = 1;
            break;
        }
    }
    if (!postoji) {
        strncpy(rezultati[brojRezultata].ime, ime, MAX_NAME);
        rezultati[brojRezultata].pobjede = 1;
        brojRezultata++;
    }
    FILE* f = fopen(FILE_NAME, "w");
    if (!f) {
        perror("Greska pri otvaranju datoteke");
        return;
    }
    for (int i = 0; i < brojRezultata; i++) {
        fprintf(f, "%s,%d\n", rezultati[i].ime, rezultati[i].pobjede);
    }
    fclose(f);
}

void ucitajRezultate() {
    FILE* f = fopen(FILE_NAME, "r");
    if (!f) return;
    brojRezultata = 0;
    while (fscanf(f, "%49[^,],%d\n", rezultati[brojRezultata].ime, &rezultati[brojRezultata].pobjede) == 2) {
        brojRezultata++;
    }
    fclose(f);
}

int usporediPobjede(const void* a, const void* b) {
    return ((Rezultat*)b)->pobjede - ((Rezultat*)a)->pobjede;
}

void sortirajPoBrojuPobjeda() {
    ucitajRezultate();
    qsort(rezultati, brojRezultata, sizeof(Rezultat), usporediPobjede);
}

void prikaziLjestvicu() {
    printf("\n-- LJESTVICA --\n");
    for (int i = 0; i < brojRezultata; i++) {
        printf("%d. %s - %d pobjeda\n", i + 1, rezultati[i].ime, rezultati[i].pobjede);
    }
}

void obrisiRezultate() {
    if (remove(FILE_NAME) == 0)
        printf("Rezultati obrisani.\n");
    else
        perror("Greska pri brisanju datoteke");
}

void azurirajRezultat() {
    ucitajRezultate();
    prikaziLjestvicu();
    int izbor;
    printf("Unesi broj rezultata za azuriranje: ");
    scanf("%d", &izbor);
    if (izbor < 1 || izbor > brojRezultata) {
        printf("Neispravan unos.\n");
        return;
    }
    izbor--;
    char novoIme[MAX_NAME];
    int novePobjede;
    printf("Unesi novo ime za igraca: ");
    scanf("%s", novoIme);
    printf("Unesi novi broj pobjeda: ");
    scanf("%d", &novePobjede);
    strncpy(rezultati[izbor].ime, novoIme, MAX_NAME);
    rezultati[izbor].pobjede = novePobjede;

    FILE* f = fopen(FILE_NAME, "w");
    if (!f) {
        perror("Greska pri pisanju datoteke");
        return;
    }
    for (int i = 0; i < brojRezultata; i++) {
        fprintf(f, "%s,%d\n", rezultati[i].ime, rezultati[i].pobjede);
    }
    fclose(f);
    printf("Azurirano.\n");
}
