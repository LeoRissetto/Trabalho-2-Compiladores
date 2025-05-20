#include "sintatico.h"

// Variáveis globais
Token lookahead;               // Token atual sendo analisado
FILE *arquivo_saida_sintatico; // Arquivo de saída para a análise sintática

// Obtém a descrição em português de um tipo de token
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

// Avança para o próximo token
void advance()
{
    lookahead = obter_token();
    while (lookahead.tipo == TOKEN_ERRO)
    {
        lookahead = obter_token();
        fprintf(arquivo_saida_sintatico, "Erro léxico na linha %d: %s\n", lookahead.linha, lookahead.lexema);
    }
}

// Registra um erro sintático
void erro(const char *msg)
{
    fprintf(arquivo_saida_sintatico, "Erro sintático na linha %d: %s. Token atual: %s\n", lookahead.linha, msg, lookahead.lexema);
}

// Verifica se o token atual corresponde ao esperado
int match(TokenTipo esperado)
{
    if (lookahead.tipo == esperado)
    {
        advance();
        return 1;
    }
    else
    {
        return 0;
    }
}

// Sincroniza o analisador após um erro
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

// Analisa um programa completo
void programa()
{
    bloco();
}

// Analisa um bloco de código
void bloco()
{
    declaracao();
    comando();
}

// Analisa declarações
void declaracao()
{
    constante();
    variavel();
    procedimento();
}

// Analisa declaração de constantes
void constante()
{
    // Conjunto de sincronização para constante
    TokenTipo sincronizadores[] = {
        TOKEN_VAR,
        TOKEN_PROCEDURE,
        TOKEN_IDENTIFICADOR,
        TOKEN_CALL,
        TOKEN_BEGIN,
        TOKEN_IF,
        TOKEN_WHILE,
        TOKEN_SIMBOLO_PONTO,
        TOKEN_EOF};

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
                    if (!match(TOKEN_SIMBOLO_PONTO_VIRGULA))
                    {
                        erro("Esperado ';' após declaração de constante");
                        sincroniza(sincronizadores, 9);
                    }
                }
                else
                {
                    erro("Esperado número após '=' na constante");
                    sincroniza(sincronizadores, 9);
                }
            }
            else
            {
                erro("Esperado '=' após identificador da constante");
                sincroniza(sincronizadores, 9);
            }
        }
        else
        {
            erro("Esperado identificador após 'CONST'");
            sincroniza(sincronizadores, 9);
        }
    }
}

// Analisa mais declarações de constantes
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
                else
                {
                    erro("Esperado número após '=' na constante");
                }
            }
            else
            {
                erro("Esperado '=' após identificador da constante");
            }
        }
        else
        {
            erro("Esperado identificador após 'CONST'");
        }
    }
}

// Analisa declaração de variáveis
void variavel()
{
    TokenTipo sincronizadores[] = {
        TOKEN_PROCEDURE,
        TOKEN_IDENTIFICADOR,
        TOKEN_CALL,
        TOKEN_BEGIN,
        TOKEN_IF,
        TOKEN_WHILE,
        TOKEN_SIMBOLO_PONTO,
        TOKEN_EOF};

    if (lookahead.tipo == TOKEN_VAR)
    {
        advance();

        if (match(TOKEN_IDENTIFICADOR))
        {
            mais_var();
            if (!match(TOKEN_SIMBOLO_PONTO_VIRGULA))
            {
                erro("Esperado ';' após declaração de variável");
                sincroniza(sincronizadores, 8);
            }
        }
        else
        {
            erro("Esperado identificador após 'VAR'");
            sincroniza(sincronizadores, 8);
        }
    }
}

// Analisa mais declarações de variáveis
void mais_var()
{
    if (lookahead.tipo == TOKEN_SIMBOLO_VIRGULA)
    {
        advance();
        if (match(TOKEN_IDENTIFICADOR))
        {
            mais_var();
        }
        else
        {
            erro("Esperado identificador após 'VAR'");
        }
    }
}

