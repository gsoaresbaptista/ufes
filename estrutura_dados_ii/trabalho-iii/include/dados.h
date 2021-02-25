#ifndef __DADOS__
#define __DADOS__

#include "btree.h"
#include <stdio.h>
#include <stdbool.h>

/*
* Retorna o estado da informacao.
*
* Inputs:
*   - FILE* arquivo: Ponteiro para um arquivo.
*   - unsigned char* dado: String ja alocada que ira ser preenchida pelos
*     dados requeridos.
*   - long int* deslocamento: Quanto será necessário percorrer
*     no arquivo.
* Output:
*   - bool, se o dado esta ativo ou nao.
*/
bool dados_get(FILE* arquivo, unsigned char* dado, long int* deslocamento);

/*
* Modifica um byte específico do arquivo.
*
* Inputs:
*   - FILE* arquivo: Ponteiro para um arquivo.
*   - void* novo: informacao a ser colocada e precisa ter 1 byte.
*   - long int* deslocamento: Quanto será necessário percorrer
*     no arquivo.
* Output:
*   - Nenhum.
*/
void dados_set(FILE* arquivo, void* novo, long int* deslocamento);

/*
* Inseri um dado no final do arquivo.
*
* Inputs:
*   - FILE* arquivo: Ponteiro para um arquivo.
*    - char* conteudo: String a ser inserida no arquivo.
* Output:
*   - int, retorna o tamanho do dado inserido com exito.
*/
int dados_inserir(FILE* arquivo, unsigned char* conteudo);

/*
* Indexa o arquivo utilizando a arvore b, utilizando o identificador
* do cliente como chave e o ponterio para o inicio do metadado como
* item.
*
* Inputs:
*    - FILE* arquivo: Ponteiro para um arquivo.
*   - Btree* arvore: Ponteiro para uma estrutura Btree,
*     inicializada previamente.
* Output:
*   - Nenhum.
*/
void dados_indexar(FILE* arquivo, Btree* arvore);

/*
* Retorna se a informcao no deslocamente que foi passado
*  esta ativa ou nao.
*
* Inputs:
*   - FILE* arquivo: Ponteiro para um arquivo.
*    - long int* deslocamento: Quanto será necessário percorrer
*      no arquivo.
* Output:
*   - bool, estado da informacao.
*/
bool dados_ativo(FILE* arquivo, long int* deslocamento);

#endif