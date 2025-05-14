#include "lexico.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Global variables definition
char caractere_atual;
FILE *arquivo_fonte;

int num_line = 1;

// Arrays globais de palavras e símbolos reservados
static const PalavraReservada palavras[] = {
    {"CONST", TOKEN_CONST},
    {"VAR", TOKEN_VAR},
    {"PROCEDURE", TOKEN_PROCEDURE},
    {"CALL", TOKEN_CALL},
    {"BEGIN", TOKEN_BEGIN},
    {"END", TOKEN_END},
    {"IF", TOKEN_IF},
    {"THEN", TOKEN_THEN},
    {"WHILE", TOKEN_WHILE},
    {"DO", TOKEN_DO},
    {"ODD", TOKEN_ODD}};

static const SimboloReservado simbolos[] = {
    {",", "simbolo_virgula", TOKEN_SIMBOLO_VIRGULA},
    {";", "simbolo_ponto_virgula", TOKEN_SIMBOLO_PONTO_VIRGULA},
    {".", "simbolo_ponto", TOKEN_SIMBOLO_PONTO},
    {"(", "simbolo_abre_parenteses", TOKEN_SIMBOLO_ABRE_PARENTESIS},
    {")", "simbolo_fecha_parenteses", TOKEN_SIMBOLO_FECHA_PARENTESIS},
    {"+", "simbolo_mais", TOKEN_SIMBOLO_MAIS},
    {"-", "simbolo_menos", TOKEN_SIMBOLO_MENOS},
    {"*", "simbolo_vezes", TOKEN_SIMBOLO_VEZES},
    {"/", "simbolo_divisao", TOKEN_SIMBOLO_DIVISAO},
    {"=", "simbolo_igual", TOKEN_SIMBOLO_IGUAL}};

// Read next character from source file
void ler_caractere()
{
    caractere_atual = fgetc(arquivo_fonte);
}

// Move back one character in source file
void retroceder()
{
    if (caractere_atual != EOF)
    {
        ungetc(caractere_atual, arquivo_fonte);
    }
}

// Convert string to uppercase
char *converter_para_maiusculo(const char *str)
{
    char *nova_str = malloc(strlen(str) + 1);
    for (int i = 0; str[i]; i++)
    {
        nova_str[i] = toupper(str[i]);
    }
    nova_str[strlen(str)] = '\0';
    return nova_str;
}

int obter_palavra_reservada(const char *identificador, char **valor)
{
    char *identificador_maiusculo = converter_para_maiusculo(identificador);

    int tipo = TOKEN_IDENTIFICADOR;
    *valor = strdup(identificador); // Por padrão, mantém o valor original

    for (size_t i = 0; i < sizeof(palavras) / sizeof(palavras[0]); i++)
    {
        if (strcmp(palavras[i].lexema, identificador_maiusculo) == 0)
        {
            tipo = palavras[i].tipo;
            *valor = strdup(palavras[i].lexema); // Se for palavra reservada, usa a versão maiúscula
            break;
        }
    }

    free(identificador_maiusculo);
    return tipo;
}

SimboloInfo obter_simbolo(char simbolo)
{
    static char simbolo_str[2] = {0};
    simbolo_str[0] = simbolo;

    SimboloInfo info = {.tipo = -1, .nome = NULL};
    for (size_t i = 0; i < sizeof(simbolos) / sizeof(simbolos[0]); i++)
    {
        if (strcmp(simbolos[i].lexema, simbolo_str) == 0)
        {
            info.tipo = simbolos[i].tipo;
            info.nome = simbolos[i].nomeSimbolo;
            break;
        }
    }
    return info;
}

// Helper function to create a token
static Token criar_token(TokenTipo tipo, const char *lexema, const char *valor)
{
    Token token = {
        .tipo = tipo,
        .lexema = strdup(lexema),
        .valor = strdup(valor),
        .linha = num_line};
    return token;
}

