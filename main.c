#include "minishell.h"

void print_ast(ast_node_t *node, int depth)
{
    if (!node)
        return;

    int i = 0;
    while (i < depth)
    {
        printf("  ");
        i++;
    }

    if (node->type == NODE_COMMAND)
    {
        printf("COMMAND: %s\n", node->data.command->name ? node->data.command->name : "NULL");
        int i = 0;
        while (i < node->data.command->arg_count)
        {
            int j = 0;
            while (j < depth + 1)
            {
                printf("  ");
                j++;
            }
            printf("ARG[%d]: %s\n", i, node->data.command->args[i]);
            i++;
        }

        redirection_t *redir = node->data.command->redirections;
        while (redir)
        {
            int j = 0;
            while (j < depth + 1)
            {
                printf("  ");
                j++;
            }
            printf("REDIR: %d -> %s\n", redir->type, redir->filename);
            redir = redir->next;
        }
    }
    else if (node->type == NODE_PIPELINE)
    {
        printf("PIPELINE:\n");
        print_ast(node->data.pipeline.left, depth + 1);
        print_ast(node->data.pipeline.right, depth + 1);
    }
}

char **env_list_to_environ(t_env *env) {
    if (!env) return NULL;
        int count = 0;
    t_env *tmp = env;
    while (tmp) { 
        if (tmp->key) count++;
        tmp = tmp->next; 
    }
    
    char **environ = malloc(sizeof(char*) * (count + 1));
    if (!environ) return NULL;
    
    tmp = env;
    int i = 0;
    while (tmp && i < count) {
        if (!tmp->key) {
            tmp = tmp->next;
            continue;
        }
        
        const char *value = tmp->value ? tmp->value : "";
        size_t len = ft_strlen(tmp->key) + ft_strlen(value) + 2;
        
        environ[i] = malloc(len);
        if (!environ[i]) {
            int j = 0;
            while (j < i) {
                free(environ[j]);
                j++;
            }
            free(environ);
            return NULL;
        }
        
        snprintf(environ[i], len, "%s=%s", tmp->key, value);
        i++;
        tmp = tmp->next;
    }
    
    environ[i] = NULL;
    return environ;
}

void free_environ(char **env) {
    if (!env) return;
    int i = 0;
    while (env[i]) free(env[i++]);
    free(env);
}

int apply_redirections(redirection_t *redir) {
    while (redir) {
        int fd;
        if (redir->type == REDIR_INPUT) {
            fd = open(redir->filename, O_RDONLY);
            if (fd < 0) { perror("open input"); return -1; }
            dup2(fd, STDIN_FILENO); close(fd);
        } else if (redir->type == REDIR_OUTPUT) {
            fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) { perror("open output"); return -1; }
            dup2(fd, STDOUT_FILENO); close(fd);
        } else if (redir->type == REDIR_APPEND) {
            fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) { perror("open append"); return -1; }
            dup2(fd, STDOUT_FILENO); close(fd);
        } else if (redir->type == REDIR_HEREDOC) {
            fd = heredoc(redir->filename);
            if (fd < 0) { fprintf(stderr, "heredoc failed\n"); return -1; }
            dup2(fd, STDIN_FILENO); close(fd);
        }
        redir = redir->next;
    }
    return 0;
}

char *find_in_path(const char *cmd, t_env *env) {
    if (!cmd) return NULL;
    
    if (ft_strchr(cmd, '/')) {
        if (access(cmd, X_OK) == 0) {
            return ft_strdup(cmd);
        }
        return NULL;
    }
    
    const char *path = NULL;
    t_env *tmp = env;
    while (tmp) {
        if (ft_strcmp(tmp->key, "PATH") == 0) {
            path = tmp->value;
            break;
        }
        tmp = tmp->next;
    }
    
    if (!path) path = "/bin:/usr/bin:/usr/local/bin";
    
    char *paths = ft_strdup(path);
    if (!paths) return NULL;
    
    char *saveptr = NULL;
    char *dir = strtok_r(paths, ":", &saveptr);
    while (dir) {
        char full[1024];
        snprintf(full, sizeof(full), "%s/%s", dir, cmd);
        if (access(full, X_OK) == 0) {
            free(paths);
            return ft_strdup(full);
        }
        dir = strtok_r(NULL, ":", &saveptr);
    }
    free(paths);
    return NULL;
}

