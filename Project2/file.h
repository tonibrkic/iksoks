#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_NAME 50

typedef enum {
    NOVA_IGRA = 1,
    LJESTVICA,
    AZURIRAJ,
    BRISI,
    DODAJ,
    IZLAZ
} MenuOption;

typedef struct {
    char ime[MAX_NAME];
    int pobjede;
} Rezultat;

extern Rezultat* rezultati;
extern int brojRezultata;

void spremiRezultat(const char* ime);
void ucitajRezultate();
void prikaziLjestvicu();
void obrisiRezultate();
void obrisiJedanRezultat();
void azurirajRezultat();
void sortirajPoBrojuPobjeda();
void dodajRezultat();
void copyFile(const char* izvor, const char* odrediste);
Rezultat* nadjiRezultat(const char* ime);
void oslobodiMemoriju();
void ispisiRekurzivno(int i);

#endif
