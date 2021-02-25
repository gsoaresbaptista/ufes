#ifndef __ARVORE_B__
#define __ARVORE_B__

/*
 * A estrutura Btree guarda os dados referentes a uma arvore b.
 * A implementacao desta arvore e uma implementacao da arvore b
 * (dado apenas nos nos folhas, nos intermediarios apresentam
 * apenas referencias para os filhos).
 * Implementacao baseada no livro: Algorithms in C, 3rd ed.
 */
typedef struct btree Btree;

/*
* Instancia uma estrutura de arvore b.
*
* Inputs:
*   - int ordem: Ordem da arvore pela definicao de Knuth
*     (numero de filhos que a arvore pode ter);
*   - int (*cmp)(void*, void*): Funcao de comparacao utilizada
*     pelas chaves dos dados que serao armazenados, sendo o retorno:
*       > 0: Caso o primeiro seja maior que o segundo;
*       < 0: Caso o primeiro seja menor que o segundo;
*       = 0: Caso o primeiro seja igual ao segundo.
*   - void (*lc)(void*): Funcao que libera a chave;
*   - void (*li)(void*): Funcao que libera a item;
* Output:
*   - Uma estrutura Btree nova alocada dinamicamente que pode ser
*     utilizada com outras funcoes deste cabecalho.
*/
Btree* btree_nova(int ordem, int (*cmp)(void*, void*),
                  void (*lc)(void*), void (*li)(void*));

/*
* Insere um novo elemento na arvore.
*
* Inputs:
*   - Btree* arvore: Ponteiro para uma estrutura Btree,
*     inicializada previamente.
*   - void* chave: Chave do elemento que sera inserido, atraves
*     desta informacao o dado sera ordenado na arvore;
*   - void* item: Elemento que sera armazenado.
* Output:
*   - Nenhum, a arvore apenas e alterada com o novo elemento.
*/
void btree_inserir(Btree* arvore, void* chave, void* item);

/*
* Percorre a arvore aplicando em pre-ordem a funcao de callback
* em todos elemento.
*
* Inputs:
*   - Btree* arvore: Ponteiro para uma estrutura Btree,
*     inicializada previamente;
*   - void (*cb)(void*, void*): Funcao de callback que sera
*    aplicada recebendo um aux como segundo parametro.
*   - void *aux: Parametro auxiliar informado em cb.
* Output:
*   - Nenhum, a funcao de callback apenas e aplicada em todos
*     elementos em pre-ordem.
*/
void btree_percorrer(Btree* arvore, void (*cb)(void*, void*), void* aux);

/*
* Atualiza o elemento da chave, liberando o item antigo
* e trocando pelo item informado.
*
* Inputs:
*   - Btree* arvore: Ponteiro para uma estrutura Btree,
*     inicializada previamente.
*   - void* chave: Chave do elemento que sera atualizado.
*   - void (*li)(void*): Funcao aplicada para liberar
*     os itens.
* Output:
*   - void*, sendo um ponteiro para o item da chave informada,
*     caso nao exista tal chave na estrutura e retornado NULL.
*/
void btree_atualizar(Btree* arvore, void* chave, void* item);

/*
* Busca um elemento na arvore atravez da chave, retornando um
* ponteiro para o item, caso nao seja encontrado retorna NULL.
*
* Inputs:
*   - Btree* arvore: Ponteiro para uma estrutura Btree,
*     inicializada previamente.
*   - void* chave: Chave do elemento que sera buscado.
* Output:
*   - void*, sendo um ponteiro para o item da chave informada,
*     caso nao exista tal chave na estrutura e retornado NULL.
*/
void* btree_buscar(Btree* arvore, void* chave);

/*
* Retorna a altura da arvore.
*
* Inputs:
*   - Btree* arvore: Ponteiro para uma estrutura Btree,
*     inicializada previamente.
* Output:
*   - int, altura da arvore.
*/
int btree_altura(Btree* arvore);

/*
* Libera a estrutura de uma arvore B.
*
* Inputs:
*   - Btree* arvore: Ponteiro para uma estrutura Btree,
*     inicializada previamente.
* Output:
*   - Nenhum, a arvore apenas e liberada da heap.
*/
void btree_liberar(Btree* arvore);

#endif