// Analisa declaração de procedimentos
void procedimento()
{
    TokenTipo sincronizadores[] = {
        TOKEN_IDENTIFICADOR,
        TOKEN_CALL,
        TOKEN_BEGIN,
        TOKEN_IF,
        TOKEN_WHILE,
        TOKEN_SIMBOLO_PONTO,
        TOKEN_EOF};

    if (lookahead.tipo == TOKEN_PROCEDURE)
    {
        advance();

        if (match(TOKEN_IDENTIFICADOR))
        {
            if (match(TOKEN_SIMBOLO_PONTO_VIRGULA))
            {
                bloco();
                if (match(TOKEN_SIMBOLO_PONTO_VIRGULA))
                {
                    procedimento();
                }
                else
                {
                    erro("Esperado ';' após bloco do procedimento");
                    sincroniza(sincronizadores, 7);
                }
            }
            else
            {
                erro("Esperado ';' após identificador de procedimento");
                sincroniza(sincronizadores, 7);
            }
        }
        else
        {
            erro("Esperado identificador após 'PROCEDURE'");
            sincroniza(sincronizadores, 7);
        }
    }
}

// Analisa comandos
void comando()
{
    // Conjunto de sincronização para comando
    TokenTipo sincronizadores[] = {
        TOKEN_SIMBOLO_PONTO_VIRGULA,
        TOKEN_END,
        TOKEN_EOF};

    switch (lookahead.tipo)
    {
    case TOKEN_IDENTIFICADOR:
        advance();
        if (!match(TOKEN_SIMBOLO_ATRIBUICAO))
        {
            erro("Esperado ':=' após identificador");
            sincroniza(sincronizadores, 3);
            return;
        }
        expressao();
        break;

    case TOKEN_CALL:
        advance();
        if (!match(TOKEN_IDENTIFICADOR))
        {
            erro("Esperado identificador após CALL");
            sincroniza(sincronizadores, 3);
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
            sincroniza(sincronizadores, 3);
            return;
        }
        break;

    case TOKEN_IF:
        advance();
        condicao();
        if (!match(TOKEN_THEN))
        {
            erro("Esperado THEN");
            sincroniza(sincronizadores, 3);
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
            sincroniza(sincronizadores, 3);
            return;
        }
        comando();
        break;

    default:
        // Nenhum comando válido encontrado — pode ser ε (vazio)
        break;
    }
}

// Analisa mais comandos
void mais_cmd()
{
    if (match(TOKEN_SIMBOLO_PONTO_VIRGULA))
    {
        comando();
        mais_cmd();
    }
}

// Analisa expressões
void expressao()
{
    operador_unario();
    termo();
    mais_termos();
}

// Analisa operador unário
void operador_unario()
{
    if (lookahead.tipo == TOKEN_SIMBOLO_MAIS || lookahead.tipo == TOKEN_SIMBOLO_MENOS)
    {
        advance();
    }
}

// Analisa termos
void termo()
{
    fator();
    mais_fatores();
}

// Analisa mais termos
void mais_termos()
{
    if (lookahead.tipo == TOKEN_SIMBOLO_MAIS || lookahead.tipo == TOKEN_SIMBOLO_MENOS)
    {
        advance();
        termo();
        mais_termos();
    }
}

// Analisa fatores
void fator()
{
    TokenTipo sincronizadores[] = {
        TOKEN_SIMBOLO_PONTO_VIRGULA,
        TOKEN_END,
        TOKEN_THEN,
        TOKEN_DO,
        TOKEN_EOF};

    switch (lookahead.tipo)
    {
    case TOKEN_IDENTIFICADOR:
    case TOKEN_NUMERO:
        advance();
        break;
    case TOKEN_SIMBOLO_ABRE_PARENTESIS:
        advance();
        expressao();
        if (!match(TOKEN_SIMBOLO_FECHA_PARENTESIS))
        {
            erro("Esperado ')' após expressão");
            sincroniza(sincronizadores, 5);
        }
        break;
    default:
        erro("Esperado fator (ident, num ou '(')");
        sincroniza(sincronizadores, 5);
        break;
    }
}

// Analisa mais fatores
void mais_fatores()
{
    if (lookahead.tipo == TOKEN_SIMBOLO_VEZES || lookahead.tipo == TOKEN_SIMBOLO_DIVISAO)
    {
        advance();
        fator();
        mais_fatores();
    }
}

// Analisa condições
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

// Analisa operadores relacionais
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

// Inicia a análise sintática
void analise_sintatica()
{
    advance();
    programa();

    advance();
    if (lookahead.tipo != TOKEN_EOF)
    {
        erro("Token extra após fim do programa");
    }
}