#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

t_env *set_env_var(t_env *env, const char *key, const char *value) {
    t_env *cur = env;
    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            free(cur->value);
            cur->value = ft_strdup(value);
            return env;
        }
        if (!cur->next) break;
        cur = cur->next;
    }
    t_env *new = malloc(sizeof(t_env));
    new->key = ft_strdup(key);
    new->value = ft_strdup(value);
    new->next = NULL;
    if (cur) cur->next = new;
    else env = new;
    return env;
}

static char *strip_quotes(const char *val) {
    size_t len = ft_strlen(val);
    if (len >= 2 && ((val[0] == '"' && val[len-1] == '"') || (val[0] == '\'' && val[len-1] == '\''))) {
        char *res = malloc(len - 1);
        if (!res) return NULL;
        strncpy(res, val + 1, len - 2);
        res[len - 2] = '\0';
        printf("res: %s\n", res);
        return res;
    }
    return ft_strdup(val);
}

int export_builtin(char **args, t_env **env) {
    int i = 1;
    if (!args[i]) {
        t_env *tmp = *env;
        while (tmp) {
            printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
            tmp = tmp->next;
        }
        return 0;
    }
    while (args[i]) {
        char *eq = ft_strchr(args[i], '=');
        if (!eq) { i++; continue; }
        *eq = 0;
        char *val = strip_quotes(eq + 1);
        *env = set_env_var(*env, args[i], val);
        free(val);
        *eq = '=';
        i++;
    }
    return 0;
} 