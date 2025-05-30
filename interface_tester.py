import tkinter as tk
from tkinter import filedialog, messagebox, scrolledtext
import subprocess
import os

# Caminhos
CODIGO_DIR = os.path.join(os.path.dirname(__file__), 'Código')
INPUT_DIR = os.path.join(CODIGO_DIR, 'input')
OUTPUT_DIR = os.path.join(CODIGO_DIR, 'output')
EXECUTAVEL = os.path.join(CODIGO_DIR, 'main')
ENTRADA = os.path.join(INPUT_DIR, 'codigo.pl0')
SAIDA = os.path.join(OUTPUT_DIR, 'saida_sintatico.txt')

class AnalisadorGUI:
    def __init__(self, root):
        self.root = root
        root.title('Testador Visual - Analisador Sintático/Léxico')

        # Área de texto para código-fonte
        self.text_area = scrolledtext.ScrolledText(root, width=100, height=28)
        self.text_area.pack(padx=10, pady=10)

        # Botões
        frame = tk.Frame(root)
        frame.pack()
        tk.Button(frame, text='Analisar', command=self.salvar_e_analisar).pack(side=tk.LEFT, padx=5)
        tk.Button(frame, text='Limpar', command=self.limpar).pack(side=tk.LEFT, padx=5)

        # Área de saída
        tk.Label(root, text='Saída do analisador:').pack()
        self.output_area = scrolledtext.ScrolledText(root, width=100, height=14, state='disabled')
        self.output_area.pack(padx=10, pady=10)



    def salvar_e_analisar(self):
        # Limpa marcações anteriores
        self.text_area.tag_remove('erro', '1.0', tk.END)

        # Salva o texto no arquivo de entrada
        codigo = self.text_area.get('1.0', tk.END)
        os.makedirs(INPUT_DIR, exist_ok=True)
        with open(ENTRADA, 'w', encoding='utf-8') as f:
            f.write(codigo)
        # Executa make run na pasta Código
        try:
            subprocess.run(['make', 'run'], cwd=CODIGO_DIR, check=True)
        except Exception as e:
            messagebox.showerror('Erro', f'Erro ao executar make run: {e}')
            return
        # Mostra a saída
        try:
            with open(SAIDA, 'r', encoding='utf-8') as f:
                saida = f.read()
        except Exception as e:
            saida = f'Erro ao ler saída: {e}'

        # Se não houver erro, avisa o usuário
        saida_strip = saida.strip()
        if not saida_strip:
            saida = 'Nenhum erro encontrado.'
        self.output_area.config(state='normal')
        self.output_area.delete('1.0', tk.END)
        self.output_area.insert(tk.END, saida)
        self.output_area.config(state='disabled')

        # Destaca linhas com erro no código-fonte
        linhas_erro = self._extrair_linhas_erro(saida)
        for linha in linhas_erro:
            self.text_area.tag_add('erro', f'{linha}.0', f'{linha}.0 lineend')
        self.text_area.tag_config('erro', background='#ffcccc')

    def _extrair_linhas_erro(self, saida):
        # Procura por padrões como "linha X" na saída do analisador
        import re
        linhas = set()
        for match in re.finditer(r'linha\s+(\d+)', saida):
            linhas.add(int(match.group(1)))
        return linhas

    def limpar(self):
        self.text_area.delete('1.0', tk.END)
        self.output_area.config(state='normal')
        self.output_area.delete('1.0', tk.END)
        self.output_area.config(state='disabled')

def on_close(root):
    try:
        subprocess.run(['make', 'clean'], cwd=CODIGO_DIR)
    except Exception:
        pass
    root.destroy()

if __name__ == '__main__':
    root = tk.Tk()
    app = AnalisadorGUI(root)
    root.protocol("WM_DELETE_WINDOW", lambda: on_close(root))
    root.mainloop()