/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delim_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:40:07 by rsaueia           #+#    #+#             */
/*   Updated: 2024/09/10 17:35:02 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     is_delim(char c)
{
    return (c == '|' || c == '>' || c == '<');
}

void    add_to_list(t_init_input **head, t_init_input **tail, char *substr)
{
    t_init_input    *new_node;

    new_node = add_node(substr);
    if (!new_node)
        return (NULL);
    else
    {
        (*tail)->next = new_node;
        new_node->prev = *tail;
    }
    *tail = new_node;
}

t_init_input    *delim_split(char *s)
{
    t_init_input    *head;
    t_init_input    *tail;
    char            *substr;
    size_t          i;
    int             start_index;
    
    i = 0;
    start_index = -1;
    if (!s)
        return (NULL);
    while (s[i])
    {
        if (!is_delimiter(s[i]) && start_index < 0)
            start_index = i;
        else if (is_delimiter(s[i]) || s[i + 1] == '\0')
        {
            if(start_index >= 0) //This tells us there's a substring to be added
            {
                substr = custom_dup(s, start_index, i + (s[i + 1] == '\0'));
                add_to_list(&head, &tail, substr);
                free(substr);
                start_index = -1;
            }
            if (is_delimiter(s[i]))
            {
                if ((s[i] == '>' || s[i] == '<') && s[i] == s[i + 1]) // Checks for douple opperand
                {
                    substr = custom_dup(s, i, i + 2);
                    i++;
                }
                else
                    substr = custom_dup(s, i, i + 1);
                add_to_list(&head, &tail, substr);
                free(substr);
            }
        }
        i++;
    }
    return (head);
}