/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_checkers_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 20:55:09 by rsaueia-          #+#    #+#             */
/*   Updated: 2025/01/07 21:01:41 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*its_time_to_expander(char *cmd, t_envp *env_list, int exit_status)
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
			len = ft_strlen(cmd);
			i = -1;
		}
		i++;
	}
	return (cmd);
}

char	*expander_or_not(char *cmd, t_envp *env_list, int exit_status)
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
