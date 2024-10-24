#include "tabela.h"
#include "dados.h"
#include "btree.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX 4097

enum comandos {
    get,
    delete,
    insert,
    rundown,
    stop
};

struct tabela {
    Btree* arvore;
    FILE* arquivo;
    FILE* saida;
    int posicaoChave;
    long int posicao;
    unsigned char comando[MAX];
};

/* Funcao utilizada na comparacao de chaves pela
   arvore b */
int compararChaves (void* a, void* b) {
    unsigned char* x = (unsigned char*)a;
    unsigned char* y = (unsigned char*)b;
    return strcmp(x, y);
}

Tabela* tabela_nova(char* arquivo, int ordem) {
    Tabela* tabela = (Tabela*)malloc(sizeof(Tabela));
    tabela->arvore = btree_nova(ordem, compararChaves, free, free);
    tabela->arquivo = fopen(arquivo, "rb+");

    if (tabela->arquivo == NULL) {
        // Caso o arquivo nao exista, cria um novo
        tabela->arquivo = fopen(arquivo, "wb+");
    }
    else {
        // Indexa o arquivo preenchedo a btree
        dados_indexar(tabela->arquivo, tabela->arvore);
    }

    tabela->posicao = ftell(tabela->arquivo);

    return tabela;
}

// Retorna o comando pela string informada
static int comando_retornar(char* comando) {
    int comp = strcmp(comando, "GET");
    if (comp == 0) return get;
    comp = strcmp(comando, "DELETE");
    if (comp == 0) return delete;
    comp = strcmp(comando, "INSERT");
    if (comp == 0) return insert;
    comp = strcmp(comando, "RUNDOWN");
    if (comp == 0) return rundown;
    comp = strcmp(comando, "STOP");
    if (comp == 0) return stop;
    return -1;
}

/* Separa a porcao de dados em comando dentro de tabela,
   note que posicaoChave e a posicao do caractere separador
   (virgula). */
void comando_separar(Tabela* tabela) {
    // Encontra a posicao do separador(virgula)
    tabela->posicaoChave = 5;

    while (true)
        if (tabela->comando[tabela->posicaoChave++] == ',')
            break;

    // Fecha a string e imprime o resultado
    tabela->comando[tabela->posicaoChave - 1] = '\0';
}

/* Atualiza tabela->comando com a informacao do registro da chave
   salva nele, ou seja, pega a chave de tabela->comando e atualiza
   a string tabela->comando com as informacoes do registro. */
bool comando_atualizar(Tabela* tabela) {
    long int* id = btree_buscar(tabela->arvore, tabela->comando);

    // Existe identificador de cliente
    if ( id != NULL) {
        tabela->posicao = *id - tabela->posicao;
        bool ativo = dados_get(tabela->arquivo, tabela->comando,
                               &tabela->posicao);
        // Atualiza a posicao do arquivo
        tabela->posicao = ftell(tabela->arquivo);
        return ativo;
    }

    return false; // Nao encontrado
}

/* Funcao utilizada para imprimir os nos da btree que guarda
   as informacoes, como a btree esta sendo percorrida em 
   pre-ordem a saida sera ordenada pela funcao de comparacao
   da btree (compararChaves)*/
void imprimir_no(void* item, void* tabela) {
    Tabela* tab = (Tabela*)tabela;

    // Atualiza a posicao
    tab->posicao = *((long int*)item) - tab->posicao;

    if (dados_get(tab->arquivo, tab->comando, &tab->posicao)) {
        comando_separar(tab);
        // Imprime no arquivo
        fprintf(tab->saida, "%s: %s\n", tab->comando + 5,
                tab->comando + tab->posicaoChave);
    }

    tab->posicao = ftell(tab->arquivo);
}

void comando_rundown(Tabela* tabela) {
    scanf("%s", tabela->comando);
    tabela->saida = fopen(tabela->comando, "wb");
    btree_percorrer(tabela->arvore, imprimir_no, tabela);
    fclose(tabela->saida);
}

