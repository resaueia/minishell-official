/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:17:17 by rsaueia           #+#    #+#             */
/*   Updated: 2024/11/13 18:15:09 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    exec_command(t_init_input *cmd, char **envp)
{
    pid_t   pid;
    int     status;

    pid = fork();
    if (pid == -1)
    {
        perror("Fork in exec function has failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        if (cmd->fd_in != STDIN_FILENO)
        {
            if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
            {
                perror("dup2 fd_in has failed in exec function");
                exit(EXIT_FAILURE);
            }
            close(cmd->fd_in);
        }
        if (cmd->fd_out != STDOUT_FILENO)
        {
            if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
            {
                perror("dup2 fd_out has failed in exec function");
                exit(EXIT_FAILURE);
            }
            close(cmd->fd_out);
        }
        if (execve(cmd->args[0], cmd->args, envp) == -1)
        {
            perror("Execution has failed");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            printf("Child has exited with status: %d\n", WEXITSTATUS(status));
    }
}