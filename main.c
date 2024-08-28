/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 21:02:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/08/28 17:04:08 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_list(t_init_input *list)
{
	t_init_input *temp;

	while (list)
	{
		temp = list;
		list = list->next;
		if (temp->string)
			free(temp->string);
		free(temp);
	}
}


void	print_stack(t_init_input *stack)
{
	int i;
	t_init_input	*current;

	i = 0;
	current = stack;
	while (current)
	{
		printf("Node: %i, Valor:%s\n", i, current->string);
		current = current->next;
		i++;
	}
	//free(stack);
}

int	check_command_line(int c)
{
	if (c > 1)
	{
		printf("Error: too many arguments\n");
		exit(0);
	}
	return (1);
}

int	main(int c, char **v, char **envp)
{
	char	*input_dup;
	t_init_input	*list = NULL;

	input_dup = ft_strdup(v[1]);
	// check if there is more than one argument
	if (!check_command_line(c))
		return (0);
	//(void)v;

	// looping the shell
	list = ft_split(input_dup);
	prompt(envp, list);
	free(input_dup);
	free_list(list);
	
	return (0);
}