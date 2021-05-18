#define _GNU_SOURCE
#define FATOR 2

#include "texto.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char* texto_pegar_linha();

struct texto {
	char** textos;
	int* tamanhos;
	int quantidade;
};

Texto* texto_novo() {
	Texto* texto = (Texto*)malloc(sizeof(Texto));
	texto->textos = NULL;
	texto->tamanhos = NULL;
	texto->quantidade = 0;
	
	return texto;
}

static char* texto_pegar_linha() {
    size_t buffsize;
	char* linha = NULL;
    getline(&linha, &buffsize, stdin);
    
	// Remove a quebra de linha
	int tamanho = strlen(linha);
	linha[tamanho - 1] = '\0';
	
	return linha;
}

Texto* texto_ler(Texto* texto, char* delimitador) {
	// Redefine a struct caso já esteja preenchida
	if (texto->textos != NULL) {
		texto_liberar(texto);
		texto = texto_novo();
	}

	int quantidade = 0;
	int maximo = 2;
	char* linha = texto_pegar_linha();
	char* ptr = strtok(linha, delimitador);
	
	char** textos = (char**)malloc(sizeof(char*) * maximo);
	int* tamanhos = (int*)malloc(sizeof(int) * maximo);

	while (ptr != NULL) {
		int tamanho = strlen(ptr);

		// Se necessário aumenta o vetor de strings
		if (quantidade >= maximo) {
			maximo *= FATOR;
			textos = realloc(textos, maximo * sizeof(char*));
			tamanhos = realloc(tamanhos, maximo * sizeof(int));
		}
		// Escreve os dados da string atual
		textos[quantidade] = (char*)malloc(sizeof(char) * (tamanho + 1));
		tamanhos[quantidade] = tamanho;
		strcpy(textos[quantidade++], ptr);
		ptr = strtok(NULL, delimitador);
	}

	// Retira os espaços sobressalentes
	if (quantidade < maximo) {
		textos = realloc(textos, sizeof(char*) * quantidade);
		tamanhos = realloc(tamanhos, sizeof(int) * quantidade);
	}
	free(linha);

	// Atribui as variáveis do texto
	texto->textos = textos;
	texto->tamanhos = tamanhos;
	texto->quantidade = quantidade;

	return texto;
}

char* texto_pegar_string(Texto* texto, int posicao) {
	return texto->textos[posicao];
}

int texto_pegar_tamanho(Texto* texto, int posicao) {
	return texto->tamanhos[posicao];
}

int texto_pegar_quantidade(Texto* texto) {
	return texto->quantidade;
}

char** texto_pegar_strings(Texto* texto) {
	return texto->textos;
}

void texto_adicionar_string(Texto* texto, char* novo) {
	// Aumenta textos para tamanho + 1
	texto->quantidade++;
	texto->textos = realloc(texto->textos, sizeof(char*) * texto->quantidade);
	texto->textos[texto->quantidade - 1] = novo;
}

void texto_liberar(Texto* texto) {
	for (int i = 0; i < texto->quantidade; i++)
		free(texto->textos[i]);
	free(texto->textos);
	free(texto->tamanhos);
	free(texto);
}