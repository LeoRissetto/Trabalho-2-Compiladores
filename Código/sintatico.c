#include "sintatico.h"

// Global variable definitions
Token lookahead;
FILE *arquivo_saida_sintatico;

void advance()
{
    lookahead = obter_token();
}

void erro(const char *msg)
{
    fprintf(arquivo_saida_sintatico, "Erro sintático na linha %d: %s. Token atual: %s\n", lookahead.linha, msg, lookahead.lexema);
}

int match(TokenTipo esperado)
{
    if (lookahead.tipo == esperado)
    {
        advance();
        return 1;
    }
    else
    {
        erro("Token inesperado");
        return 0;
    }
}

void sincroniza(TokenTipo sincronizadores[], int tamanho)
{
    int i;
    while (lookahead.tipo != TOKEN_EOF)
    {
        for (i = 0; i < tamanho; i++)
        {
            if (lookahead.tipo == sincronizadores[i])
            {
                return;
            }
        }
        advance();
    }
}

void programa()
{
    TokenTipo sync[] = {TOKEN_EOF};
    bloco();
    if (!match(TOKEN_EOF))
    {
        erro("Esperado '.' no final do programa");
        sincroniza(sync, 1);
    }
}

void bloco()
{
    declaracao();
    comando();
}

void declaracao()
{
    constante();
    variavel();
    procedimento();
}

void constante()
{
    if (lookahead.tipo == TOKEN_CONST)
    {
        advance();
        if (match(TOKEN_IDENTIFICADOR))
        {
            if (match(TOKEN_SIMBOLO_IGUAL))
            {
                if (match(TOKEN_NUMERO))
                {
                    mais_const();
                    match(TOKEN_SIMBOLO_PONTO_VIRGULA);
                }
            }
        }
    }
}

void mais_const()
{
    if (lookahead.tipo == TOKEN_SIMBOLO_VIRGULA)
    {
        advance();
        if (match(TOKEN_IDENTIFICADOR))
        {
            if (match(TOKEN_SIMBOLO_IGUAL))
            {
                if (match(TOKEN_NUMERO))
                {
                    mais_const();
                }
            }
        }
    }
}

void variavel()
{
    if (lookahead.tipo == TOKEN_VAR)
    {
        advance();
        if (match(TOKEN_IDENTIFICADOR))
        {
            mais_var();
            match(TOKEN_SIMBOLO_PONTO_VIRGULA);
        }
    }
}

void mais_var()
{
    if (lookahead.tipo == TOKEN_SIMBOLO_VIRGULA)
    {
        advance();
        if (match(TOKEN_IDENTIFICADOR))
        {
            mais_var();
        }
    }
}

void procedimento()
{
    while (lookahead.tipo == TOKEN_PROCEDURE)
    {
        advance();
        if (match(TOKEN_IDENTIFICADOR))
        {
            if (match(TOKEN_SIMBOLO_PONTO_VIRGULA))
            {
                bloco();
                match(TOKEN_SIMBOLO_PONTO_VIRGULA);
            }
        }
    }
}

void comando()
{
    switch (lookahead.tipo)
    {
    case TOKEN_IDENTIFICADOR:
        advance();
        match(TOKEN_SIMBOLO_ATRIBUICAO);
        expressao();
        break;
    case TOKEN_CALL:
        advance();
        match(TOKEN_IDENTIFICADOR);
        break;
    case TOKEN_BEGIN:
        advance();
        comando();
        mais_cmd();
        match(TOKEN_END);
        break;
    case TOKEN_IF:
        advance();
        condicao();
        match(TOKEN_THEN);
        comando();
        break;
    case TOKEN_WHILE:
        advance();
        condicao();
        match(TOKEN_DO);
        comando();
        break;
    default:
        // lambda
        break;
    }
}

void mais_cmd()
{
    if (lookahead.tipo == TOKEN_SIMBOLO_PONTO_VIRGULA)
    {
        advance();
        comando();
        mais_cmd();
    }
}

void expressao()
{
    operador_unario();
    termo();
    mais_termos();
}

void operador_unario()
{
    if (lookahead.tipo == TOKEN_SIMBOLO_MAIS || lookahead.tipo == TOKEN_SIMBOLO_MENOS)
    {
        advance();
    }
}

void termo()
{
    fator();
    mais_fatores();
}

void mais_termos()
{
    while (lookahead.tipo == TOKEN_SIMBOLO_MAIS || lookahead.tipo == TOKEN_SIMBOLO_MENOS)
    {
        advance();
        termo();
    }
}

void fator()
{
    switch (lookahead.tipo)
    {
    case TOKEN_IDENTIFICADOR:
    case TOKEN_NUMERO:
        advance();
        break;
    case TOKEN_SIMBOLO_ABRE_PARENTESIS:
        advance();
        expressao();
        match(TOKEN_SIMBOLO_FECHA_PARENTESIS);
        break;
    default:
        erro("Esperado fator (ident, num ou '(')");
        break;
    }
}

void mais_fatores()
{
    while (lookahead.tipo == TOKEN_SIMBOLO_VEZES || lookahead.tipo == TOKEN_SIMBOLO_DIVISAO)
    {
        advance();
        fator();
    }
}

void condicao()
{
    if (lookahead.tipo == TOKEN_ODD)
    {
        advance();
        expressao();
    }
    else
    {
        expressao();
        relacional();
        expressao();
    }
}

void relacional()
{
    switch (lookahead.tipo)
    {
    case TOKEN_SIMBOLO_IGUAL:
    case TOKEN_SIMBOLO_DIFERENTE:
    case TOKEN_SIMBOLO_MENOR:
    case TOKEN_SIMBOLO_MENOR_IGUAL:
    case TOKEN_SIMBOLO_MAIOR:
    case TOKEN_SIMBOLO_MAIOR_IGUAL:
        advance();
        break;
    default:
        erro("Esperado operador relacional");
        break;
    }
}

void analise_sintatica()
{
    advance(); // Inicializa lookahead
    programa();
    if (lookahead.tipo != TOKEN_EOF)
    {
        erro("Token extra após fim do programa");
    }
}