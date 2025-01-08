/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:38:53 by rsaueia           #+#    #+#             */
/*   Updated: 2025/01/07 18:35:55 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**types_to_char(t_types *list)
{
	t_types	*temp;
	char	**args;
	int		i;

	i = 0;
	temp = list;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	args = (char **)malloc(sizeof(char *) * (i + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (list)
	{
		args[i++] = ft_strdup(list->cmd);
		list = list->next;
	}
	args[i] = NULL;
	return (args);
}

static int	count_cmds(t_envp *commands)
{
	t_envp	*temp;
	int		count;

	temp = commands;
	count = 0;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

char	**env_to_char(t_envp *env_list)
{
	t_envp	*temp;
	char	**list;
	int		i;
	int		count;

	temp = env_list;
	count = count_cmds(temp);
	i = 0;
	list = (char **)malloc(sizeof(char *) * (count + 1));
	if (!list)
		return (NULL);
	temp = env_list;
	while (temp)
	{
		list[i] = ft_strjoin_whit_free(ft_strdup(temp->key), "=", 1, 0);
		list[i] = ft_strjoin_whit_free(list[i], temp->value, 1, 0);
		temp = temp->next;
		i++;
	}
	temp = NULL;
	list[i] = NULL;
	return (list);
}

char	**list_to_char(t_init_input *list)
{
	t_init_input	*temp;
	char			**cmds;
	int				i;
	int				count;

	temp = list;
	i = 0;
	count = 0;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	cmds = (char **)malloc(sizeof(char *) * count + 1);
	if (!cmds)
		return (NULL);
	temp = list;
	while (temp)
	{
		cmds[i] = ft_strdup(temp->string);
		temp = temp->next;
		i++;
	}
	cmds[i] = NULL;
	return (cmds);
}

t_init_input	*split_commands(char **commands, t_init_input **head,
		t_init_input **tail)
{
	int				i;
	t_init_input	*split_cmd_list;
	t_init_input	*current_node;

	i = 0;
	while (commands[i])
	{
		split_cmd_list = ft_split(commands[i]);
		current_node = split_cmd_list;
		while (current_node)
		{
			add_to_list(head, tail, current_node->string, current_node->token);
			current_node = current_node->next;
		}
		i++;
	}
	return (*head);
}
