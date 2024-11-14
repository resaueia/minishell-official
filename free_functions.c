/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:46:31 by rsaueia           #+#    #+#             */
/*   Updated: 2024/11/13 18:15:49 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_list(t_init_input *list)
{
	t_init_input *temp;

	while (list)
	{
		temp = list;
		list = list->next;
		if (temp->string)
			free(temp->string);
		if (temp->args)
			free(temp->args);
		free(temp);
	}
}

void    free_env_list(t_envp *env_list)
{
    t_envp  *temp;
    
    while (temp)
    {
        temp = env_list;
        env_list = env_list->next;
        if (temp->key)
            free(temp->key);
        if (temp->value)
            free(temp->value);
        free(temp);
    }
}

void    close_fds(t_init_input *cmd_list)
{
    t_init_input    *temp;

    temp = cmd_list;
    while (temp)
    {
        if (temp->fd_in != STDIN_FILENO)
            close(temp->fd_in);
        if (temp->fd_out != STDOUT_FILENO)
            close(temp->fd_out);
        temp = temp->next;
    }
}