/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:37:09 by rsaueia           #+#    #+#             */
/*   Updated: 2025/01/07 19:04:51 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_ls_fle(t_types *type)
{
	t_types	*tmp;

	tmp = type;
	while (tmp)
	{
		if (ft_strncmp(tmp->cmd, "ls", 2) == 0
			&& tmp->next->next->type == FLE)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

static void	remove_ls_fle(t_types *type)
{
	while (type)
	{
		if (ft_strncmp(type->cmd, "ls", 2) == 0
			&& type->next->next->type == FLE)
		{
			free(type->next->next->cmd);
			type->next->next->cmd = NULL;
			break ;
		}
		type = type->next;
	}
}

static void	remove_redirect_fle(t_types *type)
{
	while (type)
	{
		if ((ft_strcmp(type->cmd, ">") == 0 || ft_strcmp(type->cmd, ">>") == 0)
			&& type->next->type == FLE)
		{
			free(type->next->cmd);
			type->next->cmd = NULL;
			break ;
		}
		type = type->next;
	}
}

static void	finalize_redirection(t_types *type_head, t_types *type_echo)
{
	if (type_echo)
		type_echo->fd[1] = type_head->fd[1];
	if (has_ls_fle(type_head))
		remove_ls_fle(type_head);
	else
		remove_redirect_fle(type_head);
	remove_null_nodes(type_head);
}

int	setup_redirection(t_types *type)
{
	t_types	*type_head;
	t_types	*type_echo;

	type_echo = NULL;
	if (ft_strncmp(type->cmd, "echo", 4) == 0)
		type_echo = type;
	type_head = type;
	while (type)
	{
		if (ft_strcmp(type->cmd, "<") == 0 && type->next)
		{
			if (handle_in(type, type_head) == -1)
				return (-1);
		}
		else if ((ft_strcmp(type->cmd, ">") == 0
				|| ft_strcmp(type->cmd, ">>") == 0) && type->next)
		{
			if (handle_out(type, type_head, ft_strcmp(type->cmd, ">>") == 0))
				return (-1);
		}
		type = type->next;
	}
	finalize_redirection(type_head, type_echo);
	return (0);
}
