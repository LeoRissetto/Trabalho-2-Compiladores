#include "hash_table.h"
#include <stdio.h>
#include <ctype.h>

PalavraReservadaEntry *palavras_reservadas_hash[HASH_TABLE_SIZE] = {0};
SimboloReservadoEntry *simbolos_reservados_hash[HASH_TABLE_SIZE] = {0};

unsigned int hash_string(const char *str)
{
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + toupper((unsigned char)c);
    }
    return hash % HASH_TABLE_SIZE;
}

void inserir_palavra_reservada(const char *lexema, int tipo)
{
    unsigned int idx = hash_string(lexema);
    PalavraReservadaEntry *entry = malloc(sizeof(PalavraReservadaEntry));
    entry->lexema = strdup(lexema);
    entry->tipo = tipo;
    entry->next = palavras_reservadas_hash[idx];
    palavras_reservadas_hash[idx] = entry;
}

int buscar_palavra_reservada(const char *lexema, int *tipo)
{
    unsigned int idx = hash_string(lexema);
    PalavraReservadaEntry *entry = palavras_reservadas_hash[idx];
    while (entry)
    {
        if (strcasecmp(entry->lexema, lexema) == 0)
        {
            if (tipo)
                *tipo = entry->tipo;
            return 1;
        }
        entry = entry->next;
    }
    return 0;
}

void liberar_palavras_reservadas()
{
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        PalavraReservadaEntry *entry = palavras_reservadas_hash[i];
        while (entry)
        {
            PalavraReservadaEntry *tmp = entry;
            entry = entry->next;
            free(tmp->lexema);
            free(tmp);
        }
        palavras_reservadas_hash[i] = NULL;
    }
}

void inserir_simbolo_reservado(const char *lexema, const char *nomeSimbolo, int tipo)
{
    unsigned int idx = hash_string(lexema);
    SimboloReservadoEntry *entry = malloc(sizeof(SimboloReservadoEntry));
    entry->lexema = strdup(lexema);
    entry->nomeSimbolo = strdup(nomeSimbolo);
    entry->tipo = tipo;
    entry->next = simbolos_reservados_hash[idx];
    simbolos_reservados_hash[idx] = entry;
}

int buscar_simbolo_reservado(const char *lexema, int *tipo, const char **nomeSimbolo)
{
    unsigned int idx = hash_string(lexema);
    SimboloReservadoEntry *entry = simbolos_reservados_hash[idx];
    while (entry)
    {
        if (strcmp(entry->lexema, lexema) == 0)
        {
            if (tipo)
                *tipo = entry->tipo;
            if (nomeSimbolo)
                *nomeSimbolo = entry->nomeSimbolo;
            return 1;
        }
        entry = entry->next;
    }
    return 0;
}

void liberar_simbolos_reservados()
{
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        SimboloReservadoEntry *entry = simbolos_reservados_hash[i];
        while (entry)
        {
            SimboloReservadoEntry *tmp = entry;
            entry = entry->next;
            free(tmp->lexema);
            free(tmp->nomeSimbolo);
            free(tmp);
        }
        simbolos_reservados_hash[i] = NULL;
    }
}
