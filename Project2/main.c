#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "game.h"
#include "file.h"

int main() {
    int izbor;
    do {
        printf("\n--- IKS OKS IZBORNIK ---\n");
        printf("1. Nova igra\n");
        printf("2. Prikaz ljestvice\n");
        printf("3. Azuriranje rezultata\n");
        printf("4. Brisanje rezultata\n");
        printf("5. Dodaj novi rezultat\n");
        printf("6. Izlaz\nOdabir: ");
        if (scanf("%d", &izbor) != 1) {
            printf("Neispravan unos.\n");
            while (getchar() != '\n');
            continue;
        }

        switch ((MenuOption)izbor) {
        case NOVA_IGRA: pokreniIgru(); break;
        case LJESTVICA: sortirajPoBrojuPobjeda(); prikaziLjestvicu(); break;
        case AZURIRAJ: azurirajRezultat(); break;
        case BRISI: obrisiJedanRezultat(); break;
        case DODAJ: dodajRezultat(); break;
        case IZLAZ: printf("Izlaz iz programa.\n"); break;
        default: printf("Nepoznata opcija.\n"); break;
        }
    } while (izbor != IZLAZ);

    oslobodiMemoriju();
    return 0;
}
