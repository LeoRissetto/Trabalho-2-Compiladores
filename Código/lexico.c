#include "lexico.h"

// Variáveis globais
char caractere_atual; // Caractere atual sendo analisado
FILE *arquivo_fonte;  // Ponteiro para o arquivo fonte
int num_line = 1;     // Contador de linhas do arquivo fonte

// Tabela de palavras reservadas da linguagem
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

// Tabela de símbolos reservados da linguagem
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

/**
 * Lê o próximo caractere do arquivo fonte
 */
void ler_caractere()
{
    caractere_atual = fgetc(arquivo_fonte);
}

/**
 * Retrocede um caractere no arquivo fonte
 */
void retroceder()
{
    if (caractere_atual != EOF)
    {
        ungetc(caractere_atual, arquivo_fonte);
    }
}

/**
 * Converte uma string para maiúsculas
 * @param str String a ser convertida
 * @return Nova string em maiúsculas
 */
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

/**
 * Verifica se um identificador é uma palavra reservada
 * @param identificador Identificador a ser verificado
 * @param valor Ponteiro para armazenar o valor do token
 * @return Tipo do token (TOKEN_IDENTIFICADOR ou tipo da palavra reservada)
 */
int obter_palavra_reservada(const char *identificador, char **valor)
{
    char *identificador_maiusculo = converter_para_maiusculo(identificador);

    int tipo = TOKEN_IDENTIFICADOR;
    *valor = strdup(identificador);

    for (size_t i = 0; i < sizeof(palavras) / sizeof(palavras[0]); i++)
    {
        if (strcmp(palavras[i].lexema, identificador_maiusculo) == 0)
        {
            tipo = palavras[i].tipo;
            *valor = strdup(palavras[i].lexema);
            break;
        }
    }

    free(identificador_maiusculo);
    return tipo;
}

/**
 * Obtém informações sobre um símbolo
 * @param simbolo Símbolo a ser analisado
 * @return Estrutura com informações do símbolo
 */
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

/**
 * Cria um novo token com as informações fornecidas
 * @param tipo Tipo do token
 * @param lexema Lexema do token
 * @param valor Valor do token
 * @return Token criado
 */
static Token criar_token(TokenTipo tipo, const char *lexema, const char *valor)
{
    Token token = {
        .tipo = tipo,
        .lexema = strdup(lexema),
        .valor = strdup(valor),
        .linha = num_line};
    return token;
}

/**
 * Obtém o próximo token do arquivo fonte
 * @return Token encontrado
 */
Token obter_token()
{
    ler_caractere();

    switch (caractere_atual)
    {
    // Reconhecimento de identificadores e palavras reservadas
    case 'a' ... 'z':
    case 'A' ... 'Z':
    {
        int i = 0;
        char *identificador = malloc(sizeof(char));
        while (isalnum(caractere_atual))
        {
            identificador = realloc(identificador, (i + 1) * sizeof(char));
            identificador[i++] = caractere_atual;
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

    // Reconhecimento de números
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

        // Verifica se o número é mal formado (contém letras)
        if (isalpha(caractere_atual))
        {
            while (isalnum(caractere_atual))
            {
                numero = realloc(numero, (i + 1) * sizeof(char));
                numero[i++] = caractere_atual;
                ler_caractere();
            }
            numero[i] = '\0';
            Token token = criar_token(TOKEN_ERRO, numero, "numero mal formado");
            free(numero);
            return token;
        }

        retroceder();
        Token token = criar_token(TOKEN_NUMERO, numero, "numero");
        free(numero);
        return token;
    }

    // Reconhecimento de atribuição (:=)
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
            return criar_token(TOKEN_ERRO, ":", "caractere invalido");
        }
    }

    // Reconhecimento de operadores relacionais (<, <=, <>)
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

    // Reconhecimento de operadores relacionais (>, >=)
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

    // Ignora espaços em branco
    case ' ':
    case '\t':
    case '\r':
    case '\v':
    case '\f':
    {
        return obter_token();
    }

    // Incrementa contador de linhas
    case '\n':
    {
        num_line++;
        return obter_token();
    }

    // Reconhecimento de comentários
    case '{':
    {
        while (caractere_atual != '}')
        {
            ler_caractere();
            if (caractere_atual == EOF || caractere_atual == '\n')
            {
                retroceder();
                return criar_token(TOKEN_ERRO, "{", "comentario nao fechado");
            }
        }

        return obter_token();
    }

    // Fim do arquivo
    case EOF:
    {
        return criar_token(TOKEN_EOF, "EOF", "fim_de_arquivo");
    }

    // Reconhecimento de outros símbolos
    default:
    {
        SimboloInfo info = obter_simbolo(caractere_atual);
        if (info.tipo != -1)
        {
            return criar_token(info.tipo, info.nome, info.nome);
        }
        else
        {
            char erro[2] = {caractere_atual, '\0'};
            return criar_token(TOKEN_ERRO, erro, "caractere invalido");
        }
    }
    }
}

/**
 * Libera a memória alocada para um token
 * @param token Token a ser liberado
 */
void liberar_token(Token *token)
{
    if (token != NULL)
    {
        free(token->lexema);
        free(token->valor);
    }
}