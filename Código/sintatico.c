#include "sintatico.h"

// Variáveis globais
Token lookahead;               // Token atual analisado
FILE *arquivo_saida_sintatico; // Arquivo de saída da análise sintática

// Retorna a descrição textual de um tipo de token
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

// Sincronizadores para recuperação de erros
TokenTipo sincronizadores_programa[] = {TOKEN_SIMBOLO_PONTO, TOKEN_EOF};

TokenTipo sincronizadores_constante[] = {
    TOKEN_VAR, TOKEN_PROCEDURE, TOKEN_BEGIN, TOKEN_IF, TOKEN_WHILE, TOKEN_SIMBOLO_PONTO, TOKEN_EOF};

TokenTipo sincronizadores_mais_const[] = {
    TOKEN_VAR, TOKEN_PROCEDURE, TOKEN_BEGIN, TOKEN_IF, TOKEN_WHILE, TOKEN_SIMBOLO_PONTO, TOKEN_EOF};

TokenTipo sincronizadores_variavel[] = {
    TOKEN_PROCEDURE, TOKEN_BEGIN, TOKEN_IF, TOKEN_WHILE, TOKEN_SIMBOLO_PONTO, TOKEN_EOF};

TokenTipo sincronizadores_mais_var[] = {
    TOKEN_PROCEDURE, TOKEN_BEGIN, TOKEN_IF, TOKEN_WHILE, TOKEN_SIMBOLO_PONTO, TOKEN_EOF};

TokenTipo sincronizadores_procedimento[] = {
    TOKEN_BEGIN, TOKEN_IF, TOKEN_WHILE, TOKEN_SIMBOLO_PONTO, TOKEN_EOF};

TokenTipo sincronizadores_comando[] = {
    TOKEN_SIMBOLO_PONTO_VIRGULA, TOKEN_END, TOKEN_SIMBOLO_PONTO, TOKEN_EOF};

TokenTipo sincronizadores_fator[] = {
    TOKEN_SIMBOLO_PONTO_VIRGULA, TOKEN_END, TOKEN_THEN, TOKEN_DO, TOKEN_EOF};

TokenTipo sincronizadores_relacional[] = {
    TOKEN_SIMBOLO_PONTO_VIRGULA, TOKEN_END, TOKEN_THEN, TOKEN_DO, TOKEN_EOF};

// Avança para o próximo token válido
void advance(void)
{
    lookahead = obter_token();
    while (lookahead.tipo == TOKEN_ERRO)
    {
        fprintf(arquivo_saida_sintatico, "Erro léxico na linha %d: %s\n", lookahead.linha, lookahead.lexema);
        lookahead = obter_token();
    }
}

// Registra um erro sintático
void erro(const char *msg)
{
    fprintf(arquivo_saida_sintatico, "Erro sintático na linha %d: %s. Token atual: %s\n", lookahead.linha, msg, lookahead.lexema);
}

// Sincroniza o analisador após um erro
void sincroniza(TokenTipo sincronizadores[], int tamanho)
{
    int encontrou = 0;
    while (!encontrou && lookahead.tipo != TOKEN_EOF)
    {
        for (int i = 0; i < tamanho; i++)
        {
            if (lookahead.tipo == sincronizadores[i])
            {
                encontrou = 1;
                break;
            }
        }
        if (!encontrou)
            advance();
    }
}

