#ifndef LEXICO_H
#define LEXICO_H

// Inclusão das bibliotecas necessárias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Variáveis globais externas
extern int num_line;         // Contador de linhas do arquivo fonte
extern char caractere_atual; // Caractere atual sendo analisado
extern FILE *arquivo_fonte;  // Ponteiro para o arquivo fonte

/**
 * Enumeração que define todos os tipos possíveis de tokens
 * Inclui identificadores, números, palavras reservadas e símbolos
 */
typedef enum
{
    TOKEN_IDENTIFICADOR,            // Identificadores (nomes de variáveis, procedimentos)
    TOKEN_NUMERO,                   // Números
    TOKEN_CONST,                    // Palavra reservada 'const'
    TOKEN_VAR,                      // Palavra reservada 'var'
    TOKEN_PROCEDURE,                // Palavra reservada 'procedure'
    TOKEN_CALL,                     // Palavra reservada 'call'
    TOKEN_BEGIN,                    // Palavra reservada 'begin'
    TOKEN_END,                      // Palavra reservada 'end'
    TOKEN_IF,                       // Palavra reservada 'if'
    TOKEN_THEN,                     // Palavra reservada 'then'
    TOKEN_WHILE,                    // Palavra reservada 'while'
    TOKEN_DO,                       // Palavra reservada 'do'
    TOKEN_ODD,                      // Palavra reservada 'odd'
    TOKEN_SIMBOLO_VIRGULA,          // Símbolo ','
    TOKEN_SIMBOLO_PONTO_VIRGULA,    // Símbolo ';'
    TOKEN_SIMBOLO_PONTO,            // Símbolo '.'
    TOKEN_SIMBOLO_ABRE_PARENTESIS,  // Símbolo '('
    TOKEN_SIMBOLO_FECHA_PARENTESIS, // Símbolo ')'
    TOKEN_SIMBOLO_MAIS,             // Símbolo '+'
    TOKEN_SIMBOLO_MENOS,            // Símbolo '-'
    TOKEN_SIMBOLO_VEZES,            // Símbolo '*'
    TOKEN_SIMBOLO_DIVISAO,          // Símbolo '/'
    TOKEN_SIMBOLO_IGUAL,            // Símbolo '='
    TOKEN_SIMBOLO_MENOR,            // Símbolo '<'
    TOKEN_SIMBOLO_MAIOR,            // Símbolo '>'
    TOKEN_SIMBOLO_MENOR_IGUAL,      // Símbolo '<='
    TOKEN_SIMBOLO_MAIOR_IGUAL,      // Símbolo '>='
    TOKEN_SIMBOLO_DIFERENTE,        // Símbolo '<>'
    TOKEN_SIMBOLO_ATRIBUICAO,       // Símbolo ':='
    TOKEN_ERRO,                     // Token de erro
    TOKEN_EOF                       // Fim do arquivo
} TokenTipo;

/**
 * Estrutura que representa um token
 * Contém o tipo do token, seu lexema, valor e linha onde foi encontrado
 */
typedef struct
{
    TokenTipo tipo; // Tipo do token
    char *lexema;   // String que representa o token
    char *valor;    // Valor do token (para números e identificadores)
    int linha;      // Número da linha onde o token foi encontrado
} Token;

/**
 * Estrutura para palavras reservadas
 * Mapeia o lexema da palavra reservada para seu tipo de token
 */
typedef struct
{
    const char *lexema; // Palavra reservada
    TokenTipo tipo;     // Tipo do token correspondente
} PalavraReservada;

/**
 * Estrutura para símbolos reservados
 * Mapeia o símbolo para seu nome e tipo de token
 */
typedef struct
{
    const char *lexema;      // Símbolo
    const char *nomeSimbolo; // Nome do símbolo
    TokenTipo tipo;          // Tipo do token correspondente
} SimboloReservado;

/**
 * Estrutura para informações de símbolos
 * Contém o tipo e nome do símbolo
 */
typedef struct
{
    int tipo;         // Tipo do símbolo
    const char *nome; // Nome do símbolo
} SimboloInfo;

// Declaração das funções do analisador léxico
void ler_caractere();                                                 // Lê o próximo caractere do arquivo fonte
void retroceder();                                                    // Retrocede um caractere no arquivo fonte
char *converter_para_maiusculo(const char *str);                      // Converte uma string para maiúsculas
int obter_palavra_reservada(const char *identificador, char **valor); // Verifica se um identificador é uma palavra reservada
SimboloInfo obter_simbolo(char simbolo);                              // Obtém informações sobre um símbolo
Token obter_token();                                                  // Obtém o próximo token do arquivo fonte
void liberar_token(Token *token);                                     // Libera a memória alocada para um token

#endif