/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:37:09 by rsaueia           #+#    #+#             */
/*   Updated: 2024/12/20 17:54:29 by rsaueia          ###   ########.fr       */
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
                ft_putstr_fd("minishell: no such file or directory:", 2);
                last_status(1);
                return (-1);
            }
            if (type->fd[0] != STDIN_FILENO)
                close(type->fd[0]);
            temp_fd = open(type->next->cmd, O_RDONLY);
            if (temp_fd == -1)
            {
                perror("Error opening fd for input redirect");
                last_status(1);
                return (-1);
            }
            type->fd[0] = temp_fd;
            type_head->fd[0] = temp_fd;
            if (type->fd[0] != STDIN_FILENO)
                type->next->fd[1] = type->fd[0];
        }
        else if (ft_strcmp(type->cmd, ">") == 0 && type->next)
        {
            if (access(type->next->cmd, W_OK) == -1 && errno != ENOENT)
            {
                perror("No write permission for output redirect");
                last_status(1);
                return (-1);
            }
            if (type->fd[1] != STDOUT_FILENO)
                close(type->fd[1]);
            temp_fd = open(type->next->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (temp_fd == -1)
            {
                perror("Error opening fd for output redirect");
                return (-1);
            }
            type->fd[1] = temp_fd;
            type_head->fd[1] = temp_fd;
            if (type->fd[1] != STDIN_FILENO)
                type->next->fd[0] = type->fd[1];
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
            type_head->fd[1] = temp_fd;
            if (type->fd[1] != STDIN_FILENO)
                type->next->fd[0] = type->fd[1];
        }
        if (ft_strncmp(type_head->cmd, "echo", 4) == 0)
                type_echo->fd[1] = temp_fd;
        type = type->next;
    }
    type = type_head;
    while (type)
    {
        if (ft_strncmp(type->cmd, "ls", 2) == 0 && type->next->next->type == FLE)
        {
            type->next->next->cmd = NULL;
            break;
        }
        if (ft_strcmp(type->cmd, ">") == 0 && type->next->type == FLE)
        {
            type->next->cmd = NULL;
            break;
        }
        if (ft_strcmp(type->cmd, ">>") == 0 && type->next->type == FLE)
        {
            type->next->cmd = NULL;
            break;
        }
        type = type->next;
    }
    type = type_head;
    return (0);
}