#ifndef LEXICO_H
#define LEXICO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Global variables for the lexical analyzer
extern int num_line;
extern char caractere_atual;
extern FILE *arquivo_fonte;

// Token types
typedef enum
{
    TOKEN_IDENTIFICADOR,
    TOKEN_NUMERO,
    TOKEN_CONST,
    TOKEN_VAR,
    TOKEN_PROCEDURE,
    TOKEN_CALL,
    TOKEN_BEGIN,
    TOKEN_END,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_WHILE,
    TOKEN_DO,
    TOKEN_ODD,
    TOKEN_SIMBOLO_VIRGULA,
    TOKEN_SIMBOLO_PONTO_VIRGULA,
    TOKEN_SIMBOLO_PONTO,
    TOKEN_SIMBOLO_ABRE_PARENTESIS,
    TOKEN_SIMBOLO_FECHA_PARENTESIS,
    TOKEN_SIMBOLO_MAIS,
    TOKEN_SIMBOLO_MENOS,
    TOKEN_SIMBOLO_VEZES,
    TOKEN_SIMBOLO_DIVISAO,
    TOKEN_SIMBOLO_IGUAL,
    TOKEN_SIMBOLO_MENOR,
    TOKEN_SIMBOLO_MAIOR,
    TOKEN_SIMBOLO_MENOR_IGUAL,
    TOKEN_SIMBOLO_MAIOR_IGUAL,
    TOKEN_SIMBOLO_DIFERENTE,
    TOKEN_SIMBOLO_ATRIBUICAO,
    TOKEN_ERRO,
    TOKEN_EOF
} TokenTipo;

// Token structure
typedef struct
{
    TokenTipo tipo;
    char *lexema;
    char *valor;
    int linha;
} Token;

// Struct para palavras reservadas
typedef struct
{
    const char *lexema;
    TokenTipo tipo;
} PalavraReservada;

// Struct para símbolos reservados
typedef struct
{
    const char *lexema;
    const char *nomeSimbolo;
    TokenTipo tipo;
} SimboloReservado;

// Struct para informações do símbolo
typedef struct
{
    int tipo;
    const char *nome;
} SimboloInfo;

// Function declarations
void ler_caractere();
void retroceder();
char *converter_para_maiusculo(const char *str);
int obter_palavra_reservada(const char *identificador, char **valor);
SimboloInfo obter_simbolo(char simbolo);
Token obter_token();
void liberar_token(Token *token);

#endif // LEXICO_H