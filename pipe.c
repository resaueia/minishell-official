/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:39:20 by rsaueia           #+#    #+#             */
/*   Updated: 2024/11/19 14:45:08 by rsaueia          ###   ########.fr       */
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

void execute_pipeline(t_init_input **pipe_cmds, char **envp)
{
    int pipe_fd[2];
    pid_t pid;
    int status;
    int input_fd = STDIN_FILENO;

    for (int i = 0; pipe_cmds[i]; i++)
    {
        pipe(pipe_fd);
        pid = fork();
        if (pid == 0)
        {
            dup2(input_fd, STDIN_FILENO);
            if (pipe_cmds[i + 1] != NULL)
                dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[0]);
            exec_command(pipe_cmds[i], envp);
            exit(EXIT_FAILURE);
        }
        else
        {
            waitpid(pid, &status, 0);
            close(pipe_fd[1]);
            input_fd = pipe_fd[0];
        }
    }
}
