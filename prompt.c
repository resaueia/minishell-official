/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/09/04 19:16:47 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(char *cmd, t_envp *env_list)
{
	t_envp	*tmp;

	//printf("on execution\n");
	tmp = env_list;
	if (ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "envp") == 0)
	{
		printf("print envp\n--");
		print_envp_list(tmp);
		printf("\n");
	}
	else if (ft_strcmp(cmd, "pwd") == 0)
	{
		printf("this is pwd\n--");
		ft_pwd();
	}
	else if (ft_strncmp(cmd, "echo", 4) == 0)
	{
		ft_echo(cmd + 4);
	}
	else if (ft_strncmp(cmd, "cd", 2) == 0)
	{
		ft_cd(cmd + 3);
	}
	else if (ft_strncmp(cmd, "export ", 7) == 0)
	{
		printf("this is export\n--");
		printf("cmd: %s\n", cmd);
		printf("export: %s\n", cmd + 7);
		ft_export(cmd + 7, &tmp);
	}
	else if (ft_strncmp(cmd, "unset ", 6) == 0)
	{
		printf("this is unset\n--");
		printf("cmd: %s\n", cmd);
		printf("unset: %s\n", cmd + 6);
		ft_unset(cmd + 6, &tmp);
	}
}

void	prompt(char **env)
{
	char	*prompt;
	t_envp	*env_list;

	// for signal handlin
	// SIGINT is the signal sent by pressing Ctrl+C
	// SIGQUIT is the signal sent by pressing Ctrl+D
	// não está funcionando - ainda não sei o motivo.
	// SIGINT roda apenas uma vez e SIQUIT roda com segfault
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, SIG_IGN);
	// get the envp list
	env_list = get_envp(env);
	// loop the shell. Temporária, necessário incluir validações e tratamentos
	while (1)
	{
		// the prompt
		prompt = readline(PROGRAM_NAME);
		add_history(prompt);
		// incluir validações e tratamentos
		if (ft_strcmp(prompt, "exit") == 0)
		{
			free(prompt);
			exit(1);
		}
		else
		{
			//printf("to execute\n");
			//printf("prompt: [%s]\n", prompt);
			// execute the command
			execute_command(prompt, env_list);
		}
		free(prompt);
	}
}
