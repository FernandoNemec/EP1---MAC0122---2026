#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

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
    char complexidade[128];
    char semana[128];
} PaginaDados;

PaginaDados banco_de_paginas[MAX_PAGINAS];
int total_paginas = 0;

int todas_paginas_foram_geradas = 1;

/* ==========================================================================
 * FUNÇÕES UTILITÁRIAS E DE ORDENAÇÃO
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
        case 7: return banco_de_paginas[indice_pagina].complexidade;
        case 8: return banco_de_paginas[indice_pagina].semana;
    }
    return NULL;
}

// Função auxiliar para o qsort ordenar arrays de strings alfabeticamente
int comparar_strings(const void *a, const void *b) {

    char aux_a[512];
    char aux_b[512];

    strcpy(aux_a, a);
    strcpy(aux_b, b);

    // Mudança das primeiras letras para melhorar a organização em ordem alfabética

    if ( ((const char *)a)[0] > 96 && ((const char *)a)[0] < 123)           // Minúsculo para maiúsculo 
        aux_a[0] = ((const char *)a)[0] - 32;
    else if ( ((const char *)a)[0] == -61) {                                // É acentuado com ´
        if ( ((const char *)a)[1] == -127 || ((const char *)a)[1] == -95)   // Á ou á
            aux_a[0] = 'A';
        if ( ((const char *)a)[1] == -109 || ((const char *)a)[1] == -77)   // Ó ou ó
            aux_a[0] = 'O';
    }
    if ( ((const char *)b)[0] > 96 && ((const char *)b)[0] < 123)           // Minúsculo para maiúsculo 
        aux_b[0] = ((const char *)b)[0] - 32;
    else if ( ((const char *)b)[0] == -61) {                                // É acentuado com ´
        if ( ((const char *)b)[1] == -127 || ((const char *)b)[1] == -95)   // Á ou á
            aux_b[0] = 'A';
        if ( ((const char *)b)[1] == -109 || ((const char *)b)[1] == -77)   // Ó ou ó
            aux_b[0] = 'O';
    }

    return strcmp((const char *)aux_a, (const char *)aux_b);
}

// Função para garantir que todo nome de categoria comece com letra maiúscula (padronização)
void corretor_automatico(char *a) {
    if (a[0] > 96 && a[0] < 123)
        a[0] = a[0] - 32;
    else if (a[0] == -61) {
        if (a[1] == -95)
            a[1] = -127;
        if (a[1] == -109)
            a[1] = -77;
    }
}

// Função para garantir que duas categorias com apenas diferença de caixa sejam
// lidas como a mesma categoria, por exemplo: Tentativa e erro / Tentativa e Erro
void caixa_alta(char *a) {
    for(int i = 0; a[i] != '\0'; i++)
        if (a[i] > 96 && a[i] < 123)
            a[i] = a[i] - 32;
        else if (a[i] == -61) {
            if (a[++i] == -95)
                a[++i] = -127;
            if (a[++i] == -109)
                a[++i] = -77;
        }
}

/* ==========================================================================
 * PARSER DO ARQUIVO TXT
 * ========================================================================== */
void processar_arquivo_txt(const char *nome_arquivo) {

    char nome_arquivo_txt[512];
    snprintf(nome_arquivo_txt, sizeof(nome_arquivo_txt), "../txt_inputs/%s.txt", nome_arquivo);

    FILE *file = fopen(nome_arquivo_txt, "r");
    if (!file) return;

    PaginaDados p;
    memset(&p, 0, sizeof(PaginaDados));
    strcpy(p.arquivo_origem, nome_arquivo_txt);
    snprintf(p.arquivo_destino_html, sizeof(p.arquivo_destino_html), "html_outputs/%s.html", nome_arquivo);

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
        } else if ((pos = strstr(linha, "Complexidade:")) != NULL) {
            strcpy(p.complexidade, pos + 13);
        } else if ((pos = strstr(linha, "Semana:")) != NULL) {
            strcpy(p.semana, pos + 7);
        }
    }

    remover_espacos_extras(p.tecnica);
    remover_espacos_extras(p.algoritmo);
    remover_espacos_extras(p.autores);
    remover_espacos_extras(p.data);
    remover_espacos_extras(p.conceitos);
    remover_espacos_extras(p.problema);
    remover_espacos_extras(p.complexidade);
    remover_espacos_extras(p.semana);

    banco_de_paginas[total_paginas++] = p;
    fclose(file);
}


