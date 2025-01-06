/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_checkers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 23:43:15 by jparnahy          #+#    #+#             */
/*   Updated: 2025/01/06 17:47:30 by jparnahy         ###   ########.fr       */
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

static char	*its_time_to_expander(char *cmd, t_envp *env_list, int exit_status)
{
	int	len;
	int	i;
	
	i = 0;
	len = ft_strlen(cmd);
	while (i < len)
	{
		if (cmd[i] == '$')
		{
			cmd = to_expander(cmd, i, env_list, exit_status);
			len = ft_strlen(cmd);  // Atualizar tamanho após expansão
			i = -1;                // Reiniciar o loop para nova análise
		}
		i++;
	}
	return (cmd);
}

static char	*expander_or_not(char *cmd, t_envp *env_list, int exit_status)
{
	int	i;

	i = 0;
	remove_backslashes(cmd);
	while (cmd[i])
	{
		if (cmd[i] == '\'')
		{
			rmv_sg_qts(&cmd);
			return (cmd);
		}
		if (cmd[i] == '\"')
		{
			rmv_db_qts(&cmd);
			break ;
		}
		i++;
	}
	cmd = its_time_to_expander(cmd, env_list, exit_status);
	return (cmd);
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
