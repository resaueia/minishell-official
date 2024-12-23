/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_checkers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 23:43:15 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/22 21:12:29 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	rmv_db_qts(char **str)
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

static void	rmv_sg_qts(char **str)
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

static void	remove_backslashes(char *str)
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

static char	*expander_or_not(char *cmd, t_envp *env_list, int exit_status)
{
	printf("expander_or_not\n");
	printf("cmd: %s\n", cmd);
	int	i;

	i = 0;
	remove_backslashes(cmd);
	while (cmd[i])
	{
		if (cmd[i] == '\"')
		{
			rmv_db_qts(&cmd);
			break ;
		}
		else if (cmd[i] == '\'')
		{
			rmv_sg_qts(&cmd);
			return (cmd);
		}
		i++;
	}
	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '$')
			cmd = to_expander(cmd, i, env_list, exit_status);
	}
	return (cmd);
}

void	lets_expander(t_types *types, t_envp *env_list, int exit_status)
{
	t_envp	*env;

	env = env_list;
	while (types)
	{
		if (has_dol(types->cmd))
			types->cmd = expander_or_not(types->cmd, env, exit_status);
		types = types->next;
	}
	env = NULL;
}
