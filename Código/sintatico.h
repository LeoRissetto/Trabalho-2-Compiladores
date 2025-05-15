#ifndef SINTATICO_H
#define SINTATICO_H

// Inclusão dos arquivos de cabeçalho necessários
#include "lexico.h"
#include <stdio.h>

// Variáveis globais externas
extern Token lookahead;               // Token atual sendo analisado
extern FILE *arquivo_saida_sintatico; // Arquivo de saída para a análise sintática

// Função auxiliar para obter a descrição de um tipo de token
const char *obter_descricao_token(TokenTipo tipo);

// Funções de controle do analisador sintático
void advance();                                            // Avança para o próximo token
void erro(const char *msg);                                // Trata erros sintáticos
int match(TokenTipo esperado);                             // Verifica se o token atual corresponde ao esperado
void sincroniza(TokenTipo sincronizadores[], int tamanho); // Sincroniza após um erro

// Funções que implementam a gramática da linguagem
void programa();        // Analisa um programa completo
void bloco();           // Analisa um bloco de código
void declaracao();      // Analisa declarações (const, var, procedure)
void constante();       // Analisa declaração de constante
void mais_const();      // Analisa mais declarações de constantes
void variavel();        // Analisa declaração de variável
void mais_var();        // Analisa mais declarações de variáveis
void procedimento();    // Analisa declaração de procedimento
void comando();         // Analisa um comando
void mais_cmd();        // Analisa mais comandos
void expressao();       // Analisa uma expressão
void operador_unario(); // Analisa operador unário
void termo();           // Analisa um termo
void mais_termos();     // Analisa mais termos
void fator();           // Analisa um fator
void mais_fatores();    // Analisa mais fatores
void condicao();        // Analisa uma condição
void relacional();      // Analisa operadores relacionais

// Função principal do analisador sintático
void analise_sintatica();

#endif