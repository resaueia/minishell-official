/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 21:02:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/10/28 20:56:48 by jparnahy         ###   ########.fr       */
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

void	free_env(t_envp *env_list)
{
	t_envp *temp;

	while (env_list)
	{
		temp = env_list;
		env_list = env_list->next;
		if (temp->key)
			free(temp->key);
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}

void	exit_mini(t_init_input *list, char *prompt, char *prompt_dup, t_envp *env_list)
{
	free(prompt);
	free(prompt_dup);
	free_list(list);
	free_env(env_list);
	exit(0); //exit the shell with error code 0 - no error
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
	
	//free(input_dup);
	return (0);
}