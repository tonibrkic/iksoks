#define _CRT_SECURE_NO_WARNINGS
#include "file.h"

static const char* FILE_NAME = "results.txt";

Rezultat* rezultati = NULL;
int brojRezultata = 0;
static int kapacitet = 0;

static inline void safeFree(void** ptr) {
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}

static int usporediPobjede(const void* a, const void* b) {
    return ((Rezultat*)b)->pobjede - ((Rezultat*)a)->pobjede;
}

static int usporediImena(const void* a, const void* b) {
    return strcmp(((Rezultat*)a)->ime, ((Rezultat*)b)->ime);
}

static int povecajKapacitet() {
    int noviKapacitet = kapacitet == 0 ? 10 : kapacitet * 2;
    Rezultat* temp = realloc(rezultati, noviKapacitet * sizeof(Rezultat));
    if (!temp) {
        perror("Greska pri alokaciji memorije");
        return 0;
    }
    rezultati = temp;
    kapacitet = noviKapacitet;
    return 1;
}

void ucitajRezultate() {
    FILE* f = fopen(FILE_NAME, "r");
    if (!f) {
        if (errno != ENOENT)
            perror("Greska pri otvaranju datoteke");
        brojRezultata = 0;
        return;
    }

    // Reset
    safeFree((void**)&rezultati);
    brojRezultata = 0;
    kapacitet = 0;

    char buffer[MAX_NAME + 20];
    while (fgets(buffer, sizeof(buffer), f)) {
        if (brojRezultata == kapacitet) {
            if (!povecajKapacitet()) break;
        }
        char* token = strtok(buffer, ",");
        if (!token) continue;
        strncpy(rezultati[brojRezultata].ime, token, MAX_NAME);
        token = strtok(NULL, "\n");
        if (!token) continue;
        rezultati[brojRezultata].pobjede = atoi(token);
        brojRezultata++;
    }

    if (ferror(f))
        perror("Greska pri citanju datoteke");

    fclose(f);
}

void spremiRezultateUdatoteku() {
    FILE* f = fopen(FILE_NAME, "w");
    if (!f) {
        perror("Greska pri otvaranju datoteke za pisanje");
        return;
    }

    for (int i = 0; i < brojRezultata; i++) {
        fprintf(f, "%s,%d\n", rezultati[i].ime, rezultati[i].pobjede);
    }

    if (ferror(f))
        perror("Greska pri pisanju datoteke");

    fclose(f);
}

void spremiRezultat(const char* ime) {
    if (!ime) return;
    ucitajRezultate();

    // Koristi bsearch
    Rezultat trazeni;
    strncpy(trazeni.ime, ime, MAX_NAME);
    rezultati[brojRezultata].pobjede = 0; // da bsearch ne baca smece
    Rezultat* found = bsearch(&trazeni, rezultati, brojRezultata, sizeof(Rezultat), usporediImena);

    if (found) {
        found->pobjede++;
    }
    else {
        if (brojRezultata == kapacitet) {
            if (!povecajKapacitet()) return;
        }
        strncpy(rezultati[brojRezultata].ime, ime, MAX_NAME);
        rezultati[brojRezultata].pobjede = 1;
        brojRezultata++;
    }

    // Sortiraj po imenu da bsearch radi
    qsort(rezultati, brojRezultata, sizeof(Rezultat), usporediImena);

    spremiRezultateUdatoteku();
}

void sortirajPoBrojuPobjeda() {
    ucitajRezultate();
    qsort(rezultati, brojRezultata, sizeof(Rezultat), usporediPobjede);
}

void prikaziLjestvicu() {
    if (brojRezultata == 0) {
        printf("Ljestvica je prazna.\n");
        return;
    }
    printf("\n-- LJESTVICA --\n");
    ispisiRekurzivno(0);
}

void ispisiRekurzivno(int i) {
    if (i >= brojRezultata) return;
    printf("%d. %s - %d pobjeda\n", i + 1, rezultati[i].ime, rezultati[i].pobjede);
    ispisiRekurzivno(i + 1);
}

void obrisiRezultate() {
    safeFree((void**)&rezultati);
    brojRezultata = 0;
    kapacitet = 0;
    if (remove(FILE_NAME) == 0)
        printf("Svi rezultati obrisani.\n");
    else
        perror("Greska pri brisanju datoteke");
}

