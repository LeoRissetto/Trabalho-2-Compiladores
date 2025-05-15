#include "sintatico.h"

// Global variable definitions
Token lookahead;
FILE *arquivo_saida_sintatico;

// Helper function to get token type description
const char *obter_descricao_token(TokenTipo tipo)
{
    switch (tipo)
    {
    case TOKEN_IDENTIFICADOR:
        return "identificador";
    case TOKEN_NUMERO:
        return "número";
    case TOKEN_CONST:
        return "CONST";
    case TOKEN_VAR:
        return "VAR";
    case TOKEN_PROCEDURE:
        return "PROCEDURE";
    case TOKEN_CALL:
        return "CALL";
    case TOKEN_BEGIN:
        return "BEGIN";
    case TOKEN_END:
        return "END";
    case TOKEN_IF:
        return "IF";
    case TOKEN_THEN:
        return "THEN";
    case TOKEN_WHILE:
        return "WHILE";
    case TOKEN_DO:
        return "DO";
    case TOKEN_ODD:
        return "ODD";
    case TOKEN_SIMBOLO_VIRGULA:
        return "vírgula";
    case TOKEN_SIMBOLO_PONTO_VIRGULA:
        return "ponto e vírgula";
    case TOKEN_SIMBOLO_PONTO:
        return "ponto";
    case TOKEN_SIMBOLO_ABRE_PARENTESIS:
        return "parêntese de abertura";
    case TOKEN_SIMBOLO_FECHA_PARENTESIS:
        return "parêntese de fechamento";
    case TOKEN_SIMBOLO_MAIS:
        return "sinal de mais";
    case TOKEN_SIMBOLO_MENOS:
        return "sinal de menos";
    case TOKEN_SIMBOLO_VEZES:
        return "sinal de multiplicação";
    case TOKEN_SIMBOLO_DIVISAO:
        return "sinal de divisão";
    case TOKEN_SIMBOLO_IGUAL:
        return "sinal de igual";
    case TOKEN_SIMBOLO_MENOR:
        return "sinal de menor";
    case TOKEN_SIMBOLO_MAIOR:
        return "sinal de maior";
    case TOKEN_SIMBOLO_MENOR_IGUAL:
        return "sinal de menor ou igual";
    case TOKEN_SIMBOLO_MAIOR_IGUAL:
        return "sinal de maior ou igual";
    case TOKEN_SIMBOLO_DIFERENTE:
        return "sinal de diferente";
    case TOKEN_SIMBOLO_ATRIBUICAO:
        return "sinal de atribuição";
    case TOKEN_EOF:
        return "fim de arquivo";
    default:
        return "token desconhecido";
    }
}

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
        char msg[100];
        snprintf(msg, sizeof(msg), "Token inesperado. Esperado: %s, Encontrado: %s",
                 obter_descricao_token(esperado),
                 obter_descricao_token(lookahead.tipo));
        erro(msg);
        return 0;
    }
}

void sincroniza(TokenTipo sincronizadores[], int tamanho)
{
    int i;
    int encontrou = 0;

    while (!encontrou && lookahead.tipo != TOKEN_EOF)
    {
        for (i = 0; i < tamanho; i++)
        {
            if (lookahead.tipo == sincronizadores[i])
            {
                encontrou = 1;
                break;
            }
        }
        if (!encontrou)
        {
            advance();
        }
    }
}

void programa()
{
    TokenTipo sync[] = {TOKEN_EOF};
    bloco();
    if (!match(TOKEN_SIMBOLO_PONTO))
    {
        erro("Esperado '.' no final do programa");
        sincroniza(sync, 1);
    }
    if (lookahead.tipo != TOKEN_EOF)
    {
        erro("Token extra após fim do programa");
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
    TokenTipo sync[] = {TOKEN_SIMBOLO_PONTO_VIRGULA, TOKEN_END, TOKEN_EOF};

    switch (lookahead.tipo)
    {
    case TOKEN_IDENTIFICADOR:
        advance();
        if (!match(TOKEN_SIMBOLO_ATRIBUICAO))
        {
            erro("Esperado ':=' após identificador");
            sincroniza(sync, 3);
            return;
        }
        expressao();
        break;
    case TOKEN_CALL:
        advance();
        if (!match(TOKEN_IDENTIFICADOR))
        {
            erro("Esperado identificador após CALL");
            sincroniza(sync, 3);
            return;
        }
        break;
    case TOKEN_BEGIN:
        advance();
        comando();
        mais_cmd();
        if (!match(TOKEN_END))
        {
            erro("Esperado END");
            sincroniza(sync, 3);
            return;
        }
        break;
    case TOKEN_IF:
        advance();
        condicao();
        if (!match(TOKEN_THEN))
        {
            erro("Esperado THEN");
            sincroniza(sync, 3);
            return;
        }
        comando();
        break;
    case TOKEN_WHILE:
        advance();
        condicao();
        if (!match(TOKEN_DO))
        {
            erro("Esperado DO");
            sincroniza(sync, 3);
            return;
        }
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