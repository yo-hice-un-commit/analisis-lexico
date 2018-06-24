//
// Created by miguelchauca on 24/06/18.
//

#ifndef TP_1_HELPERS_H
#define TP_1_HELPERS_H
typedef struct {
    char *lexema;
    char *token;
    void *sig;
} token;
typedef enum enumTokenType {
    UNDEFINED = 0,
    OPERATOR = 1,
    TYPE = 2,
    CARACTERPUNTUACION = 3,
    IDENTIFICADOR = 4
};
char *leerLinea(FILE *file, int *offset);
char * getWord(char *text, char limit);
void cleanWord(char *word);
void showResult(token *listToken);
void pushToken(token **List, token *item);
char *getToken(enum enumTokenType token);
char *trim(char *word);
#endif //TP_1_HELPERS_H
