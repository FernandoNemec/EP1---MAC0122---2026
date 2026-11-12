#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINHA 8192
#define MAX_PAGINAS 1000

typedef struct {
    char arquivo_origem[256];
    char arquivo_destino_html[256];
    char tecnica[256];
    char algoritmo[256];
    char autores[256];
    char data[128];
    char conceitos[1024];
    char problema[4096];
} PaginaDados;

PaginaDados banco_de_paginas[MAX_PAGINAS];
int total_paginas = 0;

/* ==========================================================================
 * FUNÇÕES UTILITÁRIAS
 * ========================================================================== */
void remover_espacos_extras(char *str) {
    if (!str) return;
    char *p = str;
    int l = strlen(p);
    // Remove espaços, \n e \r do final (resolve o problema do Windows/Linux)
    while (l > 0 && isspace((unsigned char)p[l - 1])) p[--l] = '\0';
    // Avança espaços do início
    while (*p && isspace((unsigned char)*p)) ++p;
    memmove(str, p, l + 1);
}

char* obter_campo(int indice_pagina, int tipo_campo) {
    switch(tipo_campo) {
        case 1: return banco_de_paginas[indice_pagina].tecnica;
        case 2: return banco_de_paginas[indice_pagina].algoritmo;
        case 3: return banco_de_paginas[indice_pagina].autores;
        case 4: return banco_de_paginas[indice_pagina].data;
        case 5: return banco_de_paginas[indice_pagina].conceitos;
        case 6: return banco_de_paginas[indice_pagina].problema;
    }
    return NULL;
}

/* ==========================================================================
 * PARSER DO ARQUIVO TXT
 * ========================================================================== */
void processar_arquivo_txt(const char *nome_arquivo) {
    FILE *file = fopen(nome_arquivo, "r");
    if (!file) return;

    PaginaDados p;
    memset(&p, 0, sizeof(PaginaDados));
    strcpy(p.arquivo_origem, nome_arquivo);
    snprintf(p.arquivo_destino_html, sizeof(p.arquivo_destino_html), "%s.html", nome_arquivo);

    char linha[MAX_LINHA];
    while (fgets(linha, MAX_LINHA, file)) {
        char *pos;
        // Procura pelos marcadores flexíveis
        if ((pos = strstr(linha, "Tecnica:")) != NULL || (pos = strstr(linha, "Técnica:")) != NULL) {
            strcpy(p.tecnica, pos + 8);
        } else if ((pos = strstr(linha, "Algoritmo:")) != NULL) {
            strcpy(p.algoritmo, pos + 10);
        } else if ((pos = strstr(linha, "Autores:")) != NULL) {
            strcpy(p.autores, pos + 8);
        } else if ((pos = strstr(linha, "Data:")) != NULL) {
            strcpy(p.data, pos + 5);
        } else if ((pos = strstr(linha, "Conceitos:")) != NULL) {
            strcpy(p.conceitos, pos + 10);
        } else if ((pos = strstr(linha, "Problema:")) != NULL) {
            strcpy(p.problema, pos + 9);
        }
    }

    remover_espacos_extras(p.tecnica);
    remover_espacos_extras(p.algoritmo);
    remover_espacos_extras(p.autores);
    remover_espacos_extras(p.data);
    remover_espacos_extras(p.conceitos);
    remover_espacos_extras(p.problema);

    banco_de_paginas[total_paginas++] = p;
    fclose(file);
}

/* ==========================================================================
 * GERAÇÃO DOS ÍNDICES E SUBPÁGINAS
 * ========================================================================== */
void gerar_index_principal() {
    FILE *f = fopen("index.html", "w");
    if (!f) return;
    fprintf(f, "<!DOCTYPE html>\n<html lang=\"pt\">\n<head><meta charset=\"UTF-8\"><title>Índice MAC122</title></head>\n<body>\n");
    fprintf(f, "<h1>Índice de Conteúdo MAC122</h1>\n<ul>\n");
    fprintf(f, "<li><a href=\"idx_tecnica.html\">1. Técnicas</a></li>\n");
    fprintf(f, "<li><a href=\"idx_algoritmo.html\">2. Algoritmos</a></li>\n");
    fprintf(f, "<li><a href=\"idx_autores.html\">3. Autores</a></li>\n");
    fprintf(f, "<li><a href=\"idx_data.html\">4. Datas</a></li>\n");
    fprintf(f, "<li><a href=\"idx_conceitos.html\">5. Conceitos</a></li>\n");
    fprintf(f, "<li><a href=\"idx_problema.html\">6. Problemas</a></li>\n");
    fprintf(f, "</ul>\n</body>\n</html>\n");
    fclose(f);
}

