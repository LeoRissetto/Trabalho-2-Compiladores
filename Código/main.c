#include "lexico.h"
#include "sintatico.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Open source file
    arquivo_fonte = fopen("input/codigo.pl0", "r");
    if (!arquivo_fonte)
    {
        printf("Erro ao abrir o arquivo fonte.\n");
        return 1;
    }

    // Open syntax analysis output file
    arquivo_saida_sintatico = fopen("output/saida_sintatico.txt", "w");
    if (!arquivo_saida_sintatico)
    {
        printf("Erro ao abrir o arquivo de saída sintática.\n");
        fclose(arquivo_fonte);
        return 1;
    }

    // Perform syntax analysis
    analise_sintatica();

    // Close all files
    fclose(arquivo_fonte);
    fclose(arquivo_saida_sintatico);

    printf("Análise sintática concluída! Resultados salvos em 'output/saida_sintatico.txt'.\n");

    return 0;
}