int	execute_command(command_t *cmd, t_env **env)
{
	int				i;
	redirection_t	*r;
	int				saved_stdin;
	int				saved_stdout;
	int				result;
	pid_t			pid;
	int				status;
	char			*expanded;
	char			*exec_path;
	char			**envp;

	if (!cmd || !cmd->name)
		return (1);
	i = 0;
	while (i < cmd->arg_count)
	{
		expanded = expand_vars(cmd->args[i], *env, 0);
		printf("expanded: %s\n", expanded);
		if (expanded)
		{
			free(cmd->args[i]);
			cmd->args[i] = expanded;
		}
		i++;
	}
	r = cmd->redirections;
	while (r)
	{
		expanded = expand_vars(r->filename, *env, 0);
		if (expanded)
		{
			free(r->filename);
			r->filename = expanded;
		}
		r = r->next;
	}
	if (is_builtin(cmd->name))
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		if (apply_redirections(cmd->redirections) < 0)
		{
			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);
			return (1);
		}
		result = 1;
		if (ft_strcmp(cmd->name, "exit") == 0)
			result = builtin_exit();
		else if (ft_strcmp(cmd->name, "cd") == 0)
			result = builtin_cd(cmd->args, *env);
		else if (ft_strcmp(cmd->name, "pwd") == 0)
			result = builtin_pwd();
		else if (ft_strcmp(cmd->name, "echo") == 0)
			result = builtin_echo(cmd->args);
		else if (ft_strcmp(cmd->name, "env") == 0)
			result = builtin_env(*env);
		else if (ft_strcmp(cmd->name, "export") == 0)
			result = builtin_export(cmd->args, env);
		else if (ft_strcmp(cmd->name, "unset") == 0)
			result = builtin_unset(cmd->args, env);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (result);
	}
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (apply_redirections(cmd->redirections) < 0)
			exit(1);
		exec_path = find_in_path(cmd->name, *env);
		if (!exec_path)
		{
			fprintf(stderr, "minishell: %s: command not found\n", cmd->name);
			exit(127);
		}
		envp = env_list_to_environ(*env);
		if (!envp)
		{
			fprintf(stderr, "minishell: failed to convert environment\n");
			free(exec_path);
			exit(1);
		}
		execve(exec_path, cmd->args, envp);
		perror("execve");
		free_environ(envp);
		free(exec_path);
		exit(127);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (1);
	}
	else
	{
		perror("fork");
		return (1);
	}
}

int	execute_ast(ast_node_t *node, t_env **env)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		status1;
	int		status2;

	if (!node)
		return (1);
	if (node->type == NODE_COMMAND)
		return (execute_command(node->data.command, env));
	else if (node->type == NODE_PIPELINE)
	{
		if (pipe(pipefd) < 0)
		{
			perror("pipe");
			return (1);
		}
		left_pid = fork();
		if (left_pid == 0)
		{
			close(pipefd[0]);
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
			exit(execute_ast(node->data.pipeline.left, env));
		}
		else if (left_pid < 0)
		{
			perror("fork left");
			close(pipefd[0]);
			close(pipefd[1]);
			return (1);
		}
		right_pid = fork();
		if (right_pid == 0)
		{
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			exit(execute_ast(node->data.pipeline.right, env));
		}
		else if (right_pid < 0)
		{
			perror("fork right");
			close(pipefd[0]);
			close(pipefd[1]);
			kill(left_pid, SIGTERM);
			waitpid(left_pid, NULL, 0);
			return (1);
		}
		close(pipefd[0]);
		close(pipefd[1]);
		waitpid(left_pid, &status1, 0);
		waitpid(right_pid, &status2, 0);
		if (WIFEXITED(status2))
			return (WEXITSTATUS(status2));
		return (1);
	}
	return (1);
}

int ft_work_parse_execute(char *line, t_env **env)
{
    ast_node_t *ast = parse_input_line(line);
    if (!ast)
        return 1;
    print_ast(ast, 0);
    int status = execute_ast(ast, env);
    free_ast(ast);
    return status;
}