void gerar_pagina_indexadora_com_subpaginas(const char* nome_arquivo, const char* titulo, int tipo_campo, const char* prefixo) {
    FILE *f = fopen(nome_arquivo, "w");
    if (!f) return;

    fprintf(f, "<!DOCTYPE html>\n<html><head><meta charset=\"UTF-8\"><title>%s</title></head><body>\n", titulo);
    fprintf(f, "<h1>%s</h1>\n<ul>\n", titulo);
    
    int processados[MAX_PAGINAS] = {0}; 
    int contador_categorias = 0;
    
    for (int i = 0; i < total_paginas; i++) {
        if (processados[i]) continue; 
        
        char *valor_atual = obter_campo(i, tipo_campo);
        if (valor_atual == NULL || strlen(valor_atual) == 0) continue;
        
        contador_categorias++;
        char nome_subpagina[256];
        snprintf(nome_subpagina, sizeof(nome_subpagina), "%s_%d.html", prefixo, contador_categorias);
        
        // Link na página índice principal (ex: Método guloso aponta para a subpágina)
        fprintf(f, "<li><a href=\"%s\">%s</a></li>\n", nome_subpagina, valor_atual);
        
        // Cria a subpágina para esta categoria específica
        FILE *f_sub = fopen(nome_subpagina, "w");
        fprintf(f_sub, "<!DOCTYPE html>\n<html><head><meta charset=\"UTF-8\"><title>%s</title></head><body>\n", valor_atual);
        fprintf(f_sub, "<h1>Categoria: %s</h1>\n<ul>\n", valor_atual);
        
        // Busca todos os TXTs que pertencem a esta mesma categoria
        for (int j = i; j < total_paginas; j++) {
            char *valor_comparacao = obter_campo(j, tipo_campo);
            
            if (valor_comparacao && strcmp(valor_atual, valor_comparacao) == 0) {
                processados[j] = 1; 
                char *texto_link = strlen(banco_de_paginas[j].algoritmo) > 0 ? banco_de_paginas[j].algoritmo : banco_de_paginas[j].arquivo_origem;
                // Link para o detalhe do algoritmo
                fprintf(f_sub, "<li><a href=\"%s\">%s</a></li>\n", banco_de_paginas[j].arquivo_destino_html, texto_link);
            }
        }
        
        fprintf(f_sub, "</ul>\n<br><a href=\"%s\">Voltar para %s</a>\n</body></html>\n", nome_arquivo, titulo);
        fclose(f_sub);
    }
    
    fprintf(f, "</ul>\n<br><a href=\"index.html\">Voltar ao Índice Principal</a>\n</body></html>\n");
    fclose(f);
}

/* Nova função exclusiva para lidar com itens separados por vírgula (Conceitos) */
void gerar_pagina_indexadora_conceitos(const char* nome_arquivo, const char* titulo, const char* prefixo) {
    FILE *f = fopen(nome_arquivo, "w");
    if (!f) return;

    fprintf(f, "<!DOCTYPE html>\n<html><head><meta charset=\"UTF-8\"><title>%s</title></head><body>\n", titulo);
    fprintf(f, "<h1>%s</h1>\n<ul>\n", titulo);

    char conceitos_unicos[1000][128]; // Vetor para armazenar os conceitos sem repetição
    int total_unicos = 0;

    // Passo 1: Varrer todas as páginas, separar por vírgulas e encontrar todos os conceitos únicos
    for (int i = 0; i < total_paginas; i++) {
        char temp[1024];
        strcpy(temp, banco_de_paginas[i].conceitos); // Cópia para não alterar o dado original
        
        char *token = strtok(temp, ","); // Corta na primeira vírgula
        
        while (token != NULL) {
            char conceito_limpo[128];
            strcpy(conceito_limpo, token);
            remover_espacos_extras(conceito_limpo); // Remove espaços antes e depois
            
            if (strlen(conceito_limpo) > 0) {
                int existe = 0;
                // Verifica se este conceito já foi adicionado à lista de únicos
                for (int c = 0; c < total_unicos; c++) {
                    if (strcmp(conceitos_unicos[c], conceito_limpo) == 0) {
                        existe = 1;
                        break;
                    }
                }
                // Se não existe, adiciona
                if (!existe && total_unicos < 1000) {
                    strcpy(conceitos_unicos[total_unicos++], conceito_limpo);
                }
            }
            token = strtok(NULL, ","); // Vai para a próxima vírgula
        }
    }

    // Passo 2: Criar uma subpágina para cada conceito único encontrado
    for (int c = 0; c < total_unicos; c++) {
        char nome_subpagina[256];
        snprintf(nome_subpagina, sizeof(nome_subpagina), "%s_%d.html", prefixo, c + 1);
        
        // Adiciona o link do conceito na página de índice geral (idx_conceitos.html)
        fprintf(f, "<li><a href=\"%s\">%s</a></li>\n", nome_subpagina, conceitos_unicos[c]);
        
        // Cria a subpágina (ex: cat_conceito_1.html)
        FILE *f_sub = fopen(nome_subpagina, "w");
        fprintf(f_sub, "<!DOCTYPE html>\n<html><head><meta charset=\"UTF-8\"><title>%s</title></head><body>\n", conceitos_unicos[c]);
        fprintf(f_sub, "<h1>Conceito: %s</h1>\n<ul>\n", conceitos_unicos[c]);
        
        // Varrer de novo os algoritmos para ver quais têm este conceito
        for (int i = 0; i < total_paginas; i++) {
            char temp[1024];
            strcpy(temp, banco_de_paginas[i].conceitos);
            int possui_conceito = 0;
            
            char *token = strtok(temp, ",");
            while (token != NULL) {
                char conceito_limpo[128];
                strcpy(conceito_limpo, token);
                remover_espacos_extras(conceito_limpo);
                
                if (strcmp(conceito_limpo, conceitos_unicos[c]) == 0) {
                    possui_conceito = 1; // Encontrou o conceito neste arquivo!
                    break;
                }
                token = strtok(NULL, ",");
            }
            
            // Se o arquivo contém o conceito, coloca o link do algoritmo nesta subpágina
            if (possui_conceito) {
                char *texto_link = strlen(banco_de_paginas[i].algoritmo) > 0 ? banco_de_paginas[i].algoritmo : banco_de_paginas[i].arquivo_origem;
                fprintf(f_sub, "<li><a href=\"%s\">%s</a></li>\n", banco_de_paginas[i].arquivo_destino_html, texto_link);
            }
        }
        
        fprintf(f_sub, "</ul>\n<br><a href=\"%s\">Voltar para %s</a>\n</body></html>\n", nome_arquivo, titulo);
        fclose(f_sub);
    }

    fprintf(f, "</ul>\n<br><a href=\"index.html\">Voltar ao Índice Principal</a>\n</body></html>\n");
    fclose(f);
}


