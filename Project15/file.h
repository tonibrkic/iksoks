#ifndef FILE_H
#define FILE_H

#define MAX_NAME 50

typedef enum {
    NOVA_IGRA = 1,
    LJESTVICA,
    AZURIRAJ,
    BRISI,
    IZLAZ
} MenuOption;

typedef struct {
    char ime[MAX_NAME];
    int pobjede;
} Rezultat;

void spremiRezultat(const char* ime);
void ucitajRezultate();
void prikaziLjestvicu();
void obrisiRezultate();
void azurirajRezultat();
void sortirajPoBrojuPobjeda();

#endif
