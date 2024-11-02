/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 21:02:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/01 21:47:42 by jparnahy         ###   ########.fr       */
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
	printf("Exiting minishell\n");
	if (list)
	{
		printf("Freeing list\n");
		free_list(list);
	}
	if (prompt)
	{
		printf("Freeing prompt\n");
		free(prompt);
	}
	if (prompt_dup)
	{
		printf("Freeing prompt_dup\n");
		free(prompt_dup);
	}
	if (env_list)
	{
		printf("Freeing env_list\n");
		free_env(env_list);
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