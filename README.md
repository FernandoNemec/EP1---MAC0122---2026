# MAC0122 - Exercício de Programação 1 (EP1)

## 📌 Gerador de Páginas HTML e Syntax Highlighter para C

Este repositório contém a implementação do **EP1 de MAC0122 (Princípios de Desenvolvimento de Algoritmos)**. O objetivo principal do projeto é desenvolver um sistema escrito em **C** capaz de processar arquivos de texto formatados (`.txt`) e gerar um site estático navegável (`.html`) organizando o conteúdo do curso, além de incluir um *highlighter* de código-fonte C.

---

## 🚀 Funcionalidades

1. **Geração de Site Estático HTML:**
   - Leitura e parsing de arquivos `.txt` contendo cabeçalhos/rótulos específicos.
   - Geração automática da página principal (`index.html`).
   - Construção de 8 páginas indexadoras e organizadoras por categoria:
     - **Técnica:** Método guloso, Divisão e Conquista, etc.
     - **Algoritmo:** Ordenação por Seleção Direta, Mergesort, Kruskal, Dijkstra, etc.
     - **Autores:** Joseph Kruskal, Edsger W. Dijkstra, etc.
     - **Data:** Ordenação cronológica das descobertas/algoritmos.
     - **Conceitos:** Grafos, Árvores, Recursividade, Union-Find, etc.
     - **Problema:** Formulação e definição do problema com links cruzados para conceitos.
     - **Complexidade:** Separação dos algoritmos por complexidade.
     - **Semana:** Separação dos algoritmos pela respectiva semana de aula.

2. **Filtro / Syntax Highlighter para C:**
   - Módulo responsável por converter arquivos `.c` ou trechos de texto em c em páginas HTML com destaque de sintaxe (*syntax highlighting*).
   - Cores estruturadas baseadas nos padrões da Wikipedia para os seguintes itens léxicos:
     - Directivas e Pragmas (`#include`, `#define`, etc.)
     - Bibliotecas (`<stdio.h>`,`<stdlib.h>`, etc.)
     - Tipos de Dados / Variáveis (`int`, `float`, `char`, etc.)
     - Estruturas de Controle e Comandos (`if`, `for`, `while`, etc.)
     - Comentários de linha e de bloco (`//` e `/* ... */`)
     - Cadeias de Caracteres / Strings (`"texto\n"`)
     - Números (`1234`)
     - Declarações de função (`main`)
  - Padrão Wikipedia: https://en.wikipedia.org/wiki/Syntax_highlighting

---

## 📝 Formato dos Arquivos de Entrada (`.txt`)

Cada arquivo `.txt` de entrada representa um conceito e possui marcadores no início. Marcadores vazios são ignorados pelo sistema.

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
├── docs/                   # Documentações exigidas para entrega
│   ├── contribuicoes.md    # Relatório de contribuição de cada membro
│   ├── dialogos_ia.md      # Registro das interações com mecanismos de IA
│   ├── dificuldades_ia.md  # Análise de dificuldades, prós e contras das IAs
│   └── compilacao.md       # Instruções de compilação
├── src/                    # Código-fonte C do gerador e analisador léxico
    ├── html_generator.c    # Código do gerador html
    └── highlighter.c       # Código do highlighter c
├── txt_inputs/             # Arquivos .txt contendo o material didático (Avaliação 1)
└── README.md               # Este arquivo
```

---

## 🛠️ Como Compilar e Executar

Para instruções sobre a compilação consulte o arquivo [`docs/compilacao.md`](docs/compilacao.md).

---

## 📦 Entregáveis do Projeto

Conforme as especificações do EP1, os seguintes itens compõem a entrega oficial:

- [x] **Identificação do Grupo:** NUSP e nomes dos 4 integrantes.
- [x] **Relatório de Contribuição:** Descrição breve do papel de cada membro (1 página).
- [x] **Diálogos com IAs:** Histórico do processo interativo com ferramentas de Inteligência Artificial.
- [x] **Dificuldades com as IAs:** Relatório comparativo destacando facilidades, dificuldades e ranking das IAs utilizadas.
- [x] **Instruções de Compilação:** Guia passo a passo de build dos códigos C.
- [x] **Pacote de Fontes TXT:** Arquivo compactado contendo todo o conteúdo processado da Avaliação 1.
- [x] **Pacote de Fontes C:** Arquivo compactado com todos os algoritmos funcionais em C.

---

## 👥 Autores e Integrantes do Grupo

| Nome | NUSP |
| :--- | :--- |
| André Tamay Honda       | 17878515 |
| Alexandre Takaki Shiota | 17862744 |
| Davi Gaioso Manso       | 17862852 |
| Fernando Nemec Vieira   | 17889820 |

---
*Projeto desenvolvido para a disciplina MAC0122 - Instituto de Matemática e Estatística da Universidade de São Paulo (IME-USP).*