void obrisiJedanRezultat() {
    ucitajRezultate();
    if (brojRezultata == 0) {
        printf("Nema rezultata za brisanje.\n");
        return;
    }
    sortirajPoBrojuPobjeda();
    prikaziLjestvicu();

    int izbor;
    printf("Unesi broj rezultata za brisanje: ");
    if (scanf("%d", &izbor) != 1 || izbor < 1 || izbor > brojRezultata) {
        printf("Neispravan unos.\n");
        while (getchar() != '\n'); // clear buffer
        return;
    }
    izbor--;

    for (int i = izbor; i < brojRezultata - 1; i++) {
        rezultati[i] = rezultati[i + 1];
    }
    brojRezultata--;
    spremiRezultateUdatoteku();
    printf("Rezultat obrisan.\n");
}

void azurirajRezultat() {
    ucitajRezultate();
    if (brojRezultata == 0) {
        printf("Nema rezultata za azuriranje.\n");
        return;
    }
    sortirajPoBrojuPobjeda();
    prikaziLjestvicu();

    int izbor;
    printf("Unesi broj rezultata za azuriranje: ");
    if (scanf("%d", &izbor) != 1 || izbor < 1 || izbor > brojRezultata) {
        printf("Neispravan unos.\n");
        while (getchar() != '\n');
        return;
    }
    izbor--;

    int novePobjede;
    printf("Unesi novi broj pobjeda za %s: ", rezultati[izbor].ime);
    if (scanf("%d", &novePobjede) != 1 || novePobjede < 0) {
        printf("Neispravan broj pobjeda.\n");
        while (getchar() != '\n');
        return;
    }

    rezultati[izbor].pobjede = novePobjede;
    spremiRezultateUdatoteku();
    printf("Rezultat azuriran.\n");
}

void dodajRezultat() {
    ucitajRezultate();
    char novoIme[MAX_NAME];
    int novePobjede;

    printf("Unesi ime novog igraca: ");
    if (scanf("%49s", novoIme) != 1) {
        printf("Neispravan unos imena.\n");
        while (getchar() != '\n');
        return;
    }
    printf("Unesi broj pobjeda: ");
    if (scanf("%d", &novePobjede) != 1 || novePobjede < 0) {
        printf("Neispravan broj pobjeda.\n");
        while (getchar() != '\n');
        return;
    }

    Rezultat trazeni;
    strncpy(trazeni.ime, novoIme, MAX_NAME);
    Rezultat* found = bsearch(&trazeni, rezultati, brojRezultata, sizeof(Rezultat), usporediImena);

    if (found) {
        printf("Igrac vec postoji. Azuriram broj pobjeda.\n");
        found->pobjede = novePobjede;
    }
    else {
        if (brojRezultata == kapacitet) {
            if (!povecajKapacitet()) return;
        }
        strncpy(rezultati[brojRezultata].ime, novoIme, MAX_NAME);
        rezultati[brojRezultata].pobjede = novePobjede;
        brojRezultata++;
    }
    qsort(rezultati, brojRezultata, sizeof(Rezultat), usporediImena);
    spremiRezultateUdatoteku();
    printf("Novi rezultat dodan.\n");
}

void copyFile(const char* izvor, const char* odrediste) {
    FILE* fIzvor = fopen(izvor, "rb");
    if (!fIzvor) {
        perror("Greska pri otvaranju izvora");
        return;
    }
    FILE* fOdrediste = fopen(odrediste, "wb");
    if (!fOdrediste) {
        perror("Greska pri otvaranju odredista");
        fclose(fIzvor);
        return;
    }

    char buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), fIzvor)) > 0) {
        if (fwrite(buffer, 1, bytesRead, fOdrediste) != bytesRead) {
            perror("Greska pri pisanju datoteke");
            break;
        }
    }

    if (ferror(fIzvor))
        perror("Greska pri citanju izvora");

    fclose(fIzvor);
    fclose(fOdrediste);
}

Rezultat* nadjiRezultat(const char* ime) {
    ucitajRezultate();
    if (brojRezultata == 0) return NULL;

    Rezultat trazeni;
    strncpy(trazeni.ime, ime, MAX_NAME);
    return bsearch(&trazeni, rezultati, brojRezultata, sizeof(Rezultat), usporediImena);
}

void oslobodiMemoriju() {
    safeFree((void**)&rezultati);
    brojRezultata = 0;
    kapacitet = 0;
}
