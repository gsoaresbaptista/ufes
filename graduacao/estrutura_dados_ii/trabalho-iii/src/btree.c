#include "btree.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


typedef struct no No;
typedef struct tupla Tupla;

/* Estrutura responsavel por guardar as infomacoes dos dados,
   chave e valor, o union e utilizado para representar ambos nos
   (intermediario e folha). */
struct tupla {
    void* chave;
    union {
        void* item;
        No* filho;
    } ref;
};

/* No, contendo um inteiro indicando a quantidade de elementos
   registrados e um vetor de tupla */
struct no {
    int tamanho;
    Tupla* tupla;
};

/* Parametros da arvore, sendo eles:
   - raiz, ponteiro para a raiz da arvore;
   - ordem, ordem da arvore(quantidade de filhos);
   - altura, altura da arvore;
   - alturaTmp, altura auxiliar, utilizada em algumas operacoes;
   - cmp, funcao de comparacao utilizada pelos dados;
   */
struct btree {
    No* raiz;
    int ordem;
    int altura;
    int alturaTmp; // Inteiro auxilar para operacao de insercao
    void (*lc)(void*); // Liberar chave
    void (*li)(void*); // Liberar item
    int (*cmp)(void*, void*);
};

No* no_novo(int ordem) {
    No* no = (No*)malloc(sizeof(No));
    no->tupla = (Tupla*)malloc(sizeof(Tupla) * ordem);
    no->tamanho = 0;
    return no;
}

Btree* btree_nova(int ordem, int (*cmp)(void*, void*),
                  void (*lc)(void*), void (*li)(void*)) {
    Btree* arvore = (Btree*)malloc(sizeof(Btree));
    arvore->raiz = no_novo(ordem);
    arvore->altura = 0;
    arvore->ordem = ordem;
    arvore->cmp = cmp;
    arvore->li = li;
    arvore->lc = lc;

    return arvore;
}

void no_percorrer(No* no, void (*cb)(void*, void*), void* aux, int altura) {
    // Caso seja um no intermediario percorre os filhos
    if (altura)
        for (int i = 0; i < no->tamanho; ++i)
            no_percorrer(no->tupla[i].ref.filho, cb, aux, altura - 1);
    // Caso no folha aplica a funcao de callback nos elementos
    else
        for (int i = 0; i < no->tamanho; ++i)
            cb(no->tupla[i].ref.item, aux);
}

void btree_percorrer(Btree* arvore, void (*cb)(void*, void*), void* aux) {
    no_percorrer(arvore->raiz, cb, aux, arvore->altura);
}

void* no_buscar(No* no, void* chave, Btree* bt) {
    // Busca a chave caso seja no folha
    if (bt->alturaTmp == 0) {
        for (int j = 0; j < no->tamanho; ++j)
            if (bt->cmp(chave, no->tupla[j].chave) == 0)
                return no->tupla[j].ref.item;
    }
    else {
        --bt->alturaTmp;
        // Busca qual filho deve seguir
        for (int j = 0; j < no->tamanho; ++j)
            if ((j + 1 == no->tamanho)
                || bt->cmp(chave, no->tupla[j + 1].chave) < 0)
                // Retorna recursivamente ate chegar em um no folha
                return no_buscar(no->tupla[j].ref.filho, chave, bt);
    }
    // Retorna NULL caso nao encontrar a chave no no folha
    return NULL;
}

void* btree_buscar(Btree* arvore, void* chave) {
    arvore->alturaTmp = arvore->altura;
    return no_buscar(arvore->raiz, chave, arvore);
}

void no_atualizar(No* no, void* chave, void* item, Btree* bt) {
    // Busca a chave caso seja no folha
    if (bt->alturaTmp == 0) {
        for (int j = 0; j < no->tamanho; ++j)
            if (bt->cmp(chave, no->tupla[j].chave) == 0) {
                bt->li(no->tupla[j].ref.item);
                no->tupla[j].ref.item = item;
            }
                
    }
    else {
        --bt->alturaTmp;
        // Busca qual filho deve seguir
        for (int j = 0; j < no->tamanho; ++j)
            if ((j + 1 == no->tamanho)
                || bt->cmp(chave, no->tupla[j + 1].chave) < 0)
                // Retorna recursivamente ate chegar em um no folha
                return no_atualizar(no->tupla[j].ref.filho, chave, item, bt);
    }
}


