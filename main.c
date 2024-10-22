#include "minishell.h"

int main(void)
{
    char *input;
    char *args[100];  // tableau pour stocker la commande et ses arguments
    pid_t pid;
    int status;

    while (1) {
        // Affiche le prompt et lit la commande de l'utilisateur
        input = readline("minishell$ ");
        if (!input)
            break;

        // Ajoute la commande à l'historique
        add_history(input);

        // Sépare la commande en programme et arguments
        int i = 0;
        char *token = strtok(input, " ");
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;  // Le dernier élément du tableau doit être NULL pour execve

        // Crée un processus fils
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            // Processus fils : exécute la commande avec execve
            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }
        else {
            // Processus parent : attend que le processus fils termine
            waitpid(pid, &status, 0);
        }

        // Libère la mémoire allouée
        free(input);
    }

    printf("Exit Minishell\n");
    return 0;
}

