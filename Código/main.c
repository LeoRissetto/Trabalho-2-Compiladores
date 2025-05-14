#include "lexico.h"
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

    // Open output file
    FILE *arquivo_saida = fopen("output/saida_tokens.txt", "w");
    if (!arquivo_saida)
    {
        printf("Erro ao abrir o arquivo de saída.\n");
        fclose(arquivo_fonte);
        return 1;
    }

    // Process tokens
    Token token;
    do
    {
        token = obter_token();
        fprintf(arquivo_saida, "%s, %s\n", token.lexema, token.valor);
        liberar_token(&token);
    } while (token.tipo != TOKEN_EOF);

    fclose(arquivo_fonte);
    fclose(arquivo_saida);

    printf("Análise léxica concluída! Tokens salvos em 'output/saida_tokens.txt'.\n");

    return 0;
}