void btree_atualizar(Btree* arvore, void* chave, void* item) {
    arvore->alturaTmp = arvore->altura;
    no_atualizar(arvore->raiz, chave, item, arvore);
}

No* no_separar(No* h, int ordem) {
    No* t = no_novo(ordem);
    int i = (ordem % 2) ? 1:0;

    // Copia metade dos elementos para o novo no
    for (int j = 0; j < ordem - ordem/2; ++j)
        t->tupla[j] = h->tupla[ordem - ordem/2 + j - i];
    
    // Atualiza o tamanho dos nos
    t->tamanho = ordem - ordem/2;
    h->tamanho = ordem/2;

    return t;
}

No* no_inserir(No* no, Tupla* tupla, Btree* bt) {
    int j;
    // Caso seja no folha encontra a posicao de insercao
    if (bt->alturaTmp == 0) {
        for (j = 0; j < no->tamanho; ++j)
            if (bt->cmp(tupla->chave, no->tupla[j].chave) < 0)
                break;
    }
    else {
        /* Caso nao seja no folha encontra o filho que tem que seguir,
           repetindo o processo ate encontrar um no folha, deixando um
           "rastro" com a referencia(filhos) */
        for (j = 0; j < no->tamanho; ++j)
            if ((j + 1 == no->tamanho) ||
                 bt->cmp(tupla->chave, no->tupla[j + 1].chave) < 0) {
                // Atualiza a altura e o no intermediario (referencia)
                --bt->alturaTmp;
                No* tmp1 = no->tupla[j++].ref.filho;
                No* tmp2 = no_inserir(tmp1, tupla, bt);
                // Nao foi necessario aumentar a altura da arvore
                if (tmp2 == NULL) return NULL;
                tupla->chave = tmp2->tupla[0].chave;
                tupla->ref.filho = tmp2;
                break;
            }
    }
    // "Empurra" as tuplas e insere a nova
    for (int i = (no->tamanho)++; i > j; --i)
        no->tupla[i] = no->tupla[i-1];
        
    no->tupla[j] = *tupla;

    // Retorna o no resultado da insercao
    if (no->tamanho < bt->ordem)
        return NULL;
    else
        return no_separar(no, bt->ordem);
}

void btree_inserir(Btree* arvore, void* chave, void* item) {
    // Insere a tupla na arvore
    arvore->alturaTmp = arvore->altura;
    Tupla tupla;
    tupla.chave = chave; tupla.ref.item = item;
    No* novo = no_inserir(arvore->raiz, &tupla, arvore);
    
    // Caso nao tenha mudado a estrutura, retorna
    if (novo == NULL) return;
    
    /* Caso contrario, cria-se uma nova raiz contendo
       o retorno e a raiz antiga */
    No* raiz = no_novo(arvore->ordem);
    raiz->tamanho = 2;
    raiz->tupla[0].chave = arvore->raiz->tupla[0].chave;
    raiz->tupla[0].ref.filho = arvore->raiz;
    raiz->tupla[1].chave = novo->tupla[0].chave;
    raiz->tupla[1].ref.filho = novo;
    arvore->raiz = raiz;
    ++arvore->altura;
}

int btree_altura(Btree* arvore) {
    return arvore->altura;
}

void no_liberar(No* no, void (*lc)(void*), void (*li)(void*), int altura) {
    if (altura != 0) {
        // Libera os nos filhos
        for (int i = 0; i < no->tamanho; ++i)
            no_liberar(no->tupla[i].ref.filho, lc, li, altura - 1);
    } else {
        // Aplica a funcao de callback para liberar os dados
        for (int i = 0; i < no->tamanho; ++i) {
            lc(no->tupla[i].chave);
            li(no->tupla[i].ref.item);
        }
    }
    // Libera a struct do no em questao
    free(no->tupla);
    free(no);
}

void btree_liberar(Btree* arvore) {
    no_liberar(arvore->raiz, arvore->lc, arvore->li, arvore->altura);
    free(arvore);
}