void sigint_handler(int signo)
{
    (void)signo;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

static const char *get_env_value(const char *key, t_env *env) {
    while (env) {
        if (strcmp(env->key, key) == 0)
            return env->value;
        env = env->next;
    }
    return "";
}

static int is_var_char(char c) {
    return (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'));
}

char *expand_vars(const char *input, t_env *env, int last_status)
{
    size_t len;
    size_t result_size;
    char *result;
    size_t ri;
    int in_single;
    int in_double;
    size_t i;
    
    if (!input)
        return (NULL);
    len = ft_strlen(input);
    result_size = len * 2 + 1;
    result = malloc(result_size);
    if (!result)
        return (NULL);
    
    ri = 0;
    in_single = 0;
    in_double = 0;
    i = 0;
    
    while (i < len)
    {
        if (ri + 256 > result_size)
        {
            size_t old_size = result_size;
            char *new_result;
            result_size *= 2;
            new_result = ft_realloc(result, old_size, result_size);
            if (!new_result)
            {
                free(result);
                return (NULL);
            }
            result = new_result;
        }
        
        if (input[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (input[i] == '\"' && !in_single)
            in_double = !in_double;
        else if (input[i] == '$' && !in_single)
        {
            if (i + 1 < len && input[i + 1] == '?')
            {
                ri += ft_itoa_to_buffer(result + ri, last_status);
                i++;
            }
            // Handle ${VAR} syntax
            else if (i + 1 < len && input[i + 1] == '{')
            {
                size_t j = i + 2;
                size_t start = j;
                char var[256];
                size_t var_len;
                
                while (j < len && input[j] != '}')
                    j++;
                
                if (j < len && input[j] == '}')
                {
                    var_len = j - start;
                    if (var_len < sizeof(var))
                    {
                        ft_strncpy_safe(var, input + start, var_len);
                        var[var_len] = 0;
                        ri += ft_strcpy_to_buffer(result + ri, get_env_value(var, env));
                    }
                    i = j;
                }
                else
                {
                    result[ri++] = input[i];
                }
            }
            // Handle $VAR syntax (not digit as first char)
            else if (i + 1 < len && is_var_char(input[i + 1])
                    && (input[i + 1] < '0' || input[i + 1] > '9'))
            {
                size_t j = i + 1;
                size_t start = j;
                char var[256];
                size_t var_len;
                
                while (j < len && is_var_char(input[j]))
                    j++;
                
                var_len = j - start;
                if (var_len < sizeof(var))
                {
                    ft_strncpy_safe(var, input + start, var_len);
                    var[var_len] = 0;
                    ri += ft_strcpy_to_buffer(result + ri, get_env_value(var, env));
                }
                i = j - 1;
            }
            else
            {
                result[ri++] = input[i];
            }
        }
        else
        {
            result[ri++] = input[i];
        }
        i++;
    }
    
    result[ri] = 0;
    printf("debug: result %s\n", result);
    return (result);
}

int is_builtin(const char *name) {
    return (strcmp(name, "cd") == 0 ||
            strcmp(name, "pwd") == 0 ||
            strcmp(name, "echo") == 0 ||
            strcmp(name, "env") == 0 ||
            strcmp(name, "export") == 0 ||
            strcmp(name, "unset") == 0 ||
            strcmp(name, "exit") == 0);
}

int main(int argc, char **argv, char **env)
{
    char *line;

    (void)argc;
    (void)argv;

    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);

    t_env *copy_env;
    ft_copy_env(env, &copy_env);
    
    while (1)
    {
        line = readline("minishell$ ");

        if (line == NULL)
        {
            write(1, "exit\n", 5);
            break;
        }
        
        if (line[0] != '\0')
        {
            add_history(line);

            if (ft_strcmp(line, "history -c") == 0)
                rl_clear_history();
            else if (ft_strcmp(line, "history") == 0)
            {
                HIST_ENTRY **hist_list = history_list();
                if (hist_list)
                {
                    int i = 0;
                    while (hist_list[i])
                    {
                        printf("\t%d  %s\n", i + history_base, hist_list[i]->line);
                        i++;
                    }
                }
            }
            else
                ft_work_parse_execute(line, &copy_env);
        }
        free(line);
    }
    
    ft_free_env(&copy_env);
    return 0;
}