/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/05 15:28:47 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	execute_builtin(char *cmd, t_envp *env_list, t_init_input *list)
{
	t_envp	*tmp;

	tmp = env_list;
	if (ft_strcmp(cmd, "print") == 0)
		print_stack(list);
	else if (ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "envp") == 0)
		print_envp_list(tmp);
	else if (ft_strcmp(cmd, "pwd") == 0)
		ft_pwd();
	else if (ft_strncmp(cmd, "echo", 4) == 0)
		ft_echo(cmd + 4, &tmp);
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		ft_cd(cmd + 2, &tmp);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		ft_export(cmd + 6, &tmp);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		ft_unset(cmd + 5, &tmp);
}
void	prompt(char **envp)
{
	char			*prompt;
	char			*prompt_dup;
	char            **cmds;
	t_init_input	*input_list;
	t_envp			*env_list;

	env_list = get_envp(envp); 	// get the envp list
	// for signal handling
	signal(SIGINT, handle_signal); // SIGINT is the signal sent by pressing Ctrl+C
	signal(SIGQUIT, SIG_IGN); // SIGQUIT is the signal sent by pressing Ctrl+D.
	while (1) // loop the shell.
	{
		//g_signal_received = 0;
		prompt = readline(PROGRAM_NAME); // the prompt
		if (g_signal_received == 130)
		{
			g_signal_received = 0;
			free(prompt);
			continue;
		}
		if (!prompt)
		{
			printf("exit\n");
			break;
		}
		if (add_to_history(prompt)) // add the prompt to the history and go on
		{
			prompt_dup = ft_strdup(prompt);
		}
		if (ft_strcmp(prompt, "exit") == 0) //if the user types exit, the shell will exit.
		{
			free(prompt); //free the prompt
			exit(1); //exit the shell with error code 1
		}
		else
		{
			if (!input_check(prompt)) // check if the input is valid
			{
				//initiliaze list (start/set variables)
				input_list = delim_split(prompt_dup); // split the input into a linked list
				
				cmds = list_to_char(input_list);
				process_input(input_list, cmds);
				
				//print_the_stack(input_list);
				execute_builtin(prompt, env_list, input_list); // execute the command line
			}
			else
			{
				printf("minishell: syntax error\n"); // if the input is invalid, print an error message
				continue;
			}
			
		}
		free(prompt); 
		//free(input_list);
	}
}
