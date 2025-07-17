#include "../minishell.h"

ast_node_t *parse_command(parser_t *parser)
{
    command_t *cmd = create_command();
    if (!cmd)
        return NULL;

    while (parser->current < parser->token_count)
    {
        token_t *token = &parser->tokens[parser->current];

        if (strcmp(token->type, "PIPE") == 0)
            break;

        if (is_redirection(token->type))
        {
            redir_type_t redir_type = get_redir_type(token->type);
            parser->current++;

            if (parser->current >= parser->token_count ||
                strcmp(parser->tokens[parser->current].type, "WORD") != 0)
            {
                fprintf(stderr, "minishell: syntax error: expected filename after redirection\n");
                free(cmd);
                return NULL;
            }

            redirection_t *redir = create_redirection(redir_type, parser->tokens[parser->current].value);

            if (!cmd->redirections)
            {
                cmd->redirections = redir;
            }
            else
            {
                redirection_t *temp = cmd->redirections;
                while (temp->next)
                    temp = temp->next;
                temp->next = redir;
            }

            parser->current++;
            continue;
        }

        if (strcmp(token->type, "WORD") == 0)
        {
            if (cmd->arg_count >= MAX_ARGS - 1)
            {
                fprintf(stderr, "minishell: too many arguments\n");
                break;
            }

            cmd->args[cmd->arg_count] = ft_strdup(token->value);

            if (cmd->arg_count == 0)
                cmd->name = ft_strdup(token->value);

            cmd->arg_count++;
            parser->current++;
        }
        else
            break;
    }

    cmd->args[cmd->arg_count] = NULL;

    ast_node_t *node = create_ast_node(NODE_COMMAND);
    if (!node)
    {
        free(cmd);
        return NULL;
    }

    node->data.command = cmd;
    return node;
}