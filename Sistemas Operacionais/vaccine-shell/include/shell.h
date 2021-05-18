#ifndef SHELL_H_
#define SHELL_H_

/*
* Inicializa um novo vaccine shell, entrando no loop principal esperando
* comandos para iniciar os processos de foreground e background.
*
* Comandos nativos do shell:
*   - cd: muda o PATH de execução do shell;
*   - liberamoita: libera processos no estado zombie;
*   - armageddon: mata todos processos em execução e encerra o processo.
*   - exit: alias para armageddon.
*
* Entradas: Nenhuma.
* Saida: void.
*/
void shell_loop();

#endif