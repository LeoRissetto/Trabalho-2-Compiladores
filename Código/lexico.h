#ifndef LEXICO_H
#define LEXICO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Variáveis globais externas
extern int num_line;         // Linha atual do arquivo fonte
extern char caractere_atual; // Caractere atual sendo analisado
extern FILE *arquivo_fonte;  // Ponteiro para o arquivo fonte

// Tipos de tokens reconhecidos pelo analisador léxico
typedef enum
{
    TOKEN_IDENTIFICADOR,            // Identificador
    TOKEN_NUMERO,                   // Número
    TOKEN_CONST,                    // 'const'
    TOKEN_VAR,                      // 'var'
    TOKEN_PROCEDURE,                // 'procedure'
    TOKEN_CALL,                     // 'call'
    TOKEN_BEGIN,                    // 'begin'
    TOKEN_END,                      // 'end'
    TOKEN_IF,                       // 'if'
    TOKEN_THEN,                     // 'then'
    TOKEN_WHILE,                    // 'while'
    TOKEN_DO,                       // 'do'
    TOKEN_ODD,                      // 'odd'
    TOKEN_SIMBOLO_VIRGULA,          // ','
    TOKEN_SIMBOLO_PONTO_VIRGULA,    // ';'
    TOKEN_SIMBOLO_PONTO,            // '.'
    TOKEN_SIMBOLO_ABRE_PARENTESIS,  // '('
    TOKEN_SIMBOLO_FECHA_PARENTESIS, // ')'
    TOKEN_SIMBOLO_MAIS,             // '+'
    TOKEN_SIMBOLO_MENOS,            // '-'
    TOKEN_SIMBOLO_VEZES,            // '*'
    TOKEN_SIMBOLO_DIVISAO,          // '/'
    TOKEN_SIMBOLO_IGUAL,            // '='
    TOKEN_SIMBOLO_MENOR,            // '<'
    TOKEN_SIMBOLO_MAIOR,            // '>'
    TOKEN_SIMBOLO_MENOR_IGUAL,      // '<='
    TOKEN_SIMBOLO_MAIOR_IGUAL,      // '>='
    TOKEN_SIMBOLO_DIFERENTE,        // '<>'
    TOKEN_SIMBOLO_ATRIBUICAO,       // ':='
    TOKEN_ERRO,                     // Erro léxico
    TOKEN_EOF                       // Fim do arquivo
} TokenTipo;

// Estrutura de um token reconhecido
typedef struct
{
    TokenTipo tipo;
    char *lexema;
    char *valor;
    int linha;
} Token;

// Palavra reservada e seu tipo
typedef struct
{
    const char *lexema;
    TokenTipo tipo;
} PalavraReservada;

// Símbolo reservado e seu nome
typedef struct
{
    const char *lexema;
    const char *nomeSimbolo;
    TokenTipo tipo;
} SimboloReservado;

// Informações de símbolo
typedef struct
{
    int tipo;
    const char *nome;
} SimboloInfo;

// Funções do analisador léxico
void ler_caractere(void);
void retroceder(void);
char *converter_para_maiusculo(const char *str);
int obter_palavra_reservada(const char *identificador, char **valor);
SimboloInfo obter_simbolo(char simbolo);
Token obter_token(void);
void liberar_token(Token *token);

#endif