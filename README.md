# MAC0122 - Exercício de Programação 1 (EP1)

## 📌 Gerador de Páginas HTML e Syntax Highlighter para C

Este repositório contém a implementação do **EP1 de MAC0122 (Princípios de Desenvolvimento de Algoritmos)**. O objetivo principal do projeto é desenvolver um sistema escrito em **C** capaz de processar arquivos de texto formatados (`.txt`) e gerar um site estático navegável (`.html`) organizando o conteúdo do curso, além de incluir um *highlighter* de código-fonte C.

---

## 🚀 Funcionalidades

1. **Geração de Site Estático HTML:**
   - Leitura e parsing de arquivos `.txt` contendo cabeçalhos/rótulos específicos.
   - Geração automática da página principal (`index.html`).
   - Construção de 6 páginas indexadoras e organizadoras por categoria:
     - **Técnica:** Método guloso, Divisão e Conquista, etc.
     - **Algoritmo:** Ordenação por Seleção Direta, Mergesort, Kruskal, Dijkstra, etc.
     - **Autores:** Joseph Kruskal, Edsger W. Dijkstra, etc.
     - **Data:** Ordenação cronológica das descobertas/algoritmos.
     - **Conceitos:** Grafos, Árvores, Recursividade, Union-Find, etc.
     - **Problema:** Formulação e definição do problema com links cruzados para conceitos.

2. **Filtro / Syntax Highlighter para C:**
   - Módulo responsável por converter arquivos `.c` em páginas HTML com destaque de sintaxe (*syntax highlighting*).
   - Cores estruturadas baseadas nos padrões da Wikipedia para os seguintes itens léxicos:
     - Directivas e Pragmas (`#include`, `#define`, etc.)
     - Tipos de Dados / Variáveis (`int`, `float`, `char`, etc.)
     - Estruturas de Controle e Comandos (`if`, `for`, `while`, etc.)
     - Comentários de linha e de bloco (`//` e `/* ... */`)
     - Cadeias de Caracteres / Strings (`"texto\n"`)

---

## 📝 Formato dos Arquivos de Entrada (`.txt`)

Cada arquivo `.txt` de entrada representa uma aula ou tópico do curso e possui marcadores no início. Marcadores vazios são ignorados pelo sistema.

**Exemplo (`kruskal.txt`):**
```text
Técnica: Método guloso
Algoritmo: Kruskal para árvore espalhada mínima
Autores: Joseph Kruskal
Data: 1956
Conceitos: Grafos, Árvores, árvore espalhada mínima, union-find
Problema: Dado um grafo não dirigido com pesos nas arestas, determinar sua árvore espalhada mínima.

[Conteúdo explicativo da aula e referências bibliográficas aqui...]
```

---

## 📂 Estrutura do Repositório

```text
.
├── src/                    # Código-fonte C do gerador e analisador léxico
├── txt_inputs/             # Arquivos .txt contendo o material didático (Avaliação 1)
├── c_sources/              # Códigos C de exemplo (ex: Kruskal, Dijkstra) e entradas auxiliares
├── docs/                   # Documentações exigidas para entrega
│   ├── contribuicoes.pdf   # Relatório de contribuição de cada membro
│   ├── dialogos_ia.md      # Registro das interações com mecanismos de IA
│   ├── relatorio_ia.md     # Análise de dificuldades, prós e contras das IAs
│   └── compilacao.md       # Instruções detalhadas de compilação
└── README.md               # Este arquivo
```

---

## 🛠️ Como Compilar e Executar

Para instruções detalhadas sobre a compilação de cada programa específico e seus módulos auxiliares, consulte o arquivo [`docs/compilacao.md`](docs/compilacao.md).

### Exemplo Rápido:
```bash
# Compilar o gerador principal
gcc -Wall -pedantic -std=c99 -o gerador_html src/main.c src/parser.c src/highlighter.c

# Executar o gerador sobre a pasta de entradas
./gerador_html txt_inputs/ c_sources/
```

---

## 📦 Entregáveis do Projeto

Conforme as especificações do EP1, os seguintes itens compõem a entrega oficial:

- [x] **Identificação do Grupo:** NUSP e nomes dos 4 integrantes na área de entrega.
- [x] **Relatório de Contribuição:** Descrição detalhada do papel de cada membro (1 página).
- [x] **Diálogos com IAs:** Histórico do processo interativo com ferramentas de Inteligência Artificial.
- [x] **Análise das IAs:** Relatório comparativo destacando facilidades, dificuldades e ranking das IAs utilizadas.
- [x] **Instruções de Compilação:** Guia passo a passo de build dos códigos C.
- [x] **Pacote de Fontes TXT:** Arquivo compactado contendo todo o conteúdo processado da Avaliação 1.
- [x] **Pacote de Fontes C:** Arquivo compactado com todos os algoritmos funcionais em C e arquivos `.txt` de teste/grafos.

---

## 👥 Autores e Integrantes do Grupo

| Nome | NUSP |
| :--- | :--- |
| Nome do Integrante 1 | 00000000 |
| Nome do Integrante 2 | 00000000 |
| Davi Gaioso Manso     | 17862852 |
| Fernando Nemec Vieira | 17889820 |

---
*Projeto desenvolvido para a disciplina MAC0122 - Instituto de Matemática e Estatística da Universidade de São Paulo (IME-USP).*
