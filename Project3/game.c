#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "file.h"

#define SIZE 3

void pokreniIgru() {
    char ploca[SIZE][SIZE];
    memset(ploca, ' ', sizeof(ploca));
    char igrac1[MAX_NAME], igrac2[MAX_NAME];
    printf("Unesite ime prvog igraca (X): "); scanf("%s", igrac1);
    printf("Unesite ime drugog igraca (O): "); scanf("%s", igrac2);
    char tren = 'X';
    int potezi = 0, pobjeda = 0;

    while (potezi < SIZE * SIZE && !pobjeda) {
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

        int r, c;
        printf("%s (%c), unesite red i kolonu: ", tren == 'X' ? igrac1 : igrac2, tren);
        scanf("%d%d", &r, &c);
        if (r < 0 || r >= SIZE || c < 0 || c >= SIZE || ploca[r][c] != ' ') {
            printf("Neispravan potez!\n");
            continue;
        }
        ploca[r][c] = tren;
        potezi++;

        for (int i = 0; i < SIZE; i++) {
            if (ploca[i][0] == tren && ploca[i][1] == tren && ploca[i][2] == tren) pobjeda = 1;
            if (ploca[0][i] == tren && ploca[1][i] == tren && ploca[2][i] == tren) pobjeda = 1;
        }
        if (ploca[0][0] == tren && ploca[1][1] == tren && ploca[2][2] == tren) pobjeda = 1;
        if (ploca[0][2] == tren && ploca[1][1] == tren && ploca[2][0] == tren) pobjeda = 1;

        if (pobjeda) {
            printf("\nPobjednik je %s!\n", tren == 'X' ? igrac1 : igrac2);
            spremiRezultat(tren == 'X' ? igrac1 : igrac2);
            break;
        }
        tren = (tren == 'X') ? 'O' : 'X';
    }
    if (!pobjeda) printf("\nIgra zavrsena bez pobjednika.\n");
}
