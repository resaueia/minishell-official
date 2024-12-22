/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_out.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 04:41:24 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/22 04:42:00 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int handle_truncate_redirect(t_types *type, t_types *type_head)
{
    int temp_fd;
    
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
		last_status(1);
		return (-1);
	}
	type->fd[1] = temp_fd;
	type_head->fd[1] = temp_fd;
	if (type->fd[1] != STDIN_FILENO)
		type->next->fd[0] = type->fd[1];
    return (0);
}

static int handle_append_redirect(t_types *type, t_types *type_head)
{
    int temp_fd;
	
    if (access(type->next->cmd, W_OK) == -1 && errno != ENOENT)
	{
		perror("No write permission for output redirect");
		last_status(1);
		return (-1);
	}
	if (type->fd[1] != STDOUT_FILENO)
		close(type->fd[1]);
	temp_fd = open(type->next->cmd, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (temp_fd == -1)
	{
		perror("Error opening fd for append redirect");
		last_status(1);
		return (-1);
	}
	type->fd[1] = temp_fd;
	type_head->fd[1] = temp_fd;
	if (type->fd[1] != STDIN_FILENO)
		type->next->fd[0] = type->fd[1];
    return (0);
}

int handle_out(t_types *type, t_types *type_head, int is_append)
{
    if (is_append)
        return handle_append_redirect(type, type_head);
    else
        return handle_truncate_redirect(type, type_head);
}