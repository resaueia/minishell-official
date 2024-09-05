/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/09/05 18:42:31 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(char *cmd, char **envp, t_init_input *list)
{
	t_envp	*env_list;

	if (ft_strcmp(cmd, "print") == 0)
	{
		//printf("entrou no print_stack");
		print_stack(list);
	}
	else if (ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "envp") == 0)
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

void	prompt(char **envp)
{
	char			*prompt;
	char			*prompt_dup;
	t_init_input	*input_list;

	// for signal handling
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
		prompt = readline(PROGRAM_NAME);
		add_history(prompt);
		prompt_dup = ft_strdup(prompt);
		input_list = ft_split(prompt_dup);
		// incluir validações e tratamentos
		if (ft_strcmp(prompt, "exit") == 0)
		{
			free(prompt);
			return ;
		}
		else
		{
			// execute the command
			execute_command(prompt, envp, input_list);
		}
		free(prompt);
		free(input_list);
	}
}
