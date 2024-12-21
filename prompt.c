/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/20 23:14:17 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_signals(int sig, siginfo_t *info, void *context)
{
	(void)context;
	printf("\n");
	if (sig == SIGINT && info->si_pid)
	{
		last_status(130);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static t_init_input	*init_list(void)
{
	t_init_input		*list;
	struct sigaction	act;

	ft_bzero(&act, sizeof(act));
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handle_signals;
	sigaction(SIGINT, &act, NULL);
	signal(SIGQUIT, SIG_IGN);
	list = (t_init_input *)malloc(sizeof(t_init_input));
	if (!list)
		return (NULL);
	list->string = NULL;
	list->args = NULL;
	list->fd_in = 0;
	list->fd_out = 1;
	list->exit_status = 0;
	list->token = (t_token){0};
	list->types = NULL;
	list->prev = NULL;
	list->next = NULL;
	return (list);
}

int	add_to_history(char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (0);
	while (line[i] != '\0')
	{
		if (!is_whitspace(line[i]))
		{
			add_history(line);
			return (1);
		}
		i++;
	}
	return (0);
}

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
	free(prompt_dup);
}

void	prompt(char **envp)
{
	char			*prompt;
	t_init_input	*input_list;
	t_envp			*env_list;

	env_list = get_envp(envp);
	input_list = init_list();
	while (1)
	{
		prompt = readline("minishell> ");
		if (handle_empty_or_exit(prompt, input_list, env_list))
			continue ;
		process_command(prompt, input_list, env_list);
	}
}
