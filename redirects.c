/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:37:09 by rsaueia           #+#    #+#             */
/*   Updated: 2024/10/24 19:00:46 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     setup_redirection(t_init_input *args_list)
{
    t_init_input    *temp;
    int             fd;
    
    temp = args_list;
    while (temp)
    {
        if (ft_strcmp(temp->string, ">") == 0 && temp->next)
        {
            fd = open(temp->next->string, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                perror("Error opening fd\n");
                return (-1);
            }
            temp->fd_out = fd;
        }
        else if (ft_strcmp(temp->string, ">>") == 0 && temp->next)
        {
            fd = open(temp->next->string, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
            {
                perror("Error opening fd\n");
                return (-1);
            }
            temp->fd_out = fd;
        }
        else if (ft_strcmp(temp->string, "<") == 0 && temp->next)
        {
            fd = open(temp->next->string, O_RDONLY);
            if (fd == -1)
            {
                perror ("Error opening fd\n");
                return (-1);
            }
            temp->fd_in = fd;
        }
        else if (ft_strcmp(temp->string, "<<") == 0 && temp->next->string)
        {
            fd = tackle_heredoc(temp->next->string);
            if (fd == -1)
            {
                perror("Error setting up heredoc");
                return (-1);
            }
            temp->fd_in = fd;
        }
        temp = temp->next;
    }
    return (0);
}