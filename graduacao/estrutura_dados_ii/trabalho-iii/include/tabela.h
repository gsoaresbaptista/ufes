#ifndef __TABELA__
#define __TABELA__

#include <stdbool.h>

/*
 * A estrutura Tabela guarda os dados referentes ao problema
 * geral do trabalho.
 */
typedef struct tabela Tabela;

/*
* Instancia uma estrutura de tabela.
*
* Inputs:
*   - char* arquivo: String contendo o nome do arquivo;
*   - int ordem: Ordem utilizada na btree(M).
* Output:
*   - Uma estrutura Tabela nova alocada dinamicamente que pode ser
*     utilizada com outras funcoes deste cabecalho.
*/
Tabela* tabela_nova(char* arquivo, int ordem);

/*
* Coloca a instancia de tabela em um laco de repeticao de comandos,
* ou seja, espera-se um comando da entrada padrao e o executa enquando
* o comando fornecido for diferente de STOP.
*
* Inputs:
*   - Tabela* tabela: Ponteiro para uma estrutura Tabela,
*     inicializada previamente.
* Output:
*   -  Nenhum, apenas coloca a estrutura em um laco de repeticao.
*/
void tabela_loop(Tabela* tabela);

/*
* Cria um arquivo de texto com todas as informacoes dos clientes
* ativos dentro da tabela, note que sera requisitado o nome apos
* a chamada da funcao.
*
* Inputs:
*   - Tabela* tabela: Ponteiro para uma estrutura Tabela,
*     inicializada previamente.
* Output:
*   -  Nenhum, apenas o arquivo com o nome requisitado sera criado.
*/
void comando_rundown(Tabela* tabela);

/*
* Insere um novo cliente na tabela, caso o cliente nao exista,
* e inserido no fim do arquivo e na arvore b, caso ja exista mas
* esteja inativo se insere no final do arquivo atualizando a arvore,
* por fim, caso esteja ativo coloca como inativo insere no fim do arquivo
* e atualiza a arvore. Note que sera requisitado a chave e o item apos
* a chamada da funcao.
*
* Inputs:
*   - Tabela* tabela: Ponteiro para uma estrutura Tabela,
*     inicializada previamente.
* Output:
*   -  Nenhum, o novo cliente apenas e inserido no arquivo.
*/
void comando_insert(Tabela* tabela);

/*
* Remove um cliente do arquivo, note que esta operacao esta
* sendo executada de maneira logica e sera requisitado a chave
* apos a chamada da funcao.
*
* Inputs:
*   - Tabela* tabela: Ponteiro para uma estrutura Tabela,
*     inicializada previamente.
* Output:
*   -  Nenhum, o cliente apenas e removido no arquivo.
*/
void comando_delete(Tabela* tabela);

/*
* Imprime na saida padrao a chave e o item correspondente,
* no formato "%s: %s\n", sendo o primeiro a chave e o segundo
* o item.
*
* Inputs:
*   - Tabela* tabela: Ponteiro para uma estrutura Tabela,
*     inicializada previamente.
* Output:
*   -  Nenhum, o cliente apenas e impresso na saida padrao.
*/
void comando_get(Tabela* tabela);

/*
* Libera a estrutura de uma Tabela.
*
* Inputs:
*   - Tabela* tabela: Ponteiro para uma estrutura Tabela,
*     inicializada previamente.
* Output:
*   - Nenhum, a tabela apenas e liberada da heap.
*/
void tabela_liberar(Tabela* tabela);

#endif