/* ==========================================================================
 * FUNÇÃO DE CABEÇALHO E CSS
 * ========================================================================== */
void imprimir_cabecalho_html(FILE *f, const char *titulo) {
    fprintf(f, "<!DOCTYPE html>\n<html lang=\"pt\">\n<head>\n");
    fprintf(f, "<meta charset=\"UTF-8\">\n<title>%s</title>\n", titulo);
    fprintf(f, "<style>\n");
    fprintf(f, "  body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; max-width: 900px; margin: 40px auto; padding: 20px; background-color: #f8f9fa; color: #333; line-height: 1.6; }\n");
    fprintf(f, "  h1, h2, h3 { color: #2c3e50; border-bottom: 2px solid #3498db; padding-bottom: 5px; }\n");
    fprintf(f, "  ul { list-style-type: square; padding-left: 20px; }\n");
    fprintf(f, "  li { margin-bottom: 10px; }\n");
    fprintf(f, "  a { color: #3498db; text-decoration: none; font-weight: 500; transition: color 0.3s; }\n");
    fprintf(f, "  a:hover { color: #e74c3c; text-decoration: underline; }\n");
    fprintf(f, "  hr { border: 0; height: 1px; background: #ddd; margin: 20px 0; }\n");
    fprintf(f, "</style>\n");
    fprintf(f, "</head>\n<body>\n");
}


/* ==========================================================================
 * GERAÇÃO DOS ÍNDICES E SUBPÁGINAS
 * ========================================================================== */
void gerar_index_principal() {
    FILE *f = fopen("index.html", "w");
    if (!f) return;

    // Chama a estilização
    imprimir_cabecalho_html(f, "Índice MAC122");

    fprintf(f, "<h1>Índice de Conteúdo MAC122</h1>\n<ul>\n");
    fprintf(f, "<li><a href=\".idx/idx_tecnica.html\">1. Técnicas</a></li>\n");
    fprintf(f, "<li><a href=\".idx/idx_algoritmo.html\">2. Algoritmos</a></li>\n");
    fprintf(f, "<li><a href=\".idx/idx_autores.html\">3. Autores</a></li>\n");
    fprintf(f, "<li><a href=\".idx/idx_data.html\">4. Datas</a></li>\n");
    fprintf(f, "<li><a href=\".idx/idx_conceitos.html\">5. Conceitos</a></li>\n");
    fprintf(f, "<li><a href=\".idx/idx_problema.html\">6. Problemas</a></li>\n");
    fprintf(f, "<li><a href=\".idx/idx_complexidade.html\">7. Complexidade</a></li>\n");
    fprintf(f, "<li><a href=\".idx/idx_semana.html\">8. Semanas</a></li>\n");
    fprintf(f, "</ul>\n</body>\n</html>\n");
    fclose(f);
}

