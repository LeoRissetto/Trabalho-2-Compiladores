// main.c - Ponto de entrada do analisador
#include "lexico.h"
#include "sintatico.h"
#include <stdio.h>
#include <stdlib.h>

// Função para abrir um arquivo e tratar erro
static FILE *abrir_arquivo(const char *caminho, const char *modo, const char *erro_msg)
{
    FILE *arquivo = fopen(caminho, modo);
    if (!arquivo)
    {
        fprintf(stderr, "%s\n", erro_msg);
    }
    return arquivo;
}

int main(void)
{
    arquivo_fonte = abrir_arquivo("input/codigo.pl0", "r", "Erro ao abrir o arquivo fonte.");
    if (!arquivo_fonte)
        return EXIT_FAILURE;

    arquivo_saida_sintatico = abrir_arquivo("output/saida_sintatico.txt", "w", "Erro ao abrir o arquivo de saída sintática.");
    if (!arquivo_saida_sintatico)
    {
        fclose(arquivo_fonte);
        return EXIT_FAILURE;
    }

    analise_sintatica();

    fclose(arquivo_fonte);
    fclose(arquivo_saida_sintatico);

    printf("Análise sintática concluída! Resultados salvos em 'output/saida_sintatico.txt'.\n");
    return EXIT_SUCCESS;
}