// Main function to get tokens from source file
Token obter_token()
{
    ler_caractere();

    switch (caractere_atual)
    {
    // Handle identifiers (words)
    case 'a' ... 'z':
    case 'A' ... 'Z':
    {
        int i = 0;
        char *identificador = malloc(sizeof(char));
        while (isalnum(caractere_atual))
        {
            identificador = realloc(identificador, (i + 1) * sizeof(char));
            identificador[i++] = caractere_atual; // Mantém o caractere original
            ler_caractere();
        }
        identificador[i] = '\0';
        retroceder();

        char *valor;
        int tipo = obter_palavra_reservada(identificador, &valor);
        Token token = criar_token(tipo, identificador, valor);

        free(identificador);
        free(valor);
        return token;
    }

    // Handle numbers
    case '0' ... '9':
    {
        int i = 0;
        char *numero = malloc(sizeof(char));

        while (isdigit(caractere_atual))
        {
            numero = realloc(numero, (i + 1) * sizeof(char));
            numero[i++] = caractere_atual;
            ler_caractere();
        }
        numero[i] = '\0';

        // Check if there are non-numeric characters after digits
        if (isalpha(caractere_atual))
        {
            // Collect additional characters to show in error
            while (isalnum(caractere_atual))
            {
                numero = realloc(numero, (i + 1) * sizeof(char));
                numero[i++] = caractere_atual;
                ler_caractere();
            }
            numero[i] = '\0';
            Token token = criar_token(TOKEN_ERRO, numero, "<ERRO_LEXICO>: numero mal formado");
            free(numero);
            return token;
        }

        retroceder();
        Token token = criar_token(TOKEN_NUMERO, numero, "numero");
        free(numero);
        return token;
    }

    // Handle operators and special symbols
    case ':':
    {
        ler_caractere();
        if (caractere_atual == '=')
        {
            return criar_token(TOKEN_SIMBOLO_ATRIBUICAO, ":=", "simbolo_atribuicao");
        }
        else
        {
            retroceder();
            return criar_token(TOKEN_ERRO, ":", "<ERRO_LEXICO>: caractere invalido");
        }
    }

    case '<':
    {
        ler_caractere();
        if (caractere_atual == '=')
        {
            return criar_token(TOKEN_SIMBOLO_MENOR_IGUAL, "<=", "simbolo_menor_igual");
        }
        else if (caractere_atual == '>')
        {
            return criar_token(TOKEN_SIMBOLO_DIFERENTE, "<>", "simbolo_diferente");
        }
        else
        {
            retroceder();
            return criar_token(TOKEN_SIMBOLO_MENOR, "<", "simbolo_menor");
        }
    }

    case '>':
    {
        ler_caractere();
        if (caractere_atual == '=')
        {
            return criar_token(TOKEN_SIMBOLO_MAIOR_IGUAL, ">=", "simbolo_maior_igual");
        }
        else
        {
            retroceder();
            return criar_token(TOKEN_SIMBOLO_MAIOR, ">", "simbolo_maior");
        }
    }

    case ' ':
    case '\t':
    case '\r':
    case '\v':
    case '\f':
    {
        return obter_token(); // Skip whitespace by getting next token
    }

    case '\n':
    {
        num_line++;
        return obter_token();
    }

    case '{':
    {
        while (caractere_atual != '}')
        {
            ler_caractere();
            if (caractere_atual == EOF || caractere_atual == '\n')
            {
                retroceder();
                return criar_token(TOKEN_ERRO, "{", "<ERRO_LEXICO>: comentario nao fechado");
            }
        }

        return obter_token();
    }

    case EOF:
    {
        return criar_token(TOKEN_EOF, "EOF", "fim_de_arquivo");
    }

    default:
    {
        // Handle simple symbols
        if (caractere_atual == '}')
        {
            return criar_token(TOKEN_ERRO, "}", "<ERRO_LEXICO>: comentario nao aberto");
        }
        else
        {
            char simbolo_str[2] = {caractere_atual, '\0'};
            SimboloInfo info = obter_simbolo(caractere_atual);

            if (info.tipo != -1)
            {
                return criar_token(info.tipo, simbolo_str, info.nome);
            }
            else
            {
                return criar_token(TOKEN_ERRO, simbolo_str, "<ERRO_LEXICO>: caractere invalido>");
            }
        }
    }
    }
}

// Function to free token memory
void liberar_token(Token *token)
{
    if (token)
    {
        free(token->lexema);
        free(token->valor);
    }
}