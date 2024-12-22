/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:37:09 by rsaueia           #+#    #+#             */
/*   Updated: 2024/12/22 16:00:46 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	finalize_redirection(t_types *type_head, t_types *type_echo)
{
	t_types	*type;

	type = type_head;
	if (type_echo)
		type_echo->fd[1] = type_head->fd[1];
	while (type)
	{
		if (ft_strncmp(type->cmd, "ls", 2) == 0
			&& type->next->next->type == FLE)
		{
			type->next->next->cmd = NULL;
			break ;
		}
		if (ft_strcmp(type->cmd, ">") == 0 && type->next->type == FLE)
		{
			type->next->cmd = NULL;
			break ;
		}
		if (ft_strcmp(type->cmd, ">>") == 0 && type->next->type == FLE)
		{
			type->next->cmd = NULL;
			break ;
		}
		type = type->next;
	}
}

int	setup_redirection(t_types *type)
{
	t_types	*type_head;
	t_types	*type_echo;

	type_head = type;
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
