/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delim_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:40:07 by rsaueia           #+#    #+#             */
/*   Updated: 2024/11/05 23:31:27 by jparnahy         ###   ########.fr       */
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

t_init_input    *delim_split(char *s)
{
    t_init_input    *head;
    t_init_input    *tail;
    char            *substr;
    size_t          i;
    int             start_index;
    t_token         token;
    
    head = NULL;
    tail = NULL;
    i = 0;
    start_index = -1;
    if (!s)
        return (NULL);
    while (s[i])
    {
        if (!is_pipe(s[i]) && start_index < 0)
            start_index = i;
        else if (is_pipe(s[i]) || s[i + 1] == '\0')
        {
            if(start_index >= 0) //This tells us there's a substring to be added
            {
                substr = custom_dup(s, start_index, i + (s[i + 1] == '\0')); //This makes sure the last character gets picked up on
                add_to_list(&head, &tail, substr, WORD);
                free(substr);
                start_index = -1;
            }
            if (is_pipe(s[i]))
            {
                if ((s[i] == '>' || s[i] == '<') && s[i] == s[i + 1]) // Checks for double opperand
                {
                    substr = custom_dup(s, i, i + 2);
                    token = get_token(substr);
                    i++;
                }
                else
                {
                    substr = custom_dup(s, i, i + 1);
                    token = get_token(substr);
                }
                add_to_list(&head, &tail, substr, token);
                free(substr);
            }
        }
        i++;
    }
    return (head);
}