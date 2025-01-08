/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_nodes_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 02:51:42 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/22 15:50:44 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_builtins_non_first(t_types **cmd)
{
	if (ft_strcmp((*cmd)->cmd, "echo") == 0 && (*cmd)->next->cmd)
	{
		*cmd = (*cmd)->next;
		while (*cmd && is_delim((*cmd)->type) == 0)
		{
			(*cmd)->type = ARGS;
			*cmd = (*cmd)->next;
		}
		if (!*cmd)
			return (1);
	}
	return (0);
}

int	handle_builtins_first(t_types **cmd)
{
	if (ft_strcmp((*cmd)->cmd, "echo") == 0 && !(*cmd)->next)
		return (1);
	if (ft_strcmp((*cmd)->cmd, "echo") == 0 && (*cmd)->next)
	{
		*cmd = (*cmd)->next;
		while (*cmd && is_delim((*cmd)->type) == 0)
		{
			(*cmd)->type = ARGS;
			*cmd = (*cmd)->next;
		}
		if (!*cmd)
			return (1);
	}
	return (0);
}

void	handle_redirects(t_types *cmd)
{
	if ((ft_strcmp(cmd->cmd, ">") == 0
			|| ft_strcmp(cmd->cmd, ">>") == 0
			|| ft_strcmp(cmd->cmd, "<") == 0) && cmd->next->cmd)
		cmd->next->type = FLE;
	else if (ft_strcmp(cmd->cmd, "<<") == 0 && cmd->next->cmd)
		cmd->next->type = ENDOF;
}
