//
// Created by miguelchauca on 24/06/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"

char *leerLinea(FILE *file, int *offset) {
    int flagLine = 1;
    char letter = 0;
    int length = 0;
    while (flagLine) {
        letter = (char) fgetc(file);
        if (letter == 10 | letter == -1) {
            flagLine = 0;
        }
        length++;
    }
    fseek(file, *offset, SEEK_SET);
    char *line = malloc((size_t) length);
    fgets(line, length + 1, file);
    if (feof(file) && strlen(line) == 0) {
        return NULL;
    }
    *offset = *offset + length;
    return line;

}

char *getWord(char *text, char limit) {
    int length = (int) strlen(text);
    if (!length) return NULL;
    int i = 0;
    int firstSpace = 0;
    int lengthSub = 0;
    char *copyText = malloc((size_t) length);
    strcpy(copyText, text);
    for (i; i < length; ++i) {
        if (firstSpace) {
            if (text[i] != limit) {
                lengthSub++;
            } else {
                break;
            }
        } else {
            if (text[i] != ' ') {
                firstSpace = 1;
                i--;
            }
        }
    }
    int j = 0;
    //limpiamos el texto
    for (j; j < length; ++j) {
        text[j] = 0;
    }
    //copiamos lo extraido
    j = i - lengthSub;
    char *subStr = malloc((size_t) lengthSub);
    int h = 0;
    for (j; j < i; j++) {
        subStr[h] = copyText[j];
        h++;
    }
    //copiamos el resto al texto
    j = 0;
    for (i; i < length; ++i) {
        text[j] = copyText[i];
        j++;
    }
    free(copyText);
    return subStr;
}

void showResult(token *listToken) {
    token *aux = listToken;
    printf("--------------TABLA DE LEXEMAS-------------------\n");
    while (aux) {
        printf("Lexema [%s]\n", aux->lexema);
        printf("Token [%s]\n", aux->token);
        printf("Linea [%d]\n", aux->linea);
        printf("----------------\n");
        aux = aux->sig;
    }
}

void cleanWord(char *word) {
    memset(word, 0, 1000);
}

void pushToken(token **List, token *item) {
    if (*List) {
        token *tokenSig = (*List)->sig;
        token *preToken = (*List)->sig;
        while (tokenSig) {
            tokenSig = tokenSig->sig;
            if (tokenSig) {
                preToken = tokenSig;
            }
        }
        if (preToken) {
            preToken->sig = item;
        } else {
            (*List)->sig = item;
        }
    } else {
        *List = item;
    }
}

char *getToken(enum enumTokenType token) {
    switch (token) {
        case OPERATOR: {
            return "OPERADOR";
        }
        case TYPE: {
            return "DEFINICION TYPE";
        }
        case CARACTERPUNTUACION: {
            return "CARACTER PUNTUACION";
        }
        case IDENTIFICADOR:
            return "IDENTIFICADOR";
        case OPERADORCARGA:
            return "OPERADOR CARGA";
        case VALORCONSTANTE:
            return "VALOR CONSTANTE";
        case LITERALCADENA:
            return "LITERAL CADENA";
        default:
            return "LEXEMA NO RECONOCIDO";
    }
}

char *trim(char *word) {
    int length = (int) strlen(word);
    int i, j = 0;
    int lengthCopy = 0;
    for (i = 0; i < length; ++i) {
        if (word[i] != 32) {
            lengthCopy++;
        }
    }
    char *textTrim = malloc((size_t) lengthCopy);
    for (i = 0; i < length; ++i) {
        if (word[i] != 32) {
            textTrim[j] = word[i];
            j++;
        }
    }
    return textTrim;
}

int isNumber(char *text) {
    char *letra = NULL;
    strtol(text, &letra, 10);
    if (strlen(letra)) {
        return 0;
    } else {
        return 1;
    }
}

int isString(char *text) {
    int length = (int) strlen(text);
    if(text[0] == '"' && text[length-1] == '"'){
        return 1;
    }
    return 0;
}