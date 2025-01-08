/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 20:52:43 by rsaueia-          #+#    #+#             */
/*   Updated: 2025/01/07 20:58:46 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	rotate_list(t_types **head)
{
	t_types	*current;

	current = *head;
	while (current)
	{
		if (current->type == 1)
		{
			current->type = 5;
			if (current == *head)
				return ;
			if (current->prev)
				current->prev->next = current->next;
			if (current->next)
				current->next->prev = current->prev;
			current->prev = NULL;
			current->next = *head;
			(*head)->prev = current;
			*head = current;
			rotate_list(head);
			return ;
		}
		current = current->next;
	}
}

static void	free_remaining(t_init_input *input_list, t_types *type)
{
	if (input_list)
		free_list(input_list);
	if (type)
		free_types(&type);
}

static int	process_execution_steps(t_init_input *input_list, t_types *type,
				t_envp *env_list)
{
	char	**env;

	env = NULL;
	if (is_hdoc(type))
	{
		rotate_list(&type);
		if (handle_heredoc(input_list, type) == -1)
			return (-1);
	}
	if (is_rdrct(type) && handle_redirection(input_list, type) == -1)
		return (-1);
	if (is_pp(type))
		return (handle_pipeline(input_list, env_list, type));
	if (is_btin(type))
		execute_builtin(env_list, input_list, type);
	if (is_exec(type))
	{
		env = env_to_char(env_list);
		execute_command(type, env_list, input_list, env);
		env = free_from_split(env);
	}
	clear_heredoc_files();
	fd_closer(input_list, type);
	free_remaining(input_list, type);
	return (0);
}

int	to_exec(t_init_input *input_list, t_types *type, t_envp *env_list)
{
	if (process_execution_steps(input_list, type, env_list) == -1)
		return (-1);
	return (0);
}
