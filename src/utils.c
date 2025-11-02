#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "utils.h"

void drawTitle(const char *string) {
    int len = strlen(string);
    for(int i = 0; i < len+4; i++) putchar('*');
    printf("\n* %20s *\n", string);
    for(int i = 0; i < len+4; i++) putchar('*');
    putchar('\n');
}

void clearScreen() {
    system("clear");
}

void clearInput() {
    int ch;
    while((ch = getchar()) != EOF && ch != '\n');
}

void readString(char *string, int size) {
    clearInput();
    while (1) {
        if (fgets(string, size, stdin) == NULL) {
            puts("Errore di lettura, riprova");
            clearInput();
            continue;
        }

        char *newline = strchr(string, '\n');
        if (newline) {
            *newline = '\0';
        } else {
            puts("La stringa è troppo lunga, riprova");
            clearInput();
            continue;
        }

        if (string[0] == '\0') {
            puts("La stringa non può essere vuota, riprova");
            continue;
        }

        break;
    }
}

void story() {
    printf("Il mondo è in rovina sotto l’assedio dell’armata oscura.\n"
        "Tu, semplice abitante scelto dal villaggio, devi fermare \n"
        "il Signore Oscuro e riportare la luce, affrontando terre \n"
        "devastate e nemici implacabili.\n");
}
