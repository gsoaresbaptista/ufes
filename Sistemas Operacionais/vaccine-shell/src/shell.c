#define _GNU_SOURCE
#define PROMPT "vsh> "
#define READ 0
#define WRITE 1

extern int errno;

#include "shell.h"
#include "texto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

// Struct utilizada durante o loop para manter as informações
typedef struct shell {
    pid_t* filhos;
    int n_filhos;
    Texto* texto;
} Shell;

// Funções estáticas
static void shell_vacinar();
static void shell_sigusr_handler();
static void shell_sigint_handler();
static void shell_protecao_sigint();
static void shell_remover_protecao_sigint();
static void shell_contaminar_sessao();
static void shell_liberar();
static void shell_separar_comando(char* (*comandos)[][4], Shell* shell, int* n);

static void shell_falha_comando(Shell* shell) {
    fprintf(stdout, "Falha ao executar comando!\n");
    shell_liberar(shell);
}

static void shell_separar_comando(char* (*comandos)[][4], Shell* shell, int* n) {
    int k = 0, len = texto_pegar_quantidade(shell->texto);
    
    for (int i = 0; i < len; i++) {
        char* string = texto_pegar_string(shell->texto, i);

        if (strcmp(string, "|") != 0) {
            (*comandos)[*n][k++] = string;
        }
        else {
            (*comandos)[(*n)++][k] = NULL;
            k = 0;
        }
    }
    (*comandos)[(*n)++][k] = NULL;
}

static void shell_executar_pipeline(Shell* shell) {
    // Separando em comandos e argumentos do pipe.
    int n = 0;
    char* comandos[6][4];
    shell_separar_comando(&comandos, shell, &n);
    
    /* Um novo processo criado terá sid diferente e será pai
       dos demais processos */
    pid_t pid = fork();

    if (pid == 0) {
        setsid();

        // Criando os pipes (apenas os necessários e no máximo 4)
        int pipes[4][2];
        for (int i = 0; i < n-1; i++) {
            if (pipe(pipes[i]) == -1) {
                fprintf(stderr, "Erro na criação do pipe\n");
                shell_falha_comando(shell);
            }
        }

        for (int i = 0; i < n; i++) {
            char** args = comandos[i];
            pid = fork();
            // Filhos que vão rodar exec
            if (pid == 0) {
                // Primeiro do pipe
                if (i > 0) {
                    dup2(pipes[i-1][READ], READ);
                    close(pipes[i-1][READ]);
                    close(pipes[i-1][WRITE]);
                }
                else { 
                    close(pipes[i][READ]);
                }

                // Ultimo do pipe
                if (i < n-1) {
                    dup2(pipes[i][WRITE], WRITE);
                    close(pipes[i][WRITE]);
                    close(pipes[i][READ]);
                }
                else {
                    close(pipes[i-1][WRITE]);
                }

                // Fechando os demais pipes
                for (int j = 0; j < n-1; j++) {
                    if (j != i && j != i-1) {
                        close(pipes[j][READ]);
                        close(pipes[j][WRITE]);
                    }
                }
                execvp(args[0], args);
                shell_falha_comando(shell);
            }
        }

        // Fechar os descritores não usados do pai
        for(int j = 0; j < n-1; j++) {
            close(pipes[j][READ]);
            close(pipes[j][WRITE]);
        }
        close(READ);
        close(WRITE);

        // Esperar os filhos terminarem sua execução
        int status = 0;
        while (wait(&status) > 0) {
            if (status == SIGUSR1 || status == SIGUSR2)
                shell_contaminar_sessao();
        }
        shell_liberar(shell);
    }
    else {
        shell->n_filhos++;
        shell->filhos = realloc(shell->filhos, sizeof(pid_t) * shell->n_filhos);
        shell->filhos[shell->n_filhos - 1] = pid;
    }
}

static void shell_executar(Shell* shell) {
    char* comando = texto_pegar_string(shell->texto, 0);
    pid_t pid = fork();

    if (pid == -1) {  
        fprintf(stderr, "Erro! Falha no fork!\n");
    }
    else if (pid > 0) {
        // Para filho não virar zombie
        waitpid(pid, NULL, 0);
        shell_remover_protecao_sigint();
    }
    else {
        int erro = 0;

        if (texto_pegar_quantidade(shell->texto) > 1) {
            // Comando com parâmetros
            texto_adicionar_string(shell->texto, NULL);
            erro = execvp(comando, texto_pegar_strings(shell->texto));
            shell_falha_comando(shell);
        }
        else {
            // Comando sem parâmetros
            erro = execlp(comando, comando, NULL);
            shell_falha_comando(shell);
        }
        
        if (erro == -1) {
            // Erro ao executar comando
            fprintf(stderr, "Erro! %s\n", strerror(errno));
        }
    }
}