// Analisa um programa completo
void programa()
{
    bloco();
    if (lookahead.tipo != TOKEN_SIMBOLO_PONTO)
    {
        erro("Esperado '.' ao final do programa");
        sincroniza(sincronizadores_programa, sizeof(sincronizadores_programa) / sizeof(TokenTipo));
    }
    else
    {
        advance();
    }
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
    if (lookahead.tipo == TOKEN_CONST)
    {
        advance();
        if (lookahead.tipo == TOKEN_IDENTIFICADOR)
        {
            advance();
            if (lookahead.tipo == TOKEN_SIMBOLO_IGUAL)
            {
                advance();
                if (lookahead.tipo == TOKEN_NUMERO)
                {
                    advance();
                    mais_const();
                    if (lookahead.tipo != TOKEN_SIMBOLO_PONTO_VIRGULA)
                    {
                        erro("Esperado ';' após declaração de constante");
                        sincroniza(sincronizadores_constante, sizeof(sincronizadores_constante) / sizeof(TokenTipo));
                    }
                    else
                    {
                        advance();
                    }
                }
                else
                {
                    erro("Esperado número após '=' na constante");
                    sincroniza(sincronizadores_constante, sizeof(sincronizadores_constante) / sizeof(TokenTipo));
                }
            }
            else
            {
                erro("Esperado '=' após identificador da constante");
                sincroniza(sincronizadores_constante, sizeof(sincronizadores_constante) / sizeof(TokenTipo));
            }
        }
        else
        {
            erro("Esperado identificador após 'CONST'");
            sincroniza(sincronizadores_constante, sizeof(sincronizadores_constante) / sizeof(TokenTipo));
        }
    }
}

// Analisa mais declarações de constantes
void mais_const()
{
    if (lookahead.tipo == TOKEN_SIMBOLO_VIRGULA)
    {
        advance();
        if (lookahead.tipo == TOKEN_IDENTIFICADOR)
        {
            advance();
            if (lookahead.tipo == TOKEN_SIMBOLO_IGUAL)
            {
                advance();
                if (lookahead.tipo == TOKEN_NUMERO)
                {
                    advance();
                    mais_const();
                }
                else
                {
                    erro("Esperado número após '=' na constante");
                    sincroniza(sincronizadores_mais_const, sizeof(sincronizadores_mais_const) / sizeof(TokenTipo));
                }
            }
            else
            {
                erro("Esperado '=' após identificador na declaração de constante");
                sincroniza(sincronizadores_mais_const, sizeof(sincronizadores_mais_const) / sizeof(TokenTipo));
            }
        }
        else
        {
            erro("Esperado identificador após ',' em declaração de constante");
            sincroniza(sincronizadores_mais_const, sizeof(sincronizadores_mais_const) / sizeof(TokenTipo));
        }
    }
}

// Analisa declaração de variáveis
void variavel()
{
    if (lookahead.tipo == TOKEN_VAR)
    {
        advance();
        if (lookahead.tipo == TOKEN_IDENTIFICADOR)
        {
            advance();
            mais_var();
            if (lookahead.tipo != TOKEN_SIMBOLO_PONTO_VIRGULA)
            {
                erro("Esperado ';' após declaração de variável");
                sincroniza(sincronizadores_variavel, sizeof(sincronizadores_variavel) / sizeof(TokenTipo));
            }
            else
            {
                advance();
            }
        }
        else
        {
            erro("Esperado identificador após 'VAR'");
            sincroniza(sincronizadores_variavel, sizeof(sincronizadores_variavel) / sizeof(TokenTipo));
        }
    }
}

// Analisa mais declarações de variáveis
void mais_var()
{
    if (lookahead.tipo == TOKEN_SIMBOLO_VIRGULA)
    {
        advance();
        if (lookahead.tipo == TOKEN_IDENTIFICADOR)
        {
            advance();
            mais_var();
        }
        else
        {
            erro("Esperado identificador após ',' em declaração de variável");
            sincroniza(sincronizadores_mais_var, sizeof(sincronizadores_mais_var) / sizeof(TokenTipo));
        }
    }
}

