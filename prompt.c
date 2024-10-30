/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/10/29 22:14:38 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_init_input	*init_list(void)
{
	t_init_input	*list;

	list = (t_init_input *)malloc(sizeof(t_init_input));
	if (!list)
		return (NULL);
	list->string = NULL;
	list->args = NULL;
	list->fd_in = dup(STDIN);
	list->fd_out = dup(STDOUT);
	list->token = (t_token){0};
	list->prev = NULL;
	list->next = NULL;
	return (list);
}

void print_the_stack(t_init_input *list)
{
	t_init_input *current = list;

	// Traverse and print the linked list
	while (current != NULL)
	{
		printf("string: [%s] - token [%u]\n", current->string, current->token);
		current = current->next;
	}
}

int	add_to_history(char *line)
{
	int i;

	i = -1;
	if (!line)
		return (0);
	while (line[++i])
	{
		if (!ft_is_whitspace(line[i]))
		{
			add_history(line);
			return (1);
		}
	}
	return (0);
}

void	prompt(char **envp)
{
	char			*prompt;
	char			*prompt_dup;
	t_init_input	*input_list;
	t_envp			*env_list;

	env_list = get_envp(envp); 	// get the envp list
	input_list = init_list(); // initialize the input list
	while (1) // loop the shell.
	{
		signal(SIGINT, handle_signals); // SIGINT is the signal sent by pressing Ctrl+C
		signal(SIGQUIT, SIG_IGN); // SIGQUIT is the signal sent by pressing Ctrl+D.
		prompt = readline(PROGRAM_NAME); // the prompt
		if (add_to_history(prompt)) // add the prompt to the history and go on
			prompt_dup = ft_strdup(prompt);
		if (ft_strcmp(prompt, "exit") == 0) //if the user types exit, the shell will exit.
		{
			// incluir lógica para verificar argumentos após exit.
			exit_mini(input_list, prompt, prompt_dup, env_list); // exit the shell end clear the memory
		}
		else
		{
			if (!input_check(prompt_dup)) // check if the input is valid
			{
				printf("\n----\nsending to process_input:\n");
				//processe_inut(struct, char**, struct);
				process_input(input_list, prompt_dup, env_list);
			}
			else
			{
				printf("minishell: syntax error\n"); // if the input is invalid, print an error message
				continue;
			}
		}
		free(prompt_dup); // free the prompt_dup
		free(prompt); // free the prompt
	}
}
