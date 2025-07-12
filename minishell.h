/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolaidi- <yolaidi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:25:05 by yolaidi-          #+#    #+#             */
/*   Updated: 2025/06/25 17:07:40 by yolaidi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

# define MAX_TOKENS 100

typedef struct s_token {
    char *type;
    char *value;
} token_t;

#define MAX_ARGS 1024

typedef enum
{
    NODE_COMMAND,
    NODE_PIPELINE,
    NODE_REDIRECT
} node_type_t;

typedef enum
{
    REDIR_INPUT,
    REDIR_OUTPUT,
    REDIR_APPEND,
    REDIR_HEREDOC
} redir_type_t;

typedef struct redirection
{
    redir_type_t type;
    char *filename;
    struct redirection *next;
} redirection_t;

typedef struct command
{
    char *name;
    char **args;
    int arg_count;
    redirection_t *redirections;
} command_t;

typedef struct ast_node
{
    node_type_t type;
    union
    {
        command_t *command; 
        struct
        {
            struct ast_node *left;
            struct ast_node *right;
        } pipeline;
    } data;
} ast_node_t;

typedef struct parser
{
    token_t *tokens;
    int current;
    int token_count;
} parser_t;

typedef struct s_env {
    char *key;
    char *value;
    struct s_env *next;
} t_env;

ast_node_t *parse_pipeline(parser_t *parser);
ast_node_t *parse_command(parser_t *parser);
command_t *create_command(void);
redirection_t *create_redirection(redir_type_t type, char *filename);
token_t *tokenize(char *input);
void free_tokens(token_t *tokens);
void ft_free_args(char **args);
ast_node_t *parse(token_t *tokens);
void free_ast(ast_node_t *node);
ast_node_t *create_ast_node(node_type_t type);
int count_tokens(token_t *tokens);
int ft_strlen_array(char **str);
ast_node_t *parse_command(parser_t *parser);
void free_ast(ast_node_t *node);
int is_redirection(const char *type);
redir_type_t get_redir_type(const char *type);
char	**ft_split(char const *s, char c);
void ft_copy_env(char **env, t_env **copy_env);
void ft_free_env(t_env **env);

#endif
