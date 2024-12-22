/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_nodes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:28:38 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/22 02:55:08 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_first_node(t_types **cmd, int *node_ref)
{
	if (is_redirects((*cmd)->type))
		handle_redirects(*cmd);
	else if (is_builtin((*cmd)->cmd))
	{
		if (handle_builtins_first(cmd))
		{
			*node_ref = 0;
			return (1);
		}
	}
	else
		(*cmd)->type = EXEC;
	*node_ref = 0;
	return (0);
}

static int process_non_first_node(t_types **cmd, int *node_ref)
{
	if (is_redirects((*cmd)->type))
		handle_redirects(*cmd);
	else if (is_builtin((*cmd)->cmd))
	{
		if (handle_builtins_non_first(cmd))
			return (1);
	}
	else if (ft_strcmp((*cmd)->cmd, "Makefile") == 0)
		(*cmd)->type = FLE;
	if ((*cmd)->type == PIPE)
		*node_ref = 1;
	return (0);
}


void args_of_cmds(t_types *cmd)
{
	t_types	*head;
	int		node_ref;

	if(!cmd)
		return ;
	head = cmd;
	node_ref = 1;
	while (cmd)
	{
		if (node_ref == 1)
		{
			if (process_first_node(&cmd, &node_ref))
				break ;
		}
		if (node_ref == 0)
			process_non_first_node(&cmd, &node_ref);
		if (cmd)
			cmd = cmd->next;
	}
	cmd = head;
}
