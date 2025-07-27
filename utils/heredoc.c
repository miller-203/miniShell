#include "../minishell.h"

int heredoc(const char *delimiter)
{
    int pipefd[2];
    char *line = NULL;

    if (pipe(pipefd) < 0) {
        perror("pipe");
        return -1;
    }
    
    while (1) {
        line = readline("> ");
        if (!line) {
            break;
        }
        if (ft_strcmp(line, delimiter) == 0) {
            free(line);
            break;
        }
        if (write(pipefd[1], line, ft_strlen(line)) < 0 ||
            write(pipefd[1], "\n", 1) < 0) {
            perror("write");
            free(line);
            break;
        }
        free(line);
    }
    
    close(pipefd[1]);
    return pipefd[0];
}