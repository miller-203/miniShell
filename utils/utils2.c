#include "../minishell.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

int ft_ft_strlen_array(char **str)
{
    int i = 0;
    while (str[i])
        i++;
    return (i);
}

ast_node_t *create_ast_node(node_type_t type)
{
    ast_node_t *node = malloc(sizeof(ast_node_t));
    if (!node)
        return NULL;

    node->type = type;
    return node;
}

size_t ft_strlen(const char *s) {
    size_t i = 0;
    while (s && s[i])
        i++;
    return i;
}

char *ft_strchr(const char *s, int c) {
    if (!s) return NULL;
    while (*s) {
        if (*s == (char)c)
            return (char *)s;
        s++;
    }
    if (c == '\0')
        return (char *)s;
    return NULL;
}

char *ft_strdup(const char *s) {
    if (!s) return NULL;
    size_t len = ft_strlen(s);
    char *dup = malloc(len + 1);
    if (!dup) return NULL;
    size_t i = 0;
    while (i < len) {
        dup[i] = s[i];
        i++;
    }
    dup[len] = '\0';
    return dup;
}

void print_exec_error(const char *context, const char *detail) {
    if (detail && detail[0]) {
        write(2, "minishell: ", 11);
        if (context)
            write(2, context, ft_strlen(context));
        write(2, ": ", 2);
        write(2, detail, ft_strlen(detail));
        write(2, "\n", 1);
    } else if (errno) {
        write(2, "minishell: ", 11);
        if (context)
            write(2, context, ft_strlen(context));
        write(2, ": ", 2);
        const char *errstr = strerror(errno);
        write(2, errstr, ft_strlen(errstr));
        write(2, "\n", 1);
    } else {
        write(2, "minishell: ", 11);
        if (context)
            write(2, context, ft_strlen(context));
        write(2, ": error\n", 8);
    }
}
