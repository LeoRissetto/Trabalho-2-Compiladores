#ifndef SINTATICO_H
#define SINTATICO_H

#include "lexico.h"
#include <stdio.h>

// Variáveis globais externas
extern Token lookahead;               // Token atual analisado
extern FILE *arquivo_saida_sintatico; // Arquivo de saída da análise sintática

// Retorna a descrição textual de um tipo de token
const char *obter_descricao_token(TokenTipo tipo);

// Funções de controle do analisador sintático
void advance(void);
void erro(const char *msg);
void sincroniza(TokenTipo sincronizadores[], int tamanho);

// Funções que implementam a gramática da linguagem
void programa(void);
void bloco(void);
void declaracao(void);
void constante(void);
void mais_const(void);
void variavel(void);
void mais_var(void);
void procedimento(void);
void comando(void);
void mais_cmd(void);
void expressao(void);
void operador_unario(void);
void termo(void);
void mais_termos(void);
void fator(void);
void mais_fatores(void);
void condicao(void);
void relacional(void);

// Função principal do analisador sintático
void analise_sintatica(void);

#endif