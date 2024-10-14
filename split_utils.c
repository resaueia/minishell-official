/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:38:53 by rsaueia           #+#    #+#             */
/*   Updated: 2024/10/14 19:03:47 by rsaueia-         ###   ########.fr       */
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
    cmds[i] = NULL;
    return (cmds);
}

/*void    split_commands(char **commands)
{
    t_init_input    *head;
    t_init_input    *tail;
    char            **split_cmd;
    int             i;
    int             j;

    i = 0;
    while (commands[i])
    {
        split_cmd[i] = ft_split(commands[i]);
        j = 0;
        while (split_cmd[j])
        {
            add_to_list()
        }
    }
    
}*/

void    process_input(char *input)
{
    t_init_input    *cmd_list;
    t_init_input    *args_list;
    char            **cmds;
    char            **args;
    
    cmd_list = delim_split(input);
    cmds = list_to_char(cmd_list);
    args_list = ft_split(input);
    args = list_to_char(args_list);
    
    // This is not working for now as my ft_split function converts a char *
    // into a list (not a char **). I believe it's a somewhat easy fix, but I fail
    // to understand this particular sequence's logic now. Need to check.
    //space_list = ft_split(cmds);
}
