/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:55:24 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/10/21 16:22:20 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*identify heredoc:
if heredoc, get a function to process it. how?
start capturing all input lines from the user until you come across
the initial delimiter.
write said lines in a pipe - pipe needs to get redirectioned as an entry for the cmd;
finally, those lines will get written on the terminal once said cmd is executed.
*/

/*how to create a pipe:
int pipe_fd[2];
if (pipe(pipe_fd) == -1)
{
    perror("pipe");
    exit(EXIT_FAILURE);
}
*/

int     is_heredoc(t_init_input *input_list)
{
    t_init_input    *temp;

    temp = input_list;
    while(temp)
    {
        if (ft_strcmp(temp->string, "<<") == 0 && temp->next->string)
            return (1);
        temp = temp->next;
    }
    return (0);
}

void    tackle_heredoc(t_init_input *input_list)
{
    t_init_input    *temp;
    char            *delim;
    int             pipe_fd[2];

    temp = input_list;
    while (temp)
    {
        if (ft_strcmp(temp->string, "<<") == 0 && temp->next)
        {
            delim = temp->next->string;
        }
        temp = temp->next;
    }
    if (!delim)
    {
        perror("Heredoc delimiter not found\n");
        return ;
    }
    // HERE WRITE PIPE AND THINK ABOUT RESETTING THE CMDS
}

