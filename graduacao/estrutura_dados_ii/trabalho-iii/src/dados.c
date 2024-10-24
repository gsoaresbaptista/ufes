#include "dados.h"
#include "btree.h"

#include <stdlib.h>
#include <string.h>

#define MAX 4097

/*  Estrutura utilizada para indexar arquivos. Note que o dados de
    indexador tem o funcionamento semelhante a um iterador, 
    atualizando o vetor para a informacao a ser consumida. Onde
    as variaveis representam respectivamente,
        - int ler: 0 ou 1, qual posicao que sera lida em dados;
        - int indice: Posicao de leitura no dado atual;
        - int faltam:  Quantidade de elementos que faltam ler em dados;
        - long int acumulado: Posicao acumulada, indice da btree;
        - unsigned short int n: Inteiro dos metadados;
        - unsigned char dados[2][MAX]: Vetor de string utilizado nas
          copias e leituras. */
typedef struct {
    Btree* arvore;
    FILE* arquivo;
    int ler;
    int indice;
    int faltam;
    long int acumulado;
    unsigned short int n;
    unsigned char dados[2][MAX];
} Indexador;

// Inicializa um novo indexador
Indexador* indexador_novo(FILE* arquivo, Btree* arvore) {
    Indexador* id = (Indexador*)malloc(sizeof(Indexador));
    id->arvore = arvore;
    id->arquivo = arquivo;
    id->indice = 0;
    id->acumulado = 0;
    id->ler = 0;
    id->faltam = 0;
    return id;
}

// Le os dadados do arquivo os carregando em dados[ler]
void lerArquivoConcatenando(Indexador* idx) {
    int i = idx->indice;
    int salvar = (idx->ler == 0) ? 1:0;
    memcpy(idx->dados[salvar], idx->dados[idx->ler] + i,
           idx->faltam);
    idx->faltam += fread(idx->dados[salvar] + idx->faltam, 1,
                         i, idx->arquivo);
    idx->acumulado += idx->indice;
    idx->indice = 0;
    idx->ler = salvar;
}

void lerArquivo(Indexador* idx) {
    if (idx->faltam == 0) {
        // Caso nao tenha dados sobrando
        idx->faltam = fread(idx->dados[idx->ler], 1,
                            MAX, idx->arquivo);
        memcpy(&idx->n, idx->dados[idx->ler], 2);
    }
    else if (idx->faltam >= 4) {
        // Caso de para ler o inteiro
        memcpy(&idx->n, idx->dados[idx->ler] + idx->indice, 2);
        if (idx->faltam - 5 < idx->n)
            lerArquivoConcatenando(idx);
    }
    else {
        // Caso nao de para ler o inteiro
        lerArquivoConcatenando(idx);
        memcpy(&idx->n, idx->dados[idx->ler], 2);
    }
}

void inserirDado(Indexador* idx) {
    int i;

    // Encontra o separador(virgula)
    for (i = 0; i < idx->n; i++)
        if (idx->dados[idx->ler][idx->indice + i + 5] == ',')
            break;

    // Copia as informacoes da chave e do item
    unsigned char* chave = malloc(sizeof(unsigned char) * (i + 1));
    memcpy(chave, idx->dados[idx->ler] + 5 + idx->indice, i);
    chave[i] = '\0'; // Fechando a string
    long int* item = malloc(sizeof(long int));
    *item = idx->indice + idx->acumulado;

    // Insere na btree
    btree_inserir(idx->arvore, chave, item);
}

void dados_indexar(FILE* arquivo, Btree* arvore) {
    // Instancia uma struct auxiliar para indexar
    Indexador* idx = indexador_novo(arquivo, arvore);

    // Le o arquivo e insere o indice na arvore
    do {
        lerArquivo(idx);
        inserirDado(idx);
        // Atualiza os indices
        idx->indice += idx->n + 5;
        idx->faltam -= idx->n + 5;
    } while (idx->faltam || !feof(arquivo));
    
    // Libera o indexador
    free(idx); 
    return arvore;
}

bool dados_get(FILE* arquivo, unsigned char* dado, long int* deslocamento) {    
    unsigned short int n;
    bool ativo;
    fseek(arquivo, *deslocamento, SEEK_CUR);

    fread(dado, 1, MAX, arquivo);
    memcpy(&n, dado, 2);
    memcpy(&ativo, dado + 4, 1);
    dado[n + 5] = '\0'; // Fecha a string

    return ativo;
}

void dados_set(FILE* arquivo, void* novo, long int* deslocamento) {
    fseek(arquivo, *deslocamento, SEEK_CUR);
    fwrite(novo, 1, 1, arquivo);
}

int dados_inserir(FILE* arquivo, unsigned char* conteudo) {
    int n = strlen(conteudo);
    // Aponta para o fim do arquivo
    fseek(arquivo, 0, SEEK_END);
    bool ativo = true;
    fwrite(&n, 4, 1, arquivo);
    fwrite(&ativo, 1, 1, arquivo);
    fwrite(conteudo, 1, n, arquivo);

    return n;
}

bool dados_ativo(FILE* arquivo, long int* deslocamento) {
    bool ativo;
    long int pos = *deslocamento;
    pos += (pos < 0) ? 4 : -4;

    fseek(arquivo, pos, SEEK_CUR);
    fread(&ativo, 1, 1, arquivo);
    
    return ativo;
}