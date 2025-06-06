# Relatório - Analisador Léxico e Sintático para PL/0

## Identificação

- **Disciplina:** SCC0605 Teoria da Computação e Compiladores  
- **Instituição:** ICMC/USP  
- **Professor:** Thiago A. S. Pardo  
- **Trabalho 2:** Análise Sintática  
- **Grupo:**  
  - Leonardo Gueno Rissetto NUSP: 13676482  
  - Lucas Lima Romero NUSP: 13676325  
  - Luciano Gonçalves Lopes Filho NUSP: 13676520  
  - Marco Garcia NUSP: 11833581  
  - Thiago Kashivagui Gonçalves NUSP: 13676579

---

## 1. Decisões de Projeto

- **Léxico:**  
  O analisador léxico foi implementado em C, utilizando tabelas hash para reconhecimento eficiente de palavras e símbolos reservados. O léxico reconhece identificadores, números, palavras reservadas, operadores e símbolos, além de tratar erros léxicos (números mal formados, comentários não fechados, caracteres inválidos).

- **Sintático:**  
  O analisador sintático segue o modelo descendente preditivo recursivo, com uma função para cada não-terminal da gramática PL/0. O tratamento de erros sintáticos é feito pelo modo pânico, utilizando conjuntos de sincronização para cada produção.

- **Integração:**  
  O analisador sintático consome tokens do léxico, reportando todos os erros encontrados em um arquivo de saída. Se não houver erros, uma mensagem de sucesso é exibida.

- **Modularidade:**  
  O código está dividido em módulos:  
  - `lexico.*` (analisador léxico)  
  - `sintatico.*` (analisador sintático)  
  - `hash_table.*` (tabelas hash para palavras/símbolos reservados)  
  - `main.c` (ponto de entrada)

---

## 2. Instruções para Compilar e Executar

### **Requisitos**
- Compilador C (testado com `gcc`)
- Python 3 (para interface)
- Sistema operacional: Linux, macOS ou Windows

### **Compilação**
No terminal, dentro da pasta `Código/`:
```sh
make
```

### **Execução via interface Python**
O modo recomendado de uso é pela interface Python fornecida no arquivo `interface_tester.py` na raiz do projeto. Ela automatiza a execução do analisador e facilita a interação com o usuário.

No terminal, execute:
```sh
python3 interface_tester.py
```

### **Execução manual (opcional)**
O programa espera um arquivo de entrada em `input/codigo.pl0` e gera a saída em `output/saida_sintatico.txt`:
```sh
./compilador
```

### **Parâmetros**
- **Entrada:** `input/codigo.pl0` (programa em PL/0)
- **Saída:** `output/saida_sintatico.txt` (relatório de erros ou sucesso)

---

## 3. Exemplo de Execução

### **Entrada (`input/codigo.pl0`):**
```pl0
CONST a = 10, b = 20;
VAR x, y;
BEGIN
  x := a + b;
  y := x * 2
END.
```

### **Saída (`output/saida_sintatico.txt`):**
```
Compilação bem-sucedida. Nenhum erro encontrado.
```

### **Exemplo com erro:**
```pl0
CONST a = 10, = 20;
```
Saída:
```
Erro sintático na linha 1: Esperado identificador após ',' em declaração de constante. Token atual: =
```

---

## 4. Decisões e Observações

- **Tratamento de erros:**  
  O modo pânico permite que o analisador continue após encontrar um erro, relatando múltiplos problemas em uma única execução.
- **Mensagens:**  
  As mensagens de erro informam a linha, o tipo de erro e o token atual, facilitando a depuração.
- **Extensibilidade:**  
  O uso de tabelas hash facilita a adição de novas palavras ou símbolos reservados.

---

## 5. Como testar

- Modifique o arquivo `input/codigo.pl0` com diferentes programas PL/0.
- Execute a interface Python (`python3 interface_tester.py`) e verifique o arquivo de saída.
- Teste casos com e sem erros para validar o tratamento de erros.

---

## 6. Observações Finais

- O código está documentado internamente para facilitar a manutenção.
- Para dúvidas ou problemas, consulte os comentários nos arquivos fonte.

---