static void shell_libera_moita() {
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

static void shell_armageddon(Shell* shell) {
    shell_libera_moita();
    for (int i = 0; i < shell->n_filhos; i++)
        killpg(shell->filhos[i], SIGKILL);
    if (shell->filhos != NULL)
        free(shell->filhos);
    exit(EXIT_SUCCESS);
}

static void shell_comando(Shell* shell, bool pipe) {
    char* comando = texto_pegar_string(shell->texto, 0);
    
    if (!strcmp(comando, "cd")) {
        chdir(texto_pegar_string(shell->texto, 1));
    }
    else if (!strcmp(comando, "pid")) {
        printf("pid: %d\n", getpid());
    }
    else if (!strcmp(comando, "liberamoita")) {
        shell_libera_moita();
    }
    else if (!strcmp(comando, "armageddon") || !strcmp(comando, "exit")) {
        shell_liberar(shell);
    }
    else {
        if (pipe) {
            shell_executar_pipeline(shell);
        }
        else {
            shell_protecao_sigint();
            shell_executar(shell);
        } 
    }
}

void shell_loop() {
    int len;
    bool pipe;

    Shell shell = { 
        .filhos = NULL,
        .n_filhos = 0,
        .texto = texto_novo()
    };
    
    shell_vacinar();

    while (true) {
        printf(PROMPT);
        pipe = false;
        shell.texto = texto_ler(shell.texto, " ");

        // Verifica se o comando informado não é nulo
        if ((len = texto_pegar_quantidade(shell.texto)) > 0) {
            for (int i = 0; i < len; i++) {
                if (strcmp(texto_pegar_string(shell.texto, i), "|") == 0) {
                    pipe = true;
                    break;
                } 
            }
            shell_comando(&shell, pipe);
        }
    }
}

static void shell_sigusr_handler() {
    printf("\n"
           "              _  _\n"
           "    _ _      (0)(0)-._  _.-'^^'^^'^^'^^'^^'--.\n"
           "   (.(.)----'`        ^^'                /^   ^^-._\n"
           "    (    `                 \\             |    _    ^^-._\n"
           "    VvvvvvvVv~~`__,/.._>  /:/:/:/:/:/:/:/\\  (_..,______^^-.\n"
           "     `^^^^^^^^`/  /   /  /`^^^^^^^^^>^^>^`>  >        _`)  )\n"
           "              (((`   (((`          (((`  (((`        `'--'^\n"
           "I feel weird...\n");
    printf(PROMPT);
    fflush(stdout);
}

static void shell_sigint_handler() {
    /* Processo pai não faz nada com o SIGINT mas
       o filho, como não tem tratamento, morre. */
    return;
}

static void shell_protecao_sigint() {
    // Definindo handler do SIGINT para processos em foreground
    struct sigaction interrupt;
    interrupt.sa_handler = &shell_sigint_handler;
    interrupt.sa_flags = SA_RESTART;
    sigaction(SIGINT, &interrupt, NULL);
}

static void shell_contaminar_sessao() {
    pid_t sid = getsid(getpid());
    killpg(sid, SIGKILL);
}

static void shell_remover_protecao_sigint() {
    signal(SIGINT, SIG_DFL);
}

static void shell_liberar(Shell* shell) {
    texto_liberar(shell->texto);
    shell_armageddon(shell);
}

static void shell_vacinar() {
    // Adiciona os sinais bloqueados
    sigset_t block_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGINT);
    sigaddset(&block_mask, SIGQUIT);  
    sigaddset(&block_mask, SIGTSTP);
    
    // Definindo máscara e handler para SIGUSR1 e SIGUSR2
    struct sigaction user;
    user.sa_handler = &shell_sigusr_handler;
    user.sa_flags = SA_RESTART;
    user.sa_mask = block_mask;
    sigaction(SIGUSR1, &user, NULL);
    sigaction(SIGUSR2, &user, NULL);
}
