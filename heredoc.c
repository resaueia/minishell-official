/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:55:24 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/11/19 12:13:05 by jparnahy         ###   ########.fr       */
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
    int             heredoc_fd;

    temp = input_list;
    while(temp)
    {
        if (ft_strcmp(temp->string, "<<") == 0 && temp->next->string)
        {
            heredoc_fd = tackle_heredoc(temp->next->string);
            if (heredoc_fd == -1)
            {
                perror ("Error setting up heredoc");
                return (-1);
            }
            temp->fd_in = heredoc_fd;
            return (1);
        }
        temp = temp->next;
    }
    return (0);
}

int    tackle_heredoc(char *delim)
{
    char            *line;
    int             pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        perror("Error creating heredoc pipe\n");
        return (-1);
    }
    while (1)
    {
        line = readline("heredoc> ");
        if (!line)
        {
            perror("Error reading line\n");
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
    //if (input_list->fd_in != STDIN_FILENO)
      //  close(input_list->fd_in);
    
    return (pipe_fd[0]);
    //dup2(pipe_fd[0], STDIN_FILENO);
    //close(pipe_fd[0]);
    /*After writing stuff on the pipe, I close its writing end, use
    the dup2 function to set its reading end to the standard input (which used to be the keyboard)
    and then proceed to close the actual read end of the pipe, hence finishing the whole process.*/

    //Not sure if the execution part of this should be written here already or elsewhere.
}

