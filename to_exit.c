/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:44:40 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/22 01:22:43 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Function: exit_mini
 * Frees all dynamically allocated resources used during the program's
 * execution, including input lists, prompts, and environment variables.
 */

void	exit_mini(t_init_input *list, char *prompt, char *prompt_dup,
		t_envp *env_list)
{
	if (list)
		free_list(list);
	if (prompt)
		free_char_ptr(prompt);
	if (prompt_dup)
		free_char_ptr(prompt_dup);
	if (env_list)
		free_env(env_list);
}

void	fd_closer(t_init_input *input_list, t_types *type)
{
	while (input_list)
	{
		if (input_list->fd_in != 0)
			close(input_list->fd_in);
		if (input_list->fd_out != 1)
			close(input_list->fd_out);
		input_list = input_list->next;
	}
	while (type)
	{
		if (type->fd[0] != 0)
			close(type->fd[0]);
		if (type->fd[1] != 1)
			close(type->fd[1]);
		type = type->next;
	}
}
