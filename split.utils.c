/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:38:53 by rsaueia           #+#    #+#             */
/*   Updated: 2024/10/24 19:12:42 by rsaueia          ###   ########.fr       */
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

//talvez alterar para retornar uma t_init_input
void    split_commands(char **commands, t_init_input **head, t_init_input **tail)
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
}

void    **process_input(t_init_input *cmd_list, char **cmds)
{
    t_init_input    *args_list;
    t_init_input    *args_tail;
    //char            *args;  Ideia que o allan deu de guardar o input inteiro
    
    args_list = NULL;
    args_tail = NULL;
    split_commands(cmds, &args_list, &args_tail); 
    //print_the_stack(args_list);
    tackle_heredoc(args_list);
    if (setup_redirection(args_list) == -1)
    {
        perror("Error whule setting up redirection\n");
        return ;
    }

    //para enviar para execução
    //has pipe
    //has redirect
    //has built-in

    free_list(cmd_list);
    free_list(args_list);

    // free the cmds array, free the cmds list and free the args list?
}