/* ==========================================================================
 * GERAÇÃO DA PÁGINA DE CONTEÚDO 
 * ========================================================================== */
void gerar_pagina_conteudo(PaginaDados *pagina) {
    FILE *html = fopen(pagina->arquivo_destino_html, "w");
    FILE *txt = fopen(pagina->arquivo_origem, "r");
    
    fprintf(html, "<!DOCTYPE html>\n<html><head><meta charset=\"UTF-8\"><title>%s</title></head><body>\n", pagina->algoritmo);
    fprintf(html, "<h2>Detalhes:</h2><ul>");
    if(strlen(pagina->tecnica) > 0) fprintf(html, "<li><b>Técnica:</b> %s</li>", pagina->tecnica);
    if(strlen(pagina->algoritmo) > 0) fprintf(html, "<li><b>Algoritmo:</b> %s</li>", pagina->algoritmo);
    if(strlen(pagina->autores) > 0) fprintf(html, "<li><b>Autores:</b> %s</li>", pagina->autores);
    if(strlen(pagina->data) > 0) fprintf(html, "<li><b>Data:</b> %s</li>", pagina->data);
    fprintf(html, "</ul><hr>\n<p>");

    char linha[MAX_LINHA];
    while (fgets(linha, MAX_LINHA, txt)) {
        // Ignora as linhas de cabeçalho na impressão do texto principal
        if (strstr(linha, "Tecnica:") || strstr(linha, "Técnica:") || strstr(linha, "Algoritmo:") ||
            strstr(linha, "Autores:") || strstr(linha, "Data:") || strstr(linha, "Conceitos:") || strstr(linha, "Problema:")) {
            continue;
        }
        fprintf(html, "%s<br>", linha);
    }
    fprintf(html, "</p>\n<br><a href=\"index.html\">Voltar ao Índice</a>\n</body></html>\n");
    fclose(html);
    fclose(txt);
}

/* ==========================================================================
 * FUNÇÃO PRINCIPAL
 * ========================================================================== */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo1.txt> <arquivo2.txt> ...\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        processar_arquivo_txt(argv[i]);
    }

gerar_index_principal();
    
    // Usa a função normal de agrupamento exato
    gerar_pagina_indexadora_com_subpaginas("idx_tecnica.html", "Índice de Técnicas", 1, "cat_tecnica");
    gerar_pagina_indexadora_com_subpaginas("idx_algoritmo.html", "Índice de Algoritmos", 2, "cat_algo");
    gerar_pagina_indexadora_com_subpaginas("idx_autores.html", "Índice de Autores", 3, "cat_autor");
    gerar_pagina_indexadora_com_subpaginas("idx_data.html", "Índice de Datas", 4, "cat_data");
    
    // *** USA A NOVA FUNÇÃO PARA SEPARAR OS CONCEITOS POR VÍRGULAS ***
    gerar_pagina_indexadora_conceitos("idx_conceitos.html", "Índice de Conceitos", "cat_conceito");
    
    // Volta a usar a normal para os problemas
    gerar_pagina_indexadora_com_subpaginas("idx_problema.html", "Índice de Problemas", 6, "cat_problema");

    
    for (int i = 0; i < total_paginas; i++) {
        gerar_pagina_conteudo(&banco_de_paginas[i]);
    }

    printf("Páginas HTML geradas com sucesso!\n");
    return 0;
}