void gerar_pagina_indexadora_com_subpaginas(const char* nome_arquivo, const char* titulo, int tipo_campo, const char* prefixo) {
    FILE *f = fopen(nome_arquivo, "w");
    if (!f) return;

    imprimir_cabecalho_html(f, titulo);
    fprintf(f, "<h1>%s</h1>\n<ul>\n", titulo);
    
    char categorias_unicas[MAX_PAGINAS][256];
    int total_categorias = 0;
    
    // Passo 1: Extrair todas as categorias únicas
    for (int i = 0; i < total_paginas; i++) {
        char *valor_atual = obter_campo(i, tipo_campo);
        if (valor_atual == NULL || strlen(valor_atual) == 0) continue;

        corretor_automatico(valor_atual);

        char aux1 [256];
        strcpy(aux1,valor_atual);
        caixa_alta(aux1);

        int existe = 0;
        for (int c = 0; c < total_categorias; c++) {

            char aux2 [256];
            strcpy(aux2,categorias_unicas[c]);
            caixa_alta(aux2);

            if (strcmp(aux2, aux1) == 0) {
                existe = 1;
                break;
            }
        }
        if (!existe) {
            if (total_categorias < MAX_PAGINAS)
                strcpy(categorias_unicas[total_categorias++], valor_atual);
            else {
                printf("Limite de páginas atingido\n");
                todas_paginas_foram_geradas = 0;
            }
        }
    }
    
    // Passo 2: Ordenar as categorias alfabeticamente usando qsort
    qsort(categorias_unicas, total_categorias, sizeof(categorias_unicas[0]), comparar_strings);
    
    // Passo 3: Gerar as subpáginas baseadas no array ordenado
    for (int c = 0; c < total_categorias; c++) {
        char nome_subpagina[256];
        snprintf(nome_subpagina, sizeof(nome_subpagina), "%s_%d.html", prefixo, c + 1);
        
        fprintf(f, "<li><a href=\"../%s\">%s</a></li>\n", nome_subpagina, categorias_unicas[c]);
        
        FILE *f_sub = fopen(nome_subpagina, "w");
        imprimir_cabecalho_html(f_sub, categorias_unicas[c]); 

        switch(tipo_campo){
        case 1:
            fprintf(f_sub, "<h1>Técnica: %s</h1>\n<ul>\n", categorias_unicas[c]);
            break;
        case 2:
            fprintf(f_sub, "<h1>Algoritmo: %s</h1>\n<ul>\n", categorias_unicas[c]);
            break;
        case 3:
            fprintf(f_sub, "<h1>Autor: %s</h1>\n<ul>\n", categorias_unicas[c]);
            break;
        case 4:
            fprintf(f_sub, "<h1>Data: %s</h1>\n<ul>\n", categorias_unicas[c]);
            break;
        case 5:
            fprintf(f_sub, "<h1>Conceito: %s</h1>\n<ul>\n", categorias_unicas[c]);
            break;
        case 6:
            fprintf(f_sub, "<h2>Problema: %s</h2>\n<ul>\n", categorias_unicas[c]);
            break;
        case 7:
            fprintf(f_sub, "<h1>Complexidade: %s</h1>\n<ul>\n", categorias_unicas[c]);
            break;
        case 8:
            fprintf(f_sub, "<h1>Semana: %s</h1>\n<ul>\n", categorias_unicas[c]);
            break;
        }
        
        for (int j = 0; j < total_paginas; j++) {
            char *valor_comparacao = obter_campo(j, tipo_campo);

            char aux1 [256];
            strcpy(aux1,valor_comparacao);
            caixa_alta(aux1);

            char aux2 [256];
            strcpy(aux2,categorias_unicas[c]);
            caixa_alta(aux2);

            if (valor_comparacao && strcmp(aux2, aux1) == 0) {
                char *texto_link = strlen(banco_de_paginas[j].algoritmo) > 0 ? banco_de_paginas[j].algoritmo : banco_de_paginas[j].arquivo_origem;
                fprintf(f_sub, "<li><a href=\"../%s\">%s</a></li>\n", banco_de_paginas[j].arquivo_destino_html, texto_link);
            }
        }
        fprintf(f_sub, "</ul>\n<br><a href=\"../%s\">Voltar para %s</a>\n</body></html>\n", nome_arquivo, titulo);
        fclose(f_sub);
    }
    
    fprintf(f, "</ul>\n<br><a href=\"../index.html\">Voltar ao Índice Principal</a>\n</body></html>\n");
    fclose(f);
}

