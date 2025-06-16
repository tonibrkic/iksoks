#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "file.h"
#include <stdio.h>
#include <string.h>

#define SIZE 3

static inline void ispisPloca(char ploca[SIZE][SIZE]) {
    printf("\n   0   1   2\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
        for (int j = 0; j < SIZE; j++) {
            printf(" %c ", ploca[i][j]);
            if (j < SIZE - 1) printf("|");
        }
        printf("\n");
        if (i < SIZE - 1) printf("  ---+---+---\n");
    }
}

static int provjeriPobjedu(char ploca[SIZE][SIZE], char tren) {
    for (int i = 0; i < SIZE; i++) {
        if (ploca[i][0] == tren && ploca[i][1] == tren && ploca[i][2] == tren) return 1;
        if (ploca[0][i] == tren && ploca[1][i] == tren && ploca[2][i] == tren) return 1;
    }
    if (ploca[0][0] == tren && ploca[1][1] == tren && ploca[2][2] == tren) return 1;
    if (ploca[0][2] == tren && ploca[1][1] == tren && ploca[2][0] == tren) return 1;
    return 0;
}

void pokreniIgru() {
    char ploca[SIZE][SIZE];
    memset(ploca, ' ', sizeof(ploca));
    char igrac1[MAX_NAME], igrac2[MAX_NAME];
    printf("Unesite ime prvog igraca (X): "); scanf("%49s", igrac1);
    printf("Unesite ime drugog igraca (O): "); scanf("%49s", igrac2);
    char tren = 'X';
    int potezi = 0, pobjeda = 0;

    while (potezi < SIZE * SIZE && !pobjeda) {
        ispisPloca(ploca);

        int r, c;
        printf("%s (%c), unesite red i kolonu: ", tren == 'X' ? igrac1 : igrac2, tren);
        if (scanf("%d%d", &r, &c) != 2) {
            printf("Neispravan unos!\n");
            while (getchar() != '\n');
            continue;
        }
        if (r < 0 || r >= SIZE || c < 0 || c >= SIZE || ploca[r][c] != ' ') {
            printf("Neispravan potez!\n");
            continue;
        }
        ploca[r][c] = tren;
        potezi++;

        if (provjeriPobjedu(ploca, tren)) {
            ispisPloca(ploca);
            printf("\nPobjednik je %s!\n", tren == 'X' ? igrac1 : igrac2);
            spremiRezultat(tren == 'X' ? igrac1 : igrac2);
            pobjeda = 1;
            break;
        }
        tren = (tren == 'X') ? 'O' : 'X';
    }
    if (!pobjeda) {
        ispisPloca(ploca);
        printf("\nIgra zavrsena bez pobjednika.\n");
    }
}
