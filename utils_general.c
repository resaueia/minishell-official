/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_general.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:36:08 by jparnahy          #+#    #+#             */
/*   Updated: 2025/01/07 16:05:40 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_the_stack(t_init_input *list)
{
	t_init_input	*current;

	current = list;
	while (current)
	{
		printf("string: [%s] || token: [%i] || ", current->string,
			current->token);
		printf("fd_in: [%d] || fd_out: [%i]\n", current->fd_in,
			current->fd_out);
		current = current->next;
	}
}

int	last_status(int new_status)
{
	static int	status;

	if (new_status > -1)
		status = new_status;
	return (status);
}

int	check_args(t_types *cmds)
{
	int		count;
	t_types	*temp;

	temp = cmds;
	count = 0;
	while (temp)
	{
		temp = temp->next;
		count++;
	}
	if (count > 2)
	{
		printf("minishell: cd: too many arguments\n");
		last_status(2);
		return (1);
	}
	return (0);
}
