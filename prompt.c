/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/06/13 17:40:23 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(char *cmd, char **envp)
{
	t_envp	*env_list;

	// declaração da lista de variáveis de ambiente
	if (ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "envp") == 0)
	{
		// to get environment variables
		env_list = get_envp(envp);
		// print the environment variables
		print_envp_list(env_list);
	}
	/*else if (ft_strcmp(cmd, "pwd") == 0)
		//do something (figure out which built in is less complex and start there)
	else if (ft_strcmp(cmd, "cd") == 0)
	...
	*/
}

void	prompt(char **env)
{
	char	*prompt;
	char	**args; //This char pointer pointer will be used to store the separate input cmds

	// for signal handlin
	// SIGINT is the signal sent by pressing Ctrl+C
	// SIGQUIT is the signal sent by pressing Ctrl+D
	// não está funcionando - ainda não sei o motivo.
	// SIGINT roda apenas uma vez e SIQUIT roda com segfault
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, SIG_IGN);
	// loop the shell. Temporária, necessário incluir validações e tratamentos
	while (1)
	{
		// the prompt
		prompt = readline("minishell> ");
		if (!prompt)
			continue;
		add_history(prompt);
		args = ft_split(prompt, ' ');
		// incluir validações e tratamentos
		if (args[0])
		{
			if (ft_strcmp(cmd, "exit") == 0)
			{
				free(prompt)
				free_split_results(args)
				return ;
			}
			if (ft_strcmp(args[0], "cd") == 0)
			{	
				if (args[1])
				//call for the upcoming cd function, for instance
			}
			else
				// execute the command
				execute_command(prompt, env);
		}
		free(prompt);
		free(args);
	}
}
