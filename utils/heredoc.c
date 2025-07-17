#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>

int heredoc(const char *delimiter) {
    int pipefd[2];
    if (pipe(pipefd) < 0) return -1;
    char *line = NULL;
    size_t len = 0;
    while (1) {
        write(1, "> ", 2);
        ssize_t n = getline(&line, &len, stdin);
        if (n < 0) break;
        if (n > 0 && line[n-1] == '\n') line[n-1] = 0;
        if (ft_strcmp(line, delimiter) == 0) break;
        write(pipefd[1], line, ft_strlen(line));
        write(pipefd[1], "\n", 1);
    }
    free(line);
    close(pipefd[1]);
    return pipefd[0];
}
