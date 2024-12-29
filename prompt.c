/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/28 22:22:08 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_empty_or_exit(char *prompt, t_init_input *input_list,
		t_envp *env_list)
{
	if (!prompt)
	{
		exit_shell(NULL, NULL, input_list, env_list);
		return (1);
	}
	if (!*prompt || is_whitespace_string(prompt))
	{
		free(prompt);
		return (1);
	}
	if (ft_strncmp(prompt, "exit", 4) == 0)
	{
		exit_shell(prompt, NULL, input_list, env_list);
		return (1);
	}
	return (0);
}

static void	exec_shell(char *prompt_dup, t_init_input *input_list,
		t_envp *env_list)
{
	if (!input_check(prompt_dup))
		process_input(input_list, input_list->types, prompt_dup, env_list);
	else
	{
		last_status(2);
		free(prompt_dup);
		prompt_dup = NULL;
		printf("minishell: syntax error\n");
	}
}

static void	process_command(char *prompt, t_init_input *input_list,
		t_envp *env_list)
{
	char	*prompt_dup;

	prompt_dup = NULL;
	if (add_to_history(prompt))
		prompt_dup = ft_strdup(prompt);
	exec_shell(prompt_dup, input_list, env_list);
	free(prompt);
	prompt = NULL;
}

void	prompt(char **envp)
{
	char			*prompt;
	t_init_input	*input_list;
	t_envp			*env_list;

	env_list = get_envp(envp);
	input_list = init_list();
	last_status(0);
	while (1)
	{
		prompt = readline("minishell> ");
		if (handle_empty_or_exit(prompt, input_list, env_list))
			continue ;
		process_command(prompt, input_list, env_list);
	}
	if (env_list)
		free_env(env_list);
}
