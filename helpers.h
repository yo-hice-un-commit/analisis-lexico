//
// Created by miguelchauca on 24/06/18.
//

#ifndef TP_1_HELPERS_H
#define TP_1_HELPERS_H
typedef struct {
    char *lexema;
    char *token;
    int linea;
    void *sig;
    char *Error;
} token;
typedef enum enumTokenType {
    UNDEFINED = 0,
    OPERATOR = 1,
    TYPE = 2,
    CARACTERPUNTUACION = 3,
    IDENTIFICADOR = 4,
    OPERADORCARGA = 5,
    VALORCONSTANTE = 6,
    LITERALCADENA = 7
} enumTokenType;


void cleanWord(char *word);

void showResult(token *listToken);

void pushToken(token **List, token *item);

char *getToken(enum enumTokenType token);

char *trim(char *word);

int isNumber(char *text);

int isString(char *text);

#endif //TP_1_HELPERS_H
