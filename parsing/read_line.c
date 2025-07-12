#include "../minishell.h"

char *read_line(t_env *copy_env)
{
    char *line;

    line = NULL;
    while (1)
    {
        line = readline("minishell-1.0$ ");
        if (line == NULL)
        {
            write(1, "exit\n", 5);
            break;
        }
        if (line[0] != '\0')
        {
            add_history(line);

            if (strcmp(line, "history -c") == 0)
                rl_clear_history();
            else if (strcmp(line, "history") == 0)
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
    return (line);
}