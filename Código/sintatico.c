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

// Avança para o próximo token válido
void advance(void)
{
    lookahead = obter_token();
    while (lookahead.tipo == TOKEN_ERRO)
    {
        fprintf(arquivo_saida_sintatico, "Erro léxico na linha %d: %s. Token atual: %s\n", lookahead.linha, lookahead.valor, lookahead.lexema);

        // Se for número mal formado, trata como TOKEN_NUMERO
        if (lookahead.valor && strcmp(lookahead.valor, "Número Mal Formado") == 0)
        {
            lookahead.tipo = TOKEN_NUMERO;
            break;
        }
        else if (strcmp(lookahead.lexema, ":") == 0)
        {
            lookahead.tipo = TOKEN_SIMBOLO_ATRIBUICAO;
            break;
        }

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
    // Conjunto de sincronização para programa
    TokenTipo sincronizadores[] = {
        TOKEN_EOF};

    bloco();
    if (lookahead.tipo != TOKEN_SIMBOLO_PONTO)
    {
        erro("Esperado '.' ao final do programa");
        sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
    }
    else
    {
        advance();
        if (lookahead.tipo != TOKEN_EOF)
        {
            erro("Tokens após final do programa");
        }
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
                        sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
                    }
                    else
                    {
                        advance();
                    }
                }
                else
                {
                    erro("Esperado número após '=' na constante");
                    sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
                }
            }
            else
            {
                erro("Esperado '=' após identificador da constante");
                sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
            }
        }
        else
        {
            erro("Esperado identificador após 'CONST'");
            sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
        }
    }
}

// Analisa mais declarações de constantes
void mais_const()
{
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
                    sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
                }
            }
            else
            {
                erro("Esperado '=' após identificador na declaração de constante");
                sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
            }
        }
        else
        {
            erro("Esperado identificador após ',' em declaração de constante");
            sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
        }
    }
    else if (lookahead.tipo == TOKEN_IDENTIFICADOR)
    {
        erro("Esperado ',' entre declaração de constantes");
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
                sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
            }
        }
        else
        {
            erro("Esperado '=' após identificador na declaração de constante");
            sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
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
        if (lookahead.tipo == TOKEN_IDENTIFICADOR)
        {
            advance();
            mais_var();
            if (lookahead.tipo != TOKEN_SIMBOLO_PONTO_VIRGULA)
            {
                erro("Esperado ';' após declaração de variável");
                sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
            }
            else
            {
                advance();
            }
        }
        else
        {
            erro("Esperado identificador após 'VAR'");
            sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
        }
    }
}

// Analisa mais declarações de variáveis
void mais_var()
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
            sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
        }
    }
    else if (lookahead.tipo == TOKEN_IDENTIFICADOR)
    {
        erro("Esperado ',' apoós declaração de variável");
        advance();
        mais_var();
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
                    sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
                }
            }
            else
            {
                erro("Esperado ';' após identificador de procedimento");
                sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
            }
        }
        else
        {
            erro("Esperado identificador após 'PROCEDURE'");
            sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
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
        if (lookahead.tipo != TOKEN_SIMBOLO_ATRIBUICAO)
        {
            erro("Esperado ':=' após identificador");
            sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
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
            sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
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
            sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
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
            sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
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
            sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
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
    else if (lookahead.tipo != TOKEN_END && lookahead.tipo != TOKEN_EOF)
    {
        erro("Esperado ';' para separar comandos ou END para finalizar o bloco");
        TokenTipo sincronizadores_mais_cmd[] = {
            TOKEN_END,
            TOKEN_SIMBOLO_PONTO_VIRGULA,
            TOKEN_EOF};
        sincroniza(sincronizadores_mais_cmd, sizeof(sincronizadores_mais_cmd) / sizeof(sincronizadores_mais_cmd[0]));

        if (lookahead.tipo == TOKEN_SIMBOLO_PONTO_VIRGULA)
        {
            mais_cmd();
        }
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
        TOKEN_SIMBOLO_FECHA_PARENTESIS,
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
        if (lookahead.tipo != TOKEN_SIMBOLO_FECHA_PARENTESIS)
        {
            erro("Esperado ')' após expressão");
            sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
        }
        else
        {
            advance();
        }
        break;
    default:
        erro("Esperado fator (ident, num ou '(')");
        sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
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
    else if (lookahead.tipo == TOKEN_IDENTIFICADOR || lookahead.tipo == TOKEN_NUMERO || lookahead.tipo == TOKEN_SIMBOLO_ABRE_PARENTESIS)
    {
        erro("Esperado operador aritmético (+, -, * ou /)");
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
    TokenTipo sincronizadores[] = {
        TOKEN_IDENTIFICADOR,
        TOKEN_NUMERO,
        TOKEN_SIMBOLO_ABRE_PARENTESIS,
        TOKEN_SIMBOLO_PONTO_VIRGULA,
        TOKEN_END,
        TOKEN_THEN,
        TOKEN_DO,
        TOKEN_EOF};

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
        sincroniza(sincronizadores, sizeof(sincronizadores) / sizeof(sincronizadores[0]));
        break;
    }
}

// Inicia a análise sintática
void analise_sintatica(void)
{
    advance();
    programa();
}