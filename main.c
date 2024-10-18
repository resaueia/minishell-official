/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 21:02:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/10/17 21:23:12 by jparnahy         ###   ########.fr       */
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
    t_init_input *head = stack;
    t_init_input *current = head;
	while (current != NULL)
    {
        printf("Valor: %s\n", current->string);
        current = current->next;
    }
    current = head;
    t_init_input *tmp;
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
		exit(0);
	}
	return (1);
}
int	main(int c, char **v, char **envp)
{
	if (!check_command_line(c)) // check if there is more than one argument
		return (0);
	(void)v;
	
	prompt(envp); // looping the shell
	
	//free(input_dup);
	return (0);
}