#include "tabela.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Parametros insuficientes!\n");
    }
    else {
        int m = atoi(argv[1]);
        char* arquivo = argv[2];

        Tabela* tabela = tabela_nova(arquivo, m);
        tabela_loop(tabela);
        tabela_liberar(tabela);
    }

    return 0;
}