// Analisa declaração de procedimentos
void procedimento()
{
    if (lookahead.tipo == TOKEN_PROCEDURE)
    {
        advance();
        if (lookahead.tipo == TOKEN_IDENTIFICADOR)
        {
            advance();
            if (lookahead.tipo == TOKEN_SIMBOLO_PONTO_VIRGULA)
            {
                advance();
                bloco();
                if (lookahead.tipo == TOKEN_SIMBOLO_PONTO_VIRGULA)
                {
                    advance();
                    procedimento();
                }
                else
                {
                    erro("Esperado ';' após bloco do procedimento");
                    sincroniza(sincronizadores_procedimento, sizeof(sincronizadores_procedimento) / sizeof(TokenTipo));
                }
            }
            else
            {
                erro("Esperado ';' após identificador de procedimento");
                sincroniza(sincronizadores_procedimento, sizeof(sincronizadores_procedimento) / sizeof(TokenTipo));
            }
        }
        else
        {
            erro("Esperado identificador após 'PROCEDURE'");
            sincroniza(sincronizadores_procedimento, sizeof(sincronizadores_procedimento) / sizeof(TokenTipo));
        }
    }
}

// Analisa comandos
void comando()
{
    switch (lookahead.tipo)
    {
    case TOKEN_IDENTIFICADOR:
        advance();
        if (lookahead.tipo != TOKEN_SIMBOLO_ATRIBUICAO)
        {
            erro("Esperado ':=' após identificador");
            sincroniza(sincronizadores_comando, sizeof(sincronizadores_comando) / sizeof(TokenTipo));
        }
        else
        {
            advance();
            expressao();
        }
        break;

    case TOKEN_CALL:
        advance();
        if (lookahead.tipo != TOKEN_IDENTIFICADOR)
        {
            erro("Esperado identificador após CALL");
            sincroniza(sincronizadores_comando, sizeof(sincronizadores_comando) / sizeof(TokenTipo));
        }
        else
        {
            advance();
        }
        break;

    case TOKEN_BEGIN:
        advance();
        comando();
        mais_cmd();
        if (lookahead.tipo != TOKEN_END)
        {
            erro("Esperado END");
            sincroniza(sincronizadores_comando, sizeof(sincronizadores_comando) / sizeof(TokenTipo));
        }
        else
        {
            advance();
        }
        break;

    case TOKEN_IF:
        advance();
        condicao();
        if (lookahead.tipo != TOKEN_THEN)
        {
            erro("Esperado THEN");
            sincroniza(sincronizadores_comando, sizeof(sincronizadores_comando) / sizeof(TokenTipo));
        }
        else
        {
            advance();
            comando();
        }
        break;

    case TOKEN_WHILE:
        advance();
        condicao();
        if (lookahead.tipo != TOKEN_DO)
        {
            erro("Esperado DO");
            sincroniza(sincronizadores_comando, sizeof(sincronizadores_comando) / sizeof(TokenTipo));
        }
        else
        {
            advance();
            comando();
        }
        break;

    default:
        break;
    }
}

// Analisa mais comandos
void mais_cmd()
{
    if (lookahead.tipo == TOKEN_SIMBOLO_PONTO_VIRGULA)
    {
        advance();
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
    switch (lookahead.tipo)
    {
    case TOKEN_IDENTIFICADOR:
    case TOKEN_NUMERO:
        advance();
        break;
    case TOKEN_SIMBOLO_ABRE_PARENTESIS:
        advance();
        expressao();
        if (lookahead.tipo != TOKEN_SIMBOLO_FECHA_PARENTESIS)
        {
            erro("Esperado ')' após expressão");
            sincroniza(sincronizadores_fator, sizeof(sincronizadores_fator) / sizeof(TokenTipo));
        }
        else
        {
            advance();
        }
        break;
    default:
        erro("Esperado fator (ident, num ou '(')");
        sincroniza(sincronizadores_fator, sizeof(sincronizadores_fator) / sizeof(TokenTipo));
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
        erro("Esperado operador relacional (=, <>, <, <=, >, >=)");
        sincroniza(sincronizadores_relacional, sizeof(sincronizadores_relacional) / sizeof(TokenTipo));
        break;
    }
}

// Inicia a análise sintática
void analise_sintatica(void)
{
    advance();
    programa();
}