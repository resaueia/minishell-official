/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:39:20 by rsaueia           #+#    #+#             */
/*   Updated: 2024/11/13 16:54:10 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     has_pipe(t_init_input *args_list)
{
    t_init_input    *temp;

    temp = args_list;
    while (temp)
    {
        if (temp->token == PIPE)
            return (1);
        temp = temp->next;
    }
    return (0);
}

void    split_by_pipes(t_init_input *args_list, t_init_input **cmds)
{
    t_init_input    *temp;
    int             i;

    temp = args_list;
    i = 0;
    cmds[i] = temp;
    while (temp)
    {
        if (temp->token == PIPE)
        {
            temp->string = NULL;
            i++;
            cmds[i] = temp->next;
        }
        temp = temp->next;
    }
    cmds[i + 1] = NULL;
}

void    execute_pipeline(t_init_input **cmds, char **envp)
{
    int     pipe_fd[2];
    int     input_fd;
    pid_t   pid;
    int     i;
    
    input_fd = STDIN_FILENO;
    i = 0;
    while (cmds[i])
    {
        if (cmds[i + 1] != NULL)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("Pipe has failed!");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            pipe_fd[0] = STDIN_FILENO;
            pipe_fd[1] = STDOUT_FILENO;
        }
        pid = fork();
        if (pid == -1)
        {
            perror("Fork has failed!");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            if (input_fd != STDIN_FILENO)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            if (cmds[i + 1] != NULL)
            {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
            exec_command(cmds[i], envp);
            perror("Execution has failed.");
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(NULL);
            close(pipe_fd[1]);
            input_fd = pipe_fd[0];
        }
        i++;
    }
}