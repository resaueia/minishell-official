/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 22:12:24 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/20 15:10:21 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void remove_node(t_types **head)
{
    t_types *node;
    t_types *temp_node;

    node = *head;
    while (node)
    {
        temp_node = node->next;
        if (node->type == 4 || node->type == 7 || node->type == 77)
        {
            if (node->prev)
                node->prev->next = node->next;
            if (node->next)
                node->next->prev = node->prev;
            if (node == *head)
                *head = node->next;
            free(node->cmd);
            free(node);
            node = NULL;
        } 
        node = temp_node;
    }
}