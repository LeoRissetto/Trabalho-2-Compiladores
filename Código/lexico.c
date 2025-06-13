
#include "lexico.h"
#include "hash_table.h"

// Variáveis globais
char caractere_atual; // Caractere atual sendo analisado
FILE *arquivo_fonte;  // Ponteiro para o arquivo fonte
int numero_linha = 1; // Contador de linhas do arquivo fonte

// Inicialização das tabelas hash
void inicializar_tabelas_hash()
{
    // Palavras reservadas
    inserir_palavra_reservada("CONST", TOKEN_CONST);
    inserir_palavra_reservada("VAR", TOKEN_VAR);
    inserir_palavra_reservada("PROCEDURE", TOKEN_PROCEDURE);
    inserir_palavra_reservada("CALL", TOKEN_CALL);
    inserir_palavra_reservada("BEGIN", TOKEN_BEGIN);
    inserir_palavra_reservada("END", TOKEN_END);
    inserir_palavra_reservada("IF", TOKEN_IF);
    inserir_palavra_reservada("THEN", TOKEN_THEN);
    inserir_palavra_reservada("WHILE", TOKEN_WHILE);
    inserir_palavra_reservada("DO", TOKEN_DO);
    inserir_palavra_reservada("ODD", TOKEN_ODD);

    // Símbolos reservados
    inserir_simbolo_reservado(",", "simbolo_virgula", TOKEN_SIMBOLO_VIRGULA);
    inserir_simbolo_reservado(";", "simbolo_ponto_virgula", TOKEN_SIMBOLO_PONTO_VIRGULA);
    inserir_simbolo_reservado(".", "simbolo_ponto", TOKEN_SIMBOLO_PONTO);
    inserir_simbolo_reservado("(", "simbolo_abre_parenteses", TOKEN_SIMBOLO_ABRE_PARENTESIS);
    inserir_simbolo_reservado(")", "simbolo_fecha_parenteses", TOKEN_SIMBOLO_FECHA_PARENTESIS);
    inserir_simbolo_reservado("+", "simbolo_mais", TOKEN_SIMBOLO_MAIS);
    inserir_simbolo_reservado("-", "simbolo_menos", TOKEN_SIMBOLO_MENOS);
    inserir_simbolo_reservado("*", "simbolo_vezes", TOKEN_SIMBOLO_VEZES);
    inserir_simbolo_reservado("/", "simbolo_divisao", TOKEN_SIMBOLO_DIVISAO);
    inserir_simbolo_reservado("=", "simbolo_igual", TOKEN_SIMBOLO_IGUAL);
}

// Lê o próximo caractere do arquivo fonte
void ler_caractere(void)
{
    caractere_atual = fgetc(arquivo_fonte);
}

// Retrocede um caractere no arquivo fonte
void retroceder(void)
{
    if (caractere_atual != EOF)
    {
        ungetc(caractere_atual, arquivo_fonte);
    }
}

// Converte uma string para maiúsculas
char *converter_para_maiusculo(const char *str)
{
    size_t tamanho = strlen(str);
    char *nova_str = malloc(tamanho + 1);
    if (!nova_str)
        return NULL;
    for (size_t i = 0; i < tamanho; i++)
    {
        nova_str[i] = toupper((unsigned char)str[i]);
    }
    nova_str[tamanho] = '\0';
    return nova_str;
}

// Verifica se um identificador é uma palavra reservada
int obter_palavra_reservada(const char *identificador, char **valor)
{
    char *identificador_maiusculo = converter_para_maiusculo(identificador);
    int tipo = TOKEN_IDENTIFICADOR;
    if (buscar_palavra_reservada(identificador_maiusculo, &tipo))
    {
        *valor = strdup(identificador_maiusculo);
    }
    else
    {
        *valor = strdup(identificador);
    }
    free(identificador_maiusculo);
    return tipo;
}

