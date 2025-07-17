/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdelilah <abdelilah@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:25:05 by yolaidi-          #+#    #+#             */
/*   Updated: 2025/07/17 19:42:40 by abdelilah        ###   ########.fr       */
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
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
int ft_ft_strlen_array(char **str);
ast_node_t *parse_command(parser_t *parser);
void free_ast(ast_node_t *node);
int is_redirection(const char *type);
redir_type_t get_redir_type(const char *type);
char	**ft_split(char const *s, char c);
void ft_copy_env(char **env, t_env **copy_env);
void ft_free_env(t_env **env);
size_t	ft_strlen(const char *str);
char    *ft_strchr(const char *s, int c);
char    *ft_strdup(const char *s);
// New: parsing step
ast_node_t *parse_input_line(const char *line);

// New: built-in and helpers
int ft_itoa_to_buffer(char *buffer, int num);
int ft_strcpy_to_buffer(char *dest, const char *src);
void ft_strncpy_safe(char *dest, const char *src, size_t n);
char *ft_realloc(char *ptr, size_t old_size, size_t new_size);
int export_builtin(char **args, t_env **env);
int unset_builtin(char **args, t_env **env);
t_env *set_env_var(t_env *env, const char *key, const char *value);
int heredoc(const char *delimiter);
char *expand_vars(const char *input, t_env *env, int last_status);
int ft_strcmp(const char *s1, const char *s2);
int is_builtin(const char *name);
int builtin_cd(char **args, t_env *env);
int builtin_pwd(void);
int builtin_echo(char **args);
int builtin_env(t_env *env);
int builtin_export(char **args, t_env **env);
int builtin_unset(char **args, t_env **env);
int builtin_exit(void);
#endif
