/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:38:53 by rsaueia           #+#    #+#             */
/*   Updated: 2024/10/09 18:05:15 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
    cmds[i] = '\0';
    return (cmds);
}

void    split_per_spaces(char **cmds)
{
    int     i;
    char    **split_cmd;

    i = 0;
    while (cmds)
    {
        split_cmd = ft_split(cmds[i]);
        i++;
    }
}

void    process_input(char *input)
{
    t_init_input    *cmd_list;
    char            **cmds;
    
    cmd_list = delim_split(input);
    cmds = list_to_char(cmd_list);
    split_per_spaces(cmds);
}