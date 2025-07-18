#include "../minishell.h"
#include <stdlib.h>
#include <string.h>

void gc_init(t_garbage_collector *gc)
{
    if (!gc)
        return;
    gc->allocations = NULL;
    gc->count = 0;
}

void *gc_malloc(t_garbage_collector *gc, size_t size)
{
    t_gc_node *node;
    void *ptr;

    if (!gc)
        return NULL;
    
    ptr = malloc(size);
    if (!ptr)
        return NULL;
    
    node = malloc(sizeof(t_gc_node));
    if (!node)
    {
        free(ptr);
        return NULL;
    }
    
    node->ptr = ptr;
    node->next = gc->allocations;
    gc->allocations = node;
    gc->count++;
    
    return ptr;
}

void *gc_strdup(t_garbage_collector *gc, const char *str)
{
    char *dup;
    size_t len;

    if (!gc || !str)
        return NULL;
    
    len = strlen(str);
    dup = gc_malloc(gc, len + 1);
    if (!dup)
        return NULL;
    
    strcpy(dup, str);
    return dup;
}

void gc_free_ptr(t_garbage_collector *gc, void *ptr)
{
    t_gc_node *current;
    t_gc_node *prev;

    if (!gc || !ptr)
        return;
    
    current = gc->allocations;
    prev = NULL;
    
    while (current)
    {
        if (current->ptr == ptr)
        {
            if (prev)
                prev->next = current->next;
            else
                gc->allocations = current->next;
            
            free(current->ptr);
            free(current);
            gc->count--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void gc_free_all(t_garbage_collector *gc)
{
    t_gc_node *current;
    t_gc_node *next;

    if (!gc)
        return;
    
    current = gc->allocations;
    while (current)
    {
        next = current->next;
        if (current->ptr)
        {
            // Check if the pointer is still valid before freeing
            // This is a simple check - in a more robust implementation,
            // you might want to track freed pointers
            free(current->ptr);
        }
        free(current);
        current = next;
    }
    
    gc->allocations = NULL;
    gc->count = 0;
} 