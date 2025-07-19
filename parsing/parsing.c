#include "../minishell.h"

ast_node_t *parse_pipeline(parser_t *parser)
{
    ast_node_t *left = parse_command(parser);
    if (!left)
        return NULL;

    while (parser->current < parser->token_count &&
           strcmp(parser->tokens[parser->current].type, "PIPE") == 0)
    {
        parser->current++;

        ast_node_t *right = parse_command(parser);
        if (!right)
        {
            free_ast(left);
            return NULL;
        }

        ast_node_t *pipeline = create_ast_node(NODE_PIPELINE);
        if (!pipeline)
        {
            free_ast(left);
            free_ast(right);
            return NULL;
        }

        pipeline->data.pipeline.left = left;
        pipeline->data.pipeline.right = right;

        left = pipeline;
    }

    return left;
}

ast_node_t *parse(token_t *tokens)
{
    if (!tokens || !tokens[0].type)
    {
        fprintf(stderr, "minishell: no tokens to parse\n");
        return NULL;
    }

    parser_t parser;
    parser.tokens = tokens;
    parser.current = 0;
    parser.token_count = count_tokens(tokens);

    return parse_pipeline(&parser);
}

ast_node_t *parse_input_line(const char *line) {
    if (!line || !*line)
        return NULL;
    token_t *tokens = tokenize((char *)line);
    if (!tokens)
        return NULL;
    int j = 0;
    while (tokens[j].type)
    {
        printf("Token %d: Type: %s, Value: %s\n", j, tokens[j].type, tokens[j].value);
        j++;
    }
    ast_node_t *ast = parse(tokens);
    free_tokens(tokens);
    return ast;
}