/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:38:53 by rsaueia           #+#    #+#             */
/*   Updated: 2024/10/11 19:02:33 by rsaueia          ###   ########.fr       */
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

void    process_input(char *input)
{
    t_init_input    *cmd_list;
    t_init_input    *space_list;
    char            **cmds;
    
    cmd_list = delim_split(input);
    cmds = list_to_char(cmd_list);
    // This is not working for now as my ft_split function converts a char *
    // into a list (not a char **). I believe it's a somewhat easy fix, but I fail
    // to understand this particular sequence's logic now. Need to check.
    //space_list = ft_split(cmds);
}
