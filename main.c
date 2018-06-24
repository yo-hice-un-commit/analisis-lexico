#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"

enum enumTokenType isWordReserved(char *word) {
    char *listOperador[] = {"if", "while"};
    char *listType[] = {"int"};
    int i;
    for (i = 0; i < 2; ++i) {
        if (strcmp(listOperador[i], word) == 0) {
            return OPERATOR;
        }
    }
    for (i = 0; i < 1; ++i) {
        if (strcmp(listType[i], word) == 0) {
            return TYPE;
        }
    }
    return UNDEFINED;
}

enum enumTokenType typeWord(char *word) {
    enum enumTokenType type;
    char *cleanSpace = trim(word);
    if (strlen(word) == 0) return UNDEFINED;
    if ((type = isWordReserved(cleanSpace))) {

    } else {
        type = IDENTIFICADOR;
    }
    free(cleanSpace);
    return type;
}

int nuevoToken(enum enumTokenType type, char *word, token **List) {
    token *newToken = malloc(sizeof(token));
    newToken->token = getToken(type);
    char *lexema = malloc(strlen(word));
    strcpy(lexema, word);
    newToken->lexema = lexema;
    newToken->sig = NULL;
    pushToken(&(*List), newToken);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Se require File \n");
        return 1;
    }
    int offset = 0;
    char letter;
    char word[1000];
    int wordPosition = 0;
    cleanWord(word);
    token *listToken = NULL;
    FILE *program = fopen(argv[1], "r");
    enum enumTokenType tipo;

    while (!feof(program)) {
        letter = (char) fgetc(program);
        if (letter == -1) {
            continue;
        }
        switch (letter) {
            case 32:
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken);
                }
                cleanWord(word);
                wordPosition = 0;
                continue;
            case 40:
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken);
                }
                nuevoToken(CARACTERPUNTUACION, "(", &listToken);
                cleanWord(word);
                wordPosition = 0;
                continue;
            case 44: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken);
                }
                nuevoToken(CARACTERPUNTUACION, ",", &listToken);
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
            case 41: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken);
                }
                nuevoToken(CARACTERPUNTUACION, ")", &listToken);
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
            case 123: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken);
                }
                nuevoToken(CARACTERPUNTUACION, "{", &listToken);
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
        }
        word[wordPosition] = letter;
        wordPosition++;

    }
    showResult(listToken);
    return 0;
}