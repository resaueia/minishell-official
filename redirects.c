/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:37:09 by rsaueia           #+#    #+#             */
/*   Updated: 2024/11/04 17:33:35 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int setup_redirection(t_init_input *args_list)
{
    t_init_input *temp;
    int temp_fd;

    temp = args_list;
    while (temp)
    {
        if (ft_strcmp(temp->string, ">") == 0 && temp->next)
        {
            if (access(temp->next->string, W_OK) == -1 && errno != ENOENT)
            {
                perror("No write permission for output redirect");
                return (-1);
            }

            temp_fd = open(temp->next->string, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (temp_fd == -1)
            {
                perror("Error opening fd for output redirect");
                return (-1);
            }
            temp->fd_out = temp_fd;
        }
        else if (ft_strcmp(temp->string, ">>") == 0 && temp->next)
        {
            if (access(temp->next->string, W_OK) == -1 && errno != ENOENT)
            {
                perror("No write permission for append redirect");
                return (-1);
            }

            temp_fd = open(temp->next->string, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (temp_fd == -1)
            {
                perror("Error opening fd for append redirect");
                return (-1);
            }
            temp->fd_out = temp_fd;
        }
        else if (ft_strcmp(temp->string, "<") == 0 && temp->next)
        {
            if (access(temp->next->string, R_OK) == -1)
            {
                perror("No read permission for input redirect");
                return (-1);
            }

            temp_fd = open(temp->next->string, O_RDONLY);
            if (temp_fd == -1)
            {
                perror("Error opening fd for input redirect");
                return (-1);
            }
            temp->fd_in = temp_fd;
        }
        temp = temp->next;
    }
    return (0);
}