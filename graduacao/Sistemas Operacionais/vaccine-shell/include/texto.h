#ifndef TEXTO_H_
#define TEXTO_H_

typedef struct texto Texto;

/*
* Inicializa uma nova estrutura de texto vazia.
*
* Entradas: Nenhuma.
* Saida: Ponteiro para texto inicializada na heap.
*/
Texto* texto_novo();

/*
* Lê um texto da entrada e o separada de acordo com o delimitador
* informado, separando os textos entre cada ocorrência encontrada.
* Ex.:
*   texto = "arroz|feijao|carne";
*   delimitador = "|";
*   resultado salvo na struct = ["arroz", "feijao", "carne"]
*
* Entradas: Ponteiro para texto e .
* Saida: Ponteiro para texto e uma string com o delimitador.
*/
Texto* texto_ler(Texto* texto, char* delimitador);

/*
* Retorna o tamanho da string na posição informada em texto.
* Ex:
*   struct com strings = ["arroz", "feijao", "carne"];
*   posicao = 1;
*   tamanho = strlen(feijao) = 6.
*
* Entradas: Ponteiro para texto e inteiro com a posicao da string.
* Saida: Inteiro com o tamanho(strlen) da string em posicao.
*/
int texto_pegar_tamanho(Texto* texto, int posicao);

/*
* Retorna a quantidade de strings salvas no texto.
* Ex:
*   struct com strings = ["arroz", "feijao", "carne"];
*   quantidade = 3.
*
* Entradas: Ponteiro para texto.
* Saida: Inteiro com a quantidade de strings salva na estrutura.
*/
int texto_pegar_quantidade(Texto* texto);

/*
* Retorna ponteiro para o vetor de strings salvas na estrutura, note que
* as strings estão salvas na heap e ainda serão liberadas ao utilizar
* texto_liberar.
*
* Entradas: Ponteiro para texto.
* Saida: Ponteiro para vetor de strings alocadas na heap.
*/
char** texto_pegar_strings(Texto* texto);

/*
* Retorna o ponteiro para a string salva em texto na posição requisitada.
*
* Ex:
*   struct com strings = ["arroz", "feijao", "carne"];
*   posicao = 0.
*   resultado = "arroz".
*
* Entradas: Ponteiro para texto.
* Saida: Ponteiro para string alocada na heap.
*/
char* texto_pegar_string(Texto* texto, int posicao);

/*
* Adiciona uma nova string na estrutura texto.
*
* Ex:
*   struct inicial com strings = ["arroz", "feijao", "carne"];
*   novo = "batata". (precisa estar previamente alocada na heap);
*   struct final com strings = ["arroz", "feijao", "carne", "batata"].
*
* Entradas: Ponteiro para texto e ponteiro para string alocada na heap.
* Saida: void.
*/
void texto_adicionar_string(Texto* texto, char* novo);

/*
* Libera uma estrutura texto da heap.
*
* Entradas: Ponteiro para texto.
* Saida: void.
*/
void texto_liberar(Texto* texto);

#endif

