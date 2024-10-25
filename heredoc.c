/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:55:24 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/10/25 19:56:34 by jparnahy         ###   ########.fr       */
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
    char            *line;
    int             pipe_fd[2];

    temp = input_list;
    while (temp)
    {
        if (ft_strcmp(temp->string, "<<") == 0 && temp->next)
        {
            delim = temp->next->string;
            break ;
        }
        temp = temp->next;
    }
    if (!delim)
    {
        perror("Heredoc delimiter not found\n");
        return ;
    }
    if (pipe(pipe_fd) == -1)
    {
        perror("Error");
        return ;
    }
    while (1)
    {
        line = readline("heredoc> ");
        if (!line)
        {
            perror("Error");
            break ;
        }
        if (ft_strcmp(line, delim) == 0)
        {
            free(line);
            break ;
        }
        write(pipe_fd[1], line, ft_strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
    close(pipe_fd[1]);
    if (input_list->fd_in != 0)
        close(input_list->fd_in);
    input_list->fd_in = pipe_fd[0];
    //dup2(pipe_fd[0], STDIN_FILENO);
    //close(pipe_fd[0]);
    /*After writing stuff on the pipe, I close its writing end, use
    the dup2 function to set its reading end to the standard input (which used to be the keyboard)
    and then proceed to close the actual read end of the pipe, hence finishing the whole process.*/

    /*Not sure if the execution part of this should be written here already or elsewhere.*/
}

