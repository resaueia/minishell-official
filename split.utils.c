/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:38:53 by rsaueia           #+#    #+#             */
/*   Updated: 2024/11/15 21:33:31 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    **types_to_char(t_types *list)
{
    t_types    *temp;
    char            **cmds;
    int             i;
    int             count;

    temp = list;
    i = 0;
    count = 0;
    while(temp)
    {
        count++;
        temp = temp->next;
    }
    cmds = (char **)malloc(sizeof(char *) * count + 1);
    if (!cmds)
        return (NULL);
    temp = list;
    while (temp)
    {
        cmds[i] = ft_strdup(temp->cmd);
        temp = temp->next;
        i++;
    }
    cmds[i] = NULL;
    return (cmds);
}

char    **env_to_char(t_envp *env_list)
{
    t_envp    *temp;
    char      **list;
    int             i;
    int             count;

    temp = env_list;
    i = 0;
    count = 0;
    while(temp)
    {
        count++;
        temp = temp->next;
    }
    list = (char **)malloc(sizeof(char *) * count + 1);
    if (!list)
        return (NULL);
    temp = env_list;
    while (temp)
    {
        list[i] = ft_strjoin(temp->key, "=");
        list[i] = ft_strjoin(list[i], temp->value);
        temp = temp->next;
        i++;
    }
    list[i] = NULL;
    return (list);
}

char    **list_to_char(t_init_input *list)
{
    t_init_input    *temp;
    char            **cmds;
    int             i;
    int             count;

    temp = list;
    i = 0;
    count = 0;
    while(temp)
    {
        count++;
        temp = temp->next;
    }
    cmds = (char **)malloc(sizeof(char *) * count + 1);
    if (!cmds)
        return (NULL);
    temp = list;
    while (temp)
    {
        cmds[i] = ft_strdup(temp->string);
        temp = temp->next;
        i++;
    }
    cmds[i] = NULL;
    return (cmds);
}

//talvez alterar para retornar uma t_init_input
t_init_input    *split_commands(char **commands, t_init_input **head, t_init_input **tail)
{
    int             i;
    t_init_input    *split_cmd_list;
    t_init_input    *current_node;

    i = 0;
    while (commands[i])
    {
        split_cmd_list = ft_split(commands[i]);
        current_node = split_cmd_list;
        while (current_node)
        {
            add_to_list(head, tail, current_node->string, current_node->token);
            //printf("string %s | ", current_node->string);
            //printf("token %u\n", current_node->token);
            current_node = current_node->next;
        }
        //free the list (create a function for it);
        i++;
    }
    return(*head);
}