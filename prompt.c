/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/06/24 19:04:08 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(char *cmd, char **envp)
{
	t_envp	*env_list;

	if (ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "envp") == 0)
	{
		env_list = get_envp(envp);
		print_envp_list(env_list);
	}
	else if (ft_strcmp(cmd, "pwd") == 0)
		ft_pwd();
	else if (ft_strncmp(cmd, "echo ", 5) == 0)
		ft_echo(cmd + 5);
	else if (ft_strncmp(cmd, "cd ", 3) == 0)
		ft_cd(cmd + 3);
	else if (ft_strncmp(cmd, "export ", 7) == 0)
	{
		env_list = get_envp(envp);
		ft_export(cmd + 7, &env_list);
	}
	else if (ft_strncmp(cmd, "unset ", 6) == 0)
	{
		env_list = get_envp(envp);
		ft_unset(cmd + 6, &env_list);
	}
}

void	prompt(char **env)
{
	char	*prompt;

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
		add_history(prompt);
		// incluir validações e tratamentos
		if (ft_strcmp(prompt, "exit") == 0)
		{
			free(prompt);
			return ;
		}
		else
			// execute the command
			execute_command(prompt, env);
		free(prompt);
	}
}