// Obtém informações sobre um símbolo
SimboloInfo obter_simbolo(char simbolo)
{
    static char simbolo_str[2] = {0};
    simbolo_str[0] = simbolo;
    SimboloInfo info = {.tipo = -1, .nome = NULL};
    int tipo;
    const char *nomeSimbolo = NULL;
    if (buscar_simbolo_reservado(simbolo_str, &tipo, &nomeSimbolo))
    {
        info.tipo = tipo;
        info.nome = nomeSimbolo;
    }
    return info;
}

// Cria um novo token com as informações fornecidas
static Token criar_token(TokenTipo tipo, const char *lexema, const char *valor)
{
    Token token = {
        .tipo = tipo,
        .lexema = strdup(lexema),
        .valor = strdup(valor),
        .linha = numero_linha};
    return token;
}

// Obtém o próximo token do arquivo fonte
// returna o Token encontrado

Token obter_token(void)
{
    ler_caractere();
    switch (caractere_atual)
    {
    // Identificadores e palavras reservadas
    case 'a' ... 'z':
    case 'A' ... 'Z':
    {
        int tamanho = 0;
        char *identificador = NULL;
        while (isalnum(caractere_atual))
        {
            identificador = realloc(identificador, (tamanho + 2) * sizeof(char));
            identificador[tamanho++] = caractere_atual;
            ler_caractere();
        }
        identificador[tamanho] = '\0';
        retroceder();
        char *valor = NULL;
        int tipo = obter_palavra_reservada(identificador, &valor);
        Token token = criar_token(tipo, identificador, valor);
        free(identificador);
        free(valor);
        return token;
    }
    // Números
    case '0' ... '9':
    {
        int tamanho = 0;
        char *numero = NULL;
        while (isdigit(caractere_atual))
        {
            numero = realloc(numero, (tamanho + 2) * sizeof(char));
            numero[tamanho++] = caractere_atual;
            ler_caractere();
        }
        numero[tamanho] = '\0';
        // Verifica se o número é mal formado (contém letras)
        if (isalpha(caractere_atual))
        {
            while (isalnum(caractere_atual))
            {
                numero = realloc(numero, (tamanho + 2) * sizeof(char));
                numero[tamanho++] = caractere_atual;
                ler_caractere();
            }
            numero[tamanho] = '\0';
            retroceder();
            Token token = criar_token(TOKEN_ERRO, numero, "Número Mal Formado");
            free(numero);
            return token;
        }
        retroceder();
        Token token = criar_token(TOKEN_NUMERO, numero, "numero");
        free(numero);
        return token;
    }

    // Atribuição (:=)
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
            return criar_token(TOKEN_ERRO, ":", "Caractere Inválido. Você quis dizer ':=' ?");
        }
    }

    // Operadores relacionais (<, <=, <>)
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

    // Operadores relacionais (>, >=)
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

    // Espaços em branco
    case ' ':
    case '\t':
    case '\r':
    case '\v':
    case '\f':
        return obter_token();
    // Nova linha
    case '\n':
        numero_linha++;
        return obter_token();
    // Comentários
    case '{':
    {
        while (caractere_atual != '}')
        {
            ler_caractere();
            if (caractere_atual == EOF || caractere_atual == '\n')
            {
                retroceder();
                return criar_token(TOKEN_ERRO, "{", "Comentário Não Fechado");
            }
        }
        return obter_token();
    }
    // Fim do arquivo
    case EOF:
        return criar_token(TOKEN_EOF, "EOF", "fim_de_arquivo");
    // Outros símbolos
    default:
    {
        SimboloInfo info = obter_simbolo(caractere_atual);
        if (info.tipo != -1)
        {
            char simbolo[2] = {caractere_atual, '\0'};
            return criar_token(info.tipo, simbolo, info.nome);
        }
        else
        {
            char erro[2] = {caractere_atual, '\0'};
            if (caractere_atual == '}')
                return criar_token(TOKEN_ERRO, erro, "Comentário não aberto");
            return criar_token(TOKEN_ERRO, erro, "Caractere Inválido");
        }
    }
    }
}

// Libera a memória alocada para um token
void liberar_token(Token *token)
{
    if (token != NULL)
    {
        free(token->lexema);
        free(token->valor);
    }
}