void comando_desativarDado(Tabela* tabela) {
    long int* id = btree_buscar(tabela->arvore, tabela->comando);

    // Existe identificador de cliente
    if ( id != NULL) {
        tabela->posicao = *id - ftell(tabela->arquivo);

        if (dados_ativo(tabela->arquivo, &tabela->posicao)) {
            bool ativo = false;
            tabela->posicao = *id - ftell(tabela->arquivo);
            tabela->posicao += (tabela->posicao < 0) ? 4 : -4;
            dados_set(tabela->arquivo, &ativo, &tabela->posicao);
        }

        // Atualiza a posicao do arquivo
        tabela->posicao = ftell(tabela->arquivo);
    }
}

void comando_delete(Tabela* tabela) {
    scanf("%s", tabela->comando);
    comando_desativarDado(tabela);
}

void comando_get(Tabela* tabela) {  
    scanf("%s", tabela->comando);
    bool ativo = comando_atualizar(tabela);

    if (ativo) {
        comando_separar(tabela);
        printf("%s: %s\n", tabela->comando + 5,
               tabela->comando + tabela->posicaoChave);
    }
}

void inserir_novoRegistro(Tabela* tabela, int separador) {
    int tamanho = dados_inserir(tabela->arquivo, tabela->comando);
    tabela->comando[separador - 1] = '\0';

    // Instancia os dados
    unsigned char* chave = malloc(sizeof(unsigned char) * separador);
    long int* item = (long int*)malloc(sizeof(long int));
    memcpy(chave, tabela->comando, separador);
    *item = ftell(tabela->arquivo) - tamanho - 5;
    
    // Insere na arvore
    btree_inserir(tabela->arvore, chave, item);
    tabela->posicao = ftell(tabela->arquivo);
}

void inserir_atualizarRegistro(Tabela* tabela, int separador) {
    int tamanho = dados_inserir(tabela->arquivo, tabela->comando);
    tabela->comando[separador - 1] = '\0';
    long int* item = (long int*)malloc(sizeof(long int));
    *item = ftell(tabela->arquivo) - tamanho - 5;
    btree_atualizar(tabela->arvore, tabela->comando, item);
    tabela->posicao = ftell(tabela->arquivo);
}

void comando_insert(Tabela* tabela) {
    scanf("%s", tabela->comando);
    long int* id = btree_buscar(tabela->arvore, tabela->comando);
    
    // Concatena os dados apos a virgula
    int separador = strlen(tabela->comando);
    tabela->comando[separador++] = ',';
    scanf("%s", tabela->comando + separador);

    if (id == NULL) {
        // Caso nao exista no arquivo
        inserir_novoRegistro(tabela, separador);
    }
    else {
        long int item = *id - tabela->posicao;

        if (!dados_ativo(tabela->arquivo, &item)) {
            // Existe no arquivo mas esta inativo
            inserir_atualizarRegistro(tabela, separador);
        }
        else {
            /* Existe no arquivo e esta ativo, primeiro deve-se
                desativar o registro antigo */
            tabela->comando[separador - 1] = '\0';
            comando_desativarDado(tabela);
            tabela->comando[separador - 1] = ',';
            // Atualiza a informacao
            inserir_atualizarRegistro(tabela, separador);
        }
    }
    // Escreve as pendencias no arquivo
    fflush(tabela->arquivo);  
}

void tabela_loop(Tabela* tabela) {
    int comando;
    char prompt[10];

    do {
        scanf("%s", prompt);
        comando = comando_retornar(prompt);

        switch (comando) {
            case get:
                comando_get(tabela);
                break;
            case delete:
                comando_delete(tabela);
                break;
            case insert:
                comando_insert(tabela);
                break;
            case rundown:
                comando_rundown(tabela);
                break;
            case stop:
                /* Comando atualizado, saira ao fim da iteracao */
                break;
            default:
                // printf("Erro: Comando nao encontrado!\n");
                break;
        }
    } while (comando != stop);
}

void tabela_liberar(Tabela* tabela) {
    btree_liberar(tabela->arvore);
    fclose(tabela->arquivo);
    free(tabela);
}