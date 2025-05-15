#ifndef SINTATICO_H
#define SINTATICO_H

#include "lexico.h"
#include <stdio.h>

// Global variables
extern Token lookahead;
extern FILE *arquivo_saida_sintatico;

// Helper function declarations
const char *obter_descricao_token(TokenTipo tipo);

void advance();
void erro(const char *msg);
int match(TokenTipo esperado);
void sincroniza(TokenTipo sincronizadores[], int tamanho);

void programa();
void bloco();
void declaracao();
void constante();
void mais_const();
void variavel();
void mais_var();
void procedimento();
void comando();
void mais_cmd();
void expressao();
void operador_unario();
void termo();
void mais_termos();
void fator();
void mais_fatores();
void condicao();
void relacional();

void analise_sintatica();

#endif // SINTATICO_H