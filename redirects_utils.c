/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 22:12:24 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/02 20:30:28 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void remove_node(t_types **head)
{
    printf("\n----\non the remove_node\n");
    t_types *node;
    t_types *temp_node;

    node = *head;
    while (node)
    {
        printf("node->cmd: [%s]\n", node->cmd);
        printf("node->type: [%u]\n", node->type);
        temp_node = node->next; //save the next node
        if (node->type == 4 || node->type == 7 || node->type == 77) //check if the node is redirects
        {
            if (node->prev)
                node->prev->next = node->next;
            if (node->next)
                node->next->prev = node->prev;
            if (node == *head)
                *head = node->next;
            free(node->cmd);
            free(node); //to node
            node = NULL;
        }
        node = temp_node;
    }
}

/*void remove_node(t_types *node)
{
    //printf("\n----\non the remove_node\n");
    t_types *head;
    t_types *temp_node;

    head = node;
    if (!node)
        return ;
    while (node)
    {
        temp_node = node->next; //save the next node
        if (node->type == 4 || node->type == 7 || node->type == 77) //check if the node is redirects
        {
            if (node == head) //if is the frs node, update the head
            {
                head = node->next;
                if (head)
                    head->prev = NULL;
            }
            else //update the next and prev nodes
            {
                if (node->prev)
                    node->prev->next = node->next;
                if (node->next)
                    node->next->prev = node->prev;
            }
            free(node); //to node
            node = NULL;
        } 
        node = temp_node;
    }
    node = head;
}*/