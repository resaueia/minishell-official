/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/10/14 19:33:20 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(char *cmd, t_envp *env_list, t_init_input *list)
{
	t_envp	*tmp;

	tmp = env_list;
	if (ft_strcmp(cmd, "print") == 0)
	{
		//printf("entrou no print_stack");
		print_stack(list);
	}
	else if (ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "envp") == 0)
		print_envp_list(tmp);
	else if (ft_strcmp(cmd, "pwd") == 0)
		ft_pwd();
	else if (ft_strncmp(cmd, "echo", 4) == 0)
		ft_echo(cmd + 4);
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		ft_cd(cmd + 2, &tmp);
	else if (ft_strncmp(cmd, "export", 6) == 0)
	{
		printf("this is export\n--");
		printf("cmd: %s\n", cmd);
		printf("export: %s\n", cmd + 7);
		ft_export(cmd + 7, &tmp);
	}
	else if (ft_strncmp(cmd, "unset", 5) == 0)
	{
		printf("this is unset\n--");
		printf("cmd: %s\n", cmd);
		printf("unset: %s\n", cmd + 6);
		ft_unset(cmd + 6, &tmp);
	}
}

void	prompt(char **envp)
{
	char			*prompt;
	char			*prompt_dup;
	t_init_input	*input_list;
	t_envp			*env_list;

	// for signal handling
	// SIGINT is the signal sent by pressing Ctrl+C
	// SIGQUIT is the signal sent by pressing Ctrl+D
	// não está funcionando - ainda não sei o motivo.
	// SIGINT roda apenas uma vez e SIQUIT roda com segfault
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, SIG_IGN);
	// get the envp list
	env_list = get_envp(envp);
	// loop the shell. Temporária, necessário incluir validações e tratamentos
	while (1)
	{
		// the prompt
		prompt = readline(PROGRAM_NAME);
		add_history(prompt);
		prompt_dup = ft_strdup(prompt);
		//process_input;
		input_list = ft_split(prompt_dup);
		// incluir validações e tratamentos
		if (ft_strcmp(prompt, "exit") == 0)
		{
			free(prompt);
			exit(1);
		}
		else
		{
			// execute the command
			execute_command(prompt, env_list, input_list);
		}
		free(prompt);
		//free(input_list);
	}
}
