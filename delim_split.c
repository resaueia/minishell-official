/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delim_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:40:07 by rsaueia           #+#    #+#             */
/*   Updated: 2024/12/18 19:50:36 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int     is_pipe(char c)
{
    return (c == '|');
}

t_token         get_token(char *c)
{
    int i;

    i = 0;
    if (c[i] == '|' && c[i + 1] != '|')
        return (PIPE);
    else if (c[i] == '>' && c[i + 1] == '>')
        return (APPEND);
    else if (c[i] == '<' && c[i + 1] == '<')
        return (HDOC);
    else if (c[i] == '>' && c[i + 1] != '>')
        return (OUT);
    else if (c[i] == '<' && c[i + 1] != '<')
        return (IN);
    else if (is_builtin(c))
        return (BUILTIN);
    else
        return (WORD);
}

void    add_to_list(t_init_input **head, t_init_input **tail, char *substr, t_token token)
{
    t_init_input    *new_node;

    new_node = add_node(substr, token);
    if (!new_node)
        return ;
    if (!*head)
    {
        *head = new_node;
        *tail = new_node;
    }
    else
    {
        (*tail)->next = new_node;
        new_node->prev = *tail;
        *tail = new_node;
    }
}

static void extract_token(t_init_input **head, t_init_input **tail, char *s, int start, int end)
{
    char *token;

    token = custom_dup(s, start, end); // Extrai o token entre start e end
    add_to_list(head, tail, token, get_token(token)); // Adiciona o token à lista
    free(token);
}

/* Function: extract_token
 * Extracts a token from the string `s` between indices `start` and `end`,
 * determines its type, and adds it to the linked list.
 */

static void process_pipe_token(t_init_input **head, t_init_input **tail, char *s, size_t i)
{
    extract_token(head, tail, s, i, i + 1); // Adiciona o delimitador como token
}

/* Function: process_pipe_token
 * Handles the processing of pipe or operator tokens by extracting
 * and adding them to the linked list.
 */


static void process_current_character(t_init_input **head, t_init_input **tail,
                                      char *s, int *start, size_t i)
{
    if (is_pipe(s[i]) || s[i + 1] == '\0')
    {
        if (*start >= 0) // Extrai um token WORD
        {
            extract_token(head, tail, s, *start, i + (s[i + 1] == '\0'));
            *start = -1;
        }
        if (is_pipe(s[i])) // Processa delimitadores
            process_pipe_token(head, tail, s, i);
    }
}

/* Function: process_current_character
 * Processes the current character in the input string `s`. If it is a
 * pipe or the end of the string, it handles token extraction and delimiter
 * processing. Resets the start index after processing a WORD token.
 */

t_init_input *delim_split(char *s)
{
    t_init_input *head;
    t_init_input *tail;
    size_t i;
    int start;

    head = NULL;
    tail = NULL;
    i = 0;
    start = -1;

    if (!s)
        return (NULL);

    while (s[i])
    {
        if (!is_pipe(s[i]) && start < 0)
            start = i; // Marca o início de um token
        else
            process_current_character(&head, &tail, s, &start, i);
        i++;
    }
    return (head);
}

/* Function: delim_split
 * Splits the input string `s` into tokens based on delimiters like `|`, `>`,
 * and `<`. Adds each token to a linked list and returns the head of the list.
 */
