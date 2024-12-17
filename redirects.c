/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:37:09 by rsaueia           #+#    #+#             */
/*   Updated: 2024/12/17 17:57:29 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int setup_redirection(t_init_input *args_list, t_types *type)
{
    (void)type;
    (void)args_list;
    t_types         *type_head;
    t_types         *type_echo;
    int             temp_fd;

    if (ft_strncmp(type->cmd, "echo", 4) == 0)
            type_echo = type;
    type_head = type;
    while (type)
    {
        if (ft_strcmp(type->cmd, "<") == 0 && type->next)
        {
            if (access(type->next->cmd, R_OK) == -1)
            {
                fprintf(stderr, "minishell: no such file or directory: %s\n", type->cmd);
                return (-1);
            }
            if (type->fd[0] != STDIN_FILENO)
                close(type->fd[0]);
            temp_fd = open(type->next->cmd, O_RDONLY);
            if (temp_fd == -1)
            {
                perror("Error opening fd for input redirect");
                return (-1);
            }
            type->fd[0] = temp_fd;
            type->next->fd[0] = temp_fd;
        }
        else if (ft_strcmp(type->cmd, ">") == 0 && type->next)
        {
            if (access(type->next->cmd, W_OK) == -1 && errno != ENOENT)
            {
                perror("No write permission for output redirect");
                return (-1);
            }
            if (type->fd[1] != STDOUT_FILENO)
                close(type->fd[1]);
            temp_fd = open(type->next->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            printf("temp_fd: [%i]\n", temp_fd);
            if (temp_fd == -1)
            {
                perror("Error opening fd for output redirect");
                return (-1);
            }
            type->fd[1] = temp_fd;
        }
        else if (ft_strcmp(type->cmd, ">>") == 0 && type->next)
        {
            if (access(type->next->cmd, W_OK) == -1 && errno != ENOENT)
            {
                perror("No write permission for append redirect");
                return (-1);
            }
            if (type->fd[1] != STDOUT_FILENO)
                close(type->fd[1]);
            temp_fd = open(type->next->cmd, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (temp_fd == -1)
            {
                perror("Error opening fd for append redirect");
                return (-1);
            }
            type->fd[1] = temp_fd;
        }
        if (ft_strncmp(type_head->cmd, "echo", 4) == 0)
                type_echo->fd[1] = temp_fd;
        type = type->next;
    }
    type = type_head;
    return (0);
}