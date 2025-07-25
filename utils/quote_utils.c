#include "../minishell.h"

static void push(struct s_node** top_ref, char new_data)
{
    struct s_node* new_node = malloc(sizeof(struct s_node));
    new_node->data = new_data;
    new_node->next = (*top_ref);
    (*top_ref) = new_node;
}

static char pop(struct s_node** top_ref)
{
    if (*top_ref == NULL)
        return '\0';
    struct s_node* temp = *top_ref;
    char data = temp->data;
    *top_ref = (*top_ref)->next;
    free(temp);
    return data;
}

static char peek(struct s_node* stack)
{
    if (stack == NULL)
        return '\0';
    return stack->data;
}

int verify_quotes(const char *exp)
{
    int i = 0;
    struct s_node* stack = NULL;
    while (exp[i])
    {
        if (exp[i] == '"' || exp[i] == '\'')
        {
            if (stack == NULL || peek(stack) != exp[i])
                push(&stack, exp[i]);
            else if (peek(stack) == exp[i])
                pop(&stack);
        }
        i++;
    }
    if (stack == NULL)
        return 1;
    else
        return 0;
} 