/* Nova função exclusiva para lidar com itens separados por vírgula (Conceitos) */
void gerar_pagina_indexadora_conceitos(const char* nome_arquivo, const char* titulo, const char* prefixo) {
    FILE *f = fopen(nome_arquivo, "w");
    if (!f) return;

    imprimir_cabecalho_html(f, titulo);
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

            corretor_automatico(conceito_limpo);

            char aux1 [256];
            strcpy(aux1,conceito_limpo);
            caixa_alta(aux1);

            if (strlen(conceito_limpo) > 0) {
                int existe = 0;
                // Verifica se este conceito já foi adicionado à lista de únicos
                for (int c = 0; c < total_unicos; c++) {

                    char aux2 [256];
                    strcpy(aux2,conceitos_unicos[c]);
                    caixa_alta(aux2);

                    if (strcmp(aux2, aux1) == 0) {
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

    // Passo 2: Ordenar conceitos alfabeticamente usando qsort
    qsort(conceitos_unicos, total_unicos, sizeof(conceitos_unicos[0]), comparar_strings);

    // Passo 3: Criar uma subpágina para cada conceito único encontrado
    for (int c = 0; c < total_unicos; c++) {
        char nome_subpagina[256];
        snprintf(nome_subpagina, sizeof(nome_subpagina), "%s_%d.html", prefixo, c + 1);
        
        // Adiciona o link do conceito na página de índice geral (idx_conceitos.html)
        fprintf(f, "<li><a href=\"../%s\">%s</a></li>\n", nome_subpagina, conceitos_unicos[c]);
        
        // Cria a subpágina (ex: cat_conceito_1.html)
        FILE *f_sub = fopen(nome_subpagina, "w");
        imprimir_cabecalho_html(f_sub, conceitos_unicos[c]); // Aplica estilo na subpágina
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

                char aux1 [256];
                strcpy(aux1,conceito_limpo);
                caixa_alta(aux1);

                char aux2 [256];
                strcpy(aux2,conceitos_unicos[c]);
                caixa_alta(aux2);

                if (strcmp(aux1, aux2) == 0) {
                    possui_conceito = 1; // Encontrou o conceito neste arquivo!
                    break;
                }
                token = strtok(NULL, ",");
            }
            
            // Se o arquivo contém o conceito, coloca o link do algoritmo nesta subpágina
            if (possui_conceito) {
                char *texto_link = strlen(banco_de_paginas[i].algoritmo) > 0 ? banco_de_paginas[i].algoritmo : banco_de_paginas[i].arquivo_origem;
                fprintf(f_sub, "<li><a href=\"../%s\">%s</a></li>\n", banco_de_paginas[i].arquivo_destino_html, texto_link);
            }
        }
        
        fprintf(f_sub, "</ul>\n<br><a href=\"../%s\">Voltar para %s</a>\n</body></html>\n", nome_arquivo, titulo);
        fclose(f_sub);
    }

    fprintf(f, "</ul>\n<br><a href=\"../index.html\">Voltar ao Índice Principal</a>\n</body></html>\n");
    fclose(f);
}


/* ==========================================================================
 * GERAÇÃO DA PÁGINA DE CONTEÚDO 
 * ========================================================================== */
void gerar_pagina_conteudo(PaginaDados *pagina) {
    FILE *html = fopen(pagina->arquivo_destino_html, "w");
    FILE *txt = fopen(pagina->arquivo_origem, "r");
    
    imprimir_cabecalho_html(html, pagina->algoritmo); // Aplica estilo à página do algoritmo
 
    fprintf(html, "<h2>Detalhes:</h2><ul>");
    if(strlen(pagina->tecnica) > 0) fprintf(html, "<li><b>Técnica:</b> %s</li>", pagina->tecnica);
    if(strlen(pagina->algoritmo) > 0) fprintf(html, "<li><b>Algoritmo:</b> %s</li>", pagina->algoritmo);
    if(strlen(pagina->autores) > 0) fprintf(html, "<li><b>Autores:</b> %s</li>", pagina->autores);
    if(strlen(pagina->data) > 0) fprintf(html, "<li><b>Data:</b> %s</li>", pagina->data);
    if(strlen(pagina->complexidade) > 0) fprintf(html, "<li><b>Complexidade:</b> %s</li>", pagina->complexidade);
    if(strlen(pagina->semana) > 0) fprintf(html, "<li><b>Semana:</b> %s</li>", pagina->semana);
    fprintf(html, "</ul><hr>\n<p>");

    char linha[MAX_LINHA];
    while (fgets(linha, MAX_LINHA, txt)) {
        // Ignora as linhas de cabeçalho na impressão do texto principal
        if (strstr(linha, "Tecnica:") || strstr(linha, "Técnica:") || strstr(linha, "Algoritmo:") ||
            strstr(linha, "Autores:") || strstr(linha, "Data:") || strstr(linha, "Conceitos:") ||
            strstr(linha, "Problema:") || strstr(linha, "Complexidade:") || strstr(linha, "Semana:")) {
            continue;
        }
        fprintf(html, "%s<br>", linha);
    }
    fprintf(html, "</p>\n<br><a href=\"../index.html\">Voltar ao Índice</a>\n</body></html>\n");
    fclose(html);
    fclose(txt);
}


/* ==========================================================================
 * VERIFICAÇÃO DA EXISTÊNCIA DA BUILD
 * ========================================================================== */
int pasta_tem_arquivos(const char *caminho) {
    DIR *dir = opendir(caminho);
    if (dir == NULL) {
        return -1; // Retorna -1 se a pasta não existir
    }

    struct dirent *entrada;
    while ((entrada = readdir(dir)) != NULL) {
        // Ignora as entradas padrão "." e ".."
        if (strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0) {
            closedir(dir);
            return 1; // Encontrou pelo menos 1 arquivo ou subpasta!
        }
    }

    closedir(dir);
    return 0; // Pasta está completamente vazia
}


/* ==========================================================================
 * FUNÇÃO PRINCIPAL
 * ========================================================================== */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo1.txt> <arquivo2.txt> ...\n", argv[0]);
        return 1;
    }

    switch(pasta_tem_arquivos("build")) {
    case -1:
        mkdir("build", 0777);
        chdir("build");
        break;
    case 0:
        chdir("build");
        break;
    case 1:
        perror("Você já usou o gerador_html\nPara mudar os inputs e usá-lo de novo remova a build:\n rm -rf build\n");
        return 1;
    }

    // Gera pastas ocultas para organização do conteúdo compilado
    mkdir(".idx", 0777);
    mkdir(".cat", 0777);
    mkdir("html_outputs", 0777);

    for (int i = 1; i < argc; i++) {
        int l = strlen(argv[i]);
        char nome_arquivo[l];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "%s", argv[i]);
        l -= 4;
        nome_arquivo[l] = '\0';
        processar_arquivo_txt(nome_arquivo);
    }

    gerar_index_principal();
    
    // Usa a função normal de agrupamento exato
    gerar_pagina_indexadora_com_subpaginas(".idx/idx_tecnica.html", "Índice de Técnicas", 1, ".cat/cat_tecnica");
    gerar_pagina_indexadora_com_subpaginas(".idx/idx_algoritmo.html", "Índice de Algoritmos", 2, ".cat/cat_algo");
    gerar_pagina_indexadora_com_subpaginas(".idx/idx_autores.html", "Índice de Autores", 3, ".cat/cat_autor");
    gerar_pagina_indexadora_com_subpaginas(".idx/idx_data.html", "Índice de Datas", 4, ".cat/cat_data");
    gerar_pagina_indexadora_com_subpaginas(".idx/idx_complexidade.html", "Índice de Complexidades", 7, ".cat/cat_complexidade");
    gerar_pagina_indexadora_com_subpaginas(".idx/idx_semana.html", "Índice de Semanas", 8, ".cat/cat_semana");
    
    // *** USA A NOVA FUNÇÃO PARA SEPARAR OS CONCEITOS POR VÍRGULAS ***
    gerar_pagina_indexadora_conceitos(".idx/idx_conceitos.html", "Índice de Conceitos", ".cat/cat_conceito");
    
    // Volta a usar a normal para os problemas
    gerar_pagina_indexadora_com_subpaginas(".idx/idx_problema.html", "Índice de Problemas", 6, ".cat/cat_problema");

    
    for (int i = 0; i < total_paginas; i++) {
        gerar_pagina_conteudo(&banco_de_paginas[i]);
    }

    if(todas_paginas_foram_geradas)
        printf("Páginas HTML geradas com sucesso!\n");
    else
        printf("Nem todas as páginas foram geradas com sucesso\n");
    return 0;
}
