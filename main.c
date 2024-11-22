/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 21:02:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/22 11:27:36 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//volatile sig_atomic_t g_signal = 0;

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
		exit(1);
	}
	return (1);
}

int	main(int c, char **v, char **envp)
{
	if (!check_command_line(c)) // check if there is more than one argument
		return (0);
	(void)v;
	
	prompt(envp); // looping the shell
	return (0);
}