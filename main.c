#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"

enum enumTokenType isWordReserved(char *word) {
    char *listOperador[] = {"if", "while", "return", "else", ">"};
    char *listType[] = {"int"};
    int i;
    for (i = 0; i < 5; ++i) {
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
        if (isNumber(word)) {
            type = VALORCONSTANTE;
        } else {
            if (isString(word)) {
                type = VALORCONSTANTE;
            } else {
                type = IDENTIFICADOR;
            }
        }
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
        switch (letter) {
            case -1: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken);
                }
                cleanWord(word);
                wordPosition = 0;
                break;
            }
            case 10: {
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
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
            case 43:
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken);
                }
                cleanWord(word);
                wordPosition = 0;
                letter = (char) getc(program);
                if (letter == 43) {
                    nuevoToken(OPERATOR, "++", &listToken);
                    continue;
                } else {
                    nuevoToken(OPERATOR, "+", &listToken);
                }
                if (letter == 32) continue;
                break;
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
            case 45:
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken);
                }
                cleanWord(word);
                wordPosition = 0;
                letter = (char) getc(program);
                if (letter == 45) {
                    nuevoToken(OPERATOR, "--", &listToken);
                    continue;
                } else {
                    nuevoToken(OPERATOR, "-", &listToken);
                }
                if (letter == 32) continue;
                break;
            case 41: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken);
                }
                // si es una carga o una asignaciom

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
            case 125: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken);
                }
                nuevoToken(CARACTERPUNTUACION, "}", &listToken);
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
            case 60: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken);
                }
                nuevoToken(OPERATOR, "<", &listToken);
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
            case 61: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken);
                }
                cleanWord(word);
                wordPosition = 0;
                letter = (char) getc(program);
                if (letter == 61) {
                    nuevoToken(OPERATOR, "==", &listToken);
                    continue;
                } else {
                    nuevoToken(OPERADORCARGA, "=", &listToken);
                }
                if (letter == 32) continue;
                break;
            }
            case 62: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken);
                }
                nuevoToken(OPERATOR, ">", &listToken);
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
            case 59: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken);
                }
                nuevoToken(CARACTERPUNTUACION, ";", &listToken);
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
        }
        if (letter == -1) {
            continue;
        }
        word[wordPosition] = letter;
        wordPosition++;

    }
    showResult(listToken);
    return 0;
}