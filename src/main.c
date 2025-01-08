/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 21:02:03 by jparnahy          #+#    #+#             */
/*   Updated: 2025/01/06 18:16:07 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_stack(t_init_input *stack)
{
	t_init_input	*head;
	t_init_input	*current;
	t_init_input	*tmp;

	head = stack;
	current = head;
	while (current != NULL)
	{
		printf("Valor: %s\n", current->string);
		current = current->next;
	}
	current = head;
	while (current != NULL)
	{
		tmp = current;
		current = current->next;
		free(tmp->string);
		free(tmp);
	}
}

int	check_command_line(int c)
{
	if (c > 1)
	{
		printf("Error: too many arguments\n");
		exit(1);
	}
	return (1);
}

int	main(int c, char **v, char **envp)
{
	if (!check_command_line(c))
		return (1);
	(void)v;
	prompt(envp);
	return (0);
}
