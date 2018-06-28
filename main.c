#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"

enum enumTokenType isWordReserved(char *word) {
    char *listOperador[] = {"if", "while", "return", "else", "for", "switch", "case"};
    char *listType[] = {"int", "char", "float", "double"};
    int i;
    for (i = 0; i < 7; ++i) {
        if (strcmp(listOperador[i], word) == 0) {
            return OPERATOR;
        }
    }
    for (i = 0; i < 4; ++i) {
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
                type = LITERALCADENA;
            } else {
                type = IDENTIFICADOR;
            }
        }
    }
    free(cleanSpace);
    return type;
}

int nuevoToken(enum enumTokenType type, char *word, token **List, int linea) {
    token *newToken = malloc(sizeof(token));
    newToken->token = getToken(type);
    char *lexema = malloc(strlen(word));
    strcpy(lexema, word);
    newToken->lexema = lexema;
    newToken->linea = linea;
    newToken->sig = NULL;
    newToken->Error = "";
    pushToken(&(*List), newToken);
}

void analisisLexico(token *List) {
    if (!List)
        return;
    token *actual;
    token *post;
    actual = List;
    post = List->sig;
    while (actual) {
        if (strcmp("IDENTIFICADOR", actual->token) == 0) {
            //verificamos el siguiente para ver si es un nombre de variable o una funcion
            if (post) {
                if (strcmp("(", post->lexema) == 0) {
                    actual->token = "IDENTIFICADOR DE FUNCION";
                }
            }
        }
        actual = post;
        if (post) {
            post = post->sig;
        }
    }
}

void crearFile(char *nameFile, token *List) {
    FILE * fileResult = fopen(nameFile,"w");
    if(!fileResult){
        printf("No se pudo Crear el archivo\n");
        return;
    }
    token * Lexema = List;
    fprintf(fileResult,"--------------TABLA DE LEXEMAS-------------------\n");
    while (Lexema){
        fprintf(fileResult,"Lexema [%s]\n", Lexema->lexema);
        fprintf(fileResult,"Token [%s]\n", Lexema->token);
        fprintf(fileResult,"Linea [%d]\n", Lexema->linea);
        fprintf(fileResult,"----------------\n");
        Lexema = Lexema->sig;
    }
    fclose(fileResult);
}

int main(int argc, char *argv[]) {
    int flagResultadoinFile = 0;
    if (argc < 2) {
        printf("Se require File \n");
        return 1;
    }
    if (argc > 3) {
        printf("Se esta mandando mas parametros de lo requerido\n");
        return 1;
    }
    if (argc == 3) {
        flagResultadoinFile = 1;
    }

    char letter;
    char word[1000];
    int wordPosition = 0;
    int linea = 1;
    cleanWord(word);
    token *listToken = NULL;
    FILE *program = fopen(argv[1], "r");
    if (!program) {
        printf("No se puedo encontrar el archivo\n");
        return 1;
    }
    enum enumTokenType tipo;

    while (!feof(program)) {
        letter = (char) fgetc(program);
        switch (letter) {
            case -1: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                cleanWord(word);
                wordPosition = 0;
                break;
            }
            case 10: {
                linea++;
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
            case 32:
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                cleanWord(word);
                wordPosition = 0;
                continue;
            case 33:
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                cleanWord(word);
                wordPosition = 0;
                letter = (char) getc(program);
                if (letter == 61) {
                    nuevoToken(OPERATOR, "!=", &listToken, linea);
                    continue;
                } else {
                    nuevoToken(OPERATOR, "!", &listToken, linea);
                }
                if (letter == 32) continue;
                break;
            case 38: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                cleanWord(word);
                wordPosition = 0;
                letter = (char) getc(program);
                if (letter == 38) {
                    nuevoToken(OPERATOR, "&&", &listToken, linea);
                    continue;
                } else {
                    nuevoToken(UNDEFINED, "&", &listToken, linea);
                }
                if (letter == 32) continue;
                break;
            }
            case 124: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                cleanWord(word);
                wordPosition = 0;
                letter = (char) getc(program);
                if (letter == 124) {
                    nuevoToken(OPERATOR, "||", &listToken, linea);
                    continue;
                } else {
                    nuevoToken(UNDEFINED, "|", &listToken, linea);
                }
                if (letter == 32) continue;
                break;
            }
            case 40:
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                nuevoToken(CARACTERPUNTUACION, "(", &listToken, linea);
                cleanWord(word);
                wordPosition = 0;
                continue;
            case 43:
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                cleanWord(word);
                wordPosition = 0;
                letter = (char) getc(program);
                if (letter == 43) {
                    nuevoToken(OPERATOR, "++", &listToken, linea);
                    continue;
                } else {
                    nuevoToken(OPERATOR, "+", &listToken, linea);
                }
                if (letter == 32) continue;
                break;
            case 44: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                nuevoToken(CARACTERPUNTUACION, ",", &listToken, linea);
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
            case 45:
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                cleanWord(word);
                wordPosition = 0;
                letter = (char) getc(program);
                if (letter == 45) {
                    nuevoToken(OPERATOR, "--", &listToken, linea);
                    continue;
                } else {
                    nuevoToken(OPERATOR, "-", &listToken, linea);
                }
                if (letter == 32) continue;
                break;
            case 41: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                // si es una carga o una asignaciom

                nuevoToken(CARACTERPUNTUACION, ")", &listToken, linea);
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
            case 123: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                nuevoToken(CARACTERPUNTUACION, "{", &listToken, linea);
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
            case 125: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                nuevoToken(CARACTERPUNTUACION, "}", &listToken, linea);
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
            case 60: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                cleanWord(word);
                wordPosition = 0;
                letter = (char) getc(program);
                if (letter == 61) {
                    nuevoToken(OPERATOR, "<=", &listToken, linea);
                    continue;
                } else {
                    nuevoToken(OPERATOR, "<", &listToken, linea);
                }
                if (letter == 32) continue;
                break;
            }
            case 61: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                cleanWord(word);
                wordPosition = 0;
                letter = (char) getc(program);
                if (letter == 61) {
                    nuevoToken(OPERATOR, "==", &listToken, linea);
                    continue;
                } else {
                    nuevoToken(OPERADORCARGA, "=", &listToken, linea);
                }
                if (letter == 32) continue;
                break;
            }
            case 62: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                cleanWord(word);
                wordPosition = 0;
                letter = (char) getc(program);
                if (letter == 61) {
                    nuevoToken(OPERATOR, ">=", &listToken, linea);
                    continue;
                } else {
                    nuevoToken(OPERATOR, ">", &listToken, linea);
                }
                if (letter == 32) continue;
                break;
            }
            case 59: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                nuevoToken(CARACTERPUNTUACION, ";", &listToken, linea);
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
            case 91: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                nuevoToken(CARACTERPUNTUACION, "[", &listToken, linea);
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
            case 93: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                nuevoToken(CARACTERPUNTUACION, "]", &listToken, linea);
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
            case 42: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                nuevoToken(OPERATOR, "*", &listToken, linea);
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
            case 47: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                nuevoToken(OPERATOR, "/", &listToken, linea);
                cleanWord(word);
                wordPosition = 0;
                continue;
            }
            case 37: {
                tipo = typeWord(word);
                if (tipo) {
                    nuevoToken(tipo, word, &listToken, linea);
                }
                nuevoToken(OPERATOR, "%", &listToken, linea);
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
    analisisLexico(listToken);
    if (!flagResultadoinFile) {
        showResult(listToken);
    } else {
        crearFile(argv[2],listToken);
    }
    fclose(program);
    return 0;
}