/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_checkers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 23:43:15 by jparnahy          #+#    #+#             */
/*   Updated: 2025/01/07 21:09:59 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rmv_db_qts(char **str)
{
	char	*src;
	char	*dst;

	src = *str;
	dst = *str;
	while (*src)
	{
		if (*src == '$')
		{
			if (*(src + 1) == '\"')
				src++;
		}
		if (*src != '\"')
			*dst++ = *src;
		src++;
	}
	*dst = '\0';
}

void	rmv_sg_qts(char **str)
{
	char	*src;
	char	*dst;

	src = *str;
	dst = *str;
	while (*src)
	{
		if (*src == '$')
		{
			if (*(src + 1) == '\'')
				src++;
		}
		if (*src != '\'')
			*dst++ = *src;
		src++;
	}
	*dst = '\0';
}

void	remove_backslashes(char *str)
{
	char	*src;
	char	*dst;

	src = str;
	dst = str;
	while (*src)
	{
		if (*src != '\\')
			*dst++ = *src;
		src++;
	}
	*dst = '\0';
}

void	lets_expander(t_types *types, t_envp *env_list, int exit_status)
{
	t_envp	*env;
	t_types	*temp;

	temp = types;
	env = env_list;
	while (types)
	{
		if (has_dol(types->cmd) && types->type != 99)
			types->cmd = expander_or_not(types->cmd, env, exit_status);
		types = types->next;
		temp = temp->next;
	}
	env = NULL;
}
