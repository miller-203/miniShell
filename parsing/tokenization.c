#include "../minishell.h"

token_t *tokenize(char *input)
{
    int i;
    int j;
    int token_count;

    i = 0;
    token_count = 0;
    if (!input)
        return NULL;

    token_t *tokens = malloc(sizeof(token_t) * MAX_TOKENS);
    if (!tokens)
        return NULL;
    while (input[i] && token_count < MAX_TOKENS - 1)
    {
        while (input[i] && (input[i] == ' ' || input[i] == '\t'))
            i++;

        if (!input[i])
            break;

        if (input[i] == '|')
        {
            tokens[token_count].type = "PIPE";
            tokens[token_count].value = strdup("|");
            i++;
        }
        else if (input[i] == '<')
        {
            if (input[i + 1] == '<')
            {
                tokens[token_count].type = "REDIR_HEREDOC";
                tokens[token_count].value = strdup("<<");
                i += 2;
            }
            else
            {
                tokens[token_count].type = "REDIR_IN";
                tokens[token_count].value = strdup("<");
                i++;
            }
        }
        else if (input[i] == '>')
        {
            if (input[i + 1] == '>')
            {
                tokens[token_count].type = "REDIR_APPEND";
                tokens[token_count].value = strdup(">>");
                i += 2;
            }
            else
            {
                tokens[token_count].type = "REDIR_OUT";
                tokens[token_count].value = strdup(">");
                i++;
            }
        }
        else if (input[i] == '"' || input[i] == '\'')
        {
            char quote = input[i++];
            int start = i;

            while (input[i] && input[i] != quote)
                i++;

            if (!input[i])
            {
                print_exec_error("syntax error", "unclosed quote");
                tokens[token_count].type = "WORD";
                tokens[token_count].value = strndup(&input[start], i - start);
            }
            else
            {
                tokens[token_count].type = "WORD";
                tokens[token_count].value = strndup(&input[start-1], i - start+2);
                i++;
            }
        }
        else
        {
            int start = i;
            
            // Check if this looks like an assignment (contains =)
            int has_equals = 0;
            size_t temp_i = i;
            while (temp_i < strlen(input) && input[temp_i] != ' ' && input[temp_i] != '\t' &&
                   input[temp_i] != '|' && input[temp_i] != '<' && input[temp_i] != '>')
            {
                if (input[temp_i] == '=')
                {
                    has_equals = 1;
                    break;
                }
                temp_i++;
            }
            
            if (has_equals)
            {
                // Handle assignment: read until = and then check for quoted value
                while (input[i] && input[i] != '=')
                    i++;
                
                if (input[i] == '=')
                {
                    i++; // Skip the =
                    
                    // Check if there's a quoted value immediately after =
                    if (input[i] == '"' || input[i] == '\'')
                    {
                        char quote = input[i++];
                        while (input[i] && input[i] != quote)
                            i++;
                        if (input[i]) // Found closing quote
                            i++;
                        // If no closing quote, stop at current position
                    }
                    else
                    {
                        // No quoted value, read until space or special character
                        while (input[i] && input[i] != ' ' && input[i] != '\t' &&
                               input[i] != '|' && input[i] != '<' && input[i] != '>')
                            i++;
                    }
                }
            }
            else
            {
                // Normal word: read until space or special character
                while (input[i] && input[i] != ' ' && input[i] != '\t' &&
                       input[i] != '|' && input[i] != '<' && input[i] != '>')
                    i++;
            }

            tokens[token_count].type = "WORD";
            tokens[token_count].value = strndup(&input[start], i - start);
        }

        if (!tokens[token_count].value)
        {
            j = 0;
            while (j < token_count)
            {
                free(tokens[j].value);
                j++;
            }
            free(tokens);
            return NULL;
        }
        token_count++;
    }

    tokens[token_count].type = NULL;
    tokens[token_count].value = NULL;

    return tokens;
}
