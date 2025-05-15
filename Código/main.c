// Inclusão dos arquivos de cabeçalho necessários
#include "lexico.h"
#include "sintatico.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Função principal do programa
 * Responsável por:
 * 1. Abrir o arquivo fonte de entrada
 * 2. Abrir o arquivo de saída para a análise sintática
 * 3. Executar a análise sintática
 * 4. Fechar os arquivos e exibir mensagem de conclusão
 */
int main()
{
    // Abre o arquivo fonte de entrada no modo leitura
    arquivo_fonte = fopen("input/codigo.pl0", "r");
    if (!arquivo_fonte)
    {
        printf("Erro ao abrir o arquivo fonte.\n");
        return 1;
    }

    // Abre o arquivo de saída para a análise sintática no modo escrita
    arquivo_saida_sintatico = fopen("output/saida_sintatico.txt", "w");
    if (!arquivo_saida_sintatico)
    {
        printf("Erro ao abrir o arquivo de saída sintática.\n");
        fclose(arquivo_fonte);
        return 1;
    }

    // Executa a análise sintática do código fonte
    analise_sintatica();

    // Fecha os arquivos abertos
    fclose(arquivo_fonte);
    fclose(arquivo_saida_sintatico);

    // Exibe mensagem de conclusão com o local do arquivo de saída
    printf("Análise sintática concluída! Resultados salvos em 'output/saida_sintatico.txt'.\n");

    return 0;
}