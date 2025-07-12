#include "../minishell.h"

int ft_strlen_array(char **str)
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
