#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include <string.h>

// Estrutura para entrada de palavra reservada
typedef struct PalavraReservadaEntry
{
    char *lexema;
    int tipo;
    struct PalavraReservadaEntry *next;
} PalavraReservadaEntry;

// Estrutura para entrada de símbolo reservado
typedef struct SimboloReservadoEntry
{
    char *lexema;
    char *nomeSimbolo;
    int tipo;
    struct SimboloReservadoEntry *next;
} SimboloReservadoEntry;

#define HASH_TABLE_SIZE 32

// Tabelas hash
extern PalavraReservadaEntry *palavras_reservadas_hash[HASH_TABLE_SIZE];
extern SimboloReservadoEntry *simbolos_reservados_hash[HASH_TABLE_SIZE];

// Funções para manipulação das tabelas hash
unsigned int hash_string(const char *str);
void inserir_palavra_reservada(const char *lexema, int tipo);
int buscar_palavra_reservada(const char *lexema, int *tipo);
void liberar_palavras_reservadas();

void inserir_simbolo_reservado(const char *lexema, const char *nomeSimbolo, int tipo);
int buscar_simbolo_reservado(const char *lexema, int *tipo, const char **nomeSimbolo);
void liberar_simbolos_reservados();

#endif
