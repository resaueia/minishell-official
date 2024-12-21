/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:28:38 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/21 20:40:22 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	args_of_cmds(t_types *cmd)
{
	t_types	*head;
	int		node_ref;

	head = cmd;
	node_ref = 1;
	while (cmd)
	{
		if (node_ref == 1)
		{
			if (is_redirects(cmd->type))
			{
				if ((ft_strcmp(cmd->cmd, ">") == 0 || 
						ft_strcmp(cmd->cmd, ">>") == 0 ||
						ft_strcmp(cmd->cmd, "<") == 0) && cmd->next->cmd)
					cmd->next->type = FLE;
				else if (ft_strcmp(cmd->cmd, "<<") == 0 && cmd->next->cmd)
					cmd->next->type = ENDOF;
			}
			else if (is_builtin(cmd->cmd) == 1)
			{
				if (ft_strcmp(cmd->cmd, "echo") == 0 && !cmd->next)
					break ;
				if (ft_strcmp(cmd->cmd, "echo") == 0 && cmd->next->cmd)
				{
					cmd = cmd->next;
					while (cmd && is_delim(cmd->type) == 0)
					{
						cmd->type = ARGS;
						cmd = cmd->next;
					}
					if (!cmd)
						break ;
				}
			}
			else
				cmd->type = EXEC;
			node_ref = 0;
		}
		if (cmd->type == PIPE)
			node_ref = 1;
		if (node_ref == 0)
		{
			if (is_redirects(cmd->type))
			{
				if ((ft_strcmp(cmd->cmd, ">") == 0 || 
						ft_strcmp(cmd->cmd, ">>") == 0 ||
						ft_strcmp(cmd->cmd, "<") == 0) && cmd->next->cmd)
					cmd->next->type = FLE;
				else if (ft_strcmp(cmd->cmd, "<<") == 0 && cmd->next->cmd)
					cmd->next->type = ENDOF;
			}
			else if (is_builtin(cmd->cmd) == 1)
			{
				if (ft_strcmp(cmd->cmd, "echo") == 0 && cmd->next->cmd)
				{
					cmd = cmd->next;
					while (cmd && is_delim(cmd->type) == 0)
					{
						cmd->type = ARGS;
						cmd = cmd->next;
					}
					if (!cmd)
						break ;
				}
			}
			else if (ft_strcmp(cmd->cmd, "Makefile") == 0)
				cmd->type = FLE;
		}
		cmd = cmd->next;
	}
	cmd = head;
}
