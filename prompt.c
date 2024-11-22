/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/22 15:58:21 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_signals(int sig, siginfo_t *info, void *context)
{
	(void) context;
	printf("\n");
	if (sig == SIGINT && info->si_pid)
	{
		rl_on_new_line ();
		rl_replace_line ("", 0);
		rl_redisplay ();
	}
}

/*static t_types	*init_types(void)
{
	t_types	*types;

	types = (t_types *)malloc(sizeof(t_types));
	if (!types)
		return (NULL);
	types->cmd = NULL;
	types->type = 0;
	types->prev = NULL;
	types->next = NULL;
	return (types);
}*/

static t_init_input	*init_list(void)
{
	t_init_input	*list;
	struct	sigaction	act;

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
	list->fd_in = STDIN_FILENO;
	list->fd_out = STDOUT_FILENO;
	list->token = (t_token){0};
	list->types = NULL;
	list->prev = NULL;
	list->next = NULL;
	return (list);
}

int	add_to_history(char *line)
{
	int i;

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
		prompt = readline(PROGRAM_NAME); // the prompt
		if (!prompt)
		{
			printf("Exiting minishell...\n");
			break ;
		}
		if(input_list)
			free_list(input_list);
		input_list = init_list();
		if (add_to_history(prompt)) // add the prompt to the history and go on
			prompt_dup = ft_strdup(prompt);
		printf("prompt: [%s]\n", prompt);
		if (ft_strncmp(prompt, "exit", 4) == 0) //if the user types exit, the shell will exit.
		{
			int ret;
			int i;

			ret = 0;
			i = 4;
			while (prompt[i] == ' ')
				i++;
			while (prompt[i])
			{
				if (is_whitspace(prompt[i]))
				{
					printf("exit\nminishell: exit: too many arguments\n");
					exit(1);
				}
				if (!ft_isdigit(prompt[i]))
				{
					printf("exit\nminishell: exit: %s: numeric argument required\n", prompt + 4);
					exit(255);
				}
				else if (ft_isdigit(prompt[i]))
				{
					ret = ret * 10 + prompt[i] - '0';
					i++;
				}
			}
			exit_mini(input_list, prompt, prompt_dup, env_list); // exit the shell end clear the memory
			exit(ret);
		}
		else
		{
			if (!input_check(prompt_dup)) // check if the input is valid
			{
				printf("\n----\n");
				printf("input_list: [%p]\n", input_list);
				printf("input_list->types: [%p]\n", input_list->types);
				printf("input_list->fd_in: [%p]\n", &input_list->fd_in);
    			printf("input_list->fd_out: [%p]\n", &input_list->fd_out);	
				printf("env_list: [%p]\n", env_list);
				printf("\n----\nsending to process_input:\n");
				//processe_inut(struct, char**, struct);
				process_input(input_list, input_list->types, prompt_dup, env_list);
			}
			else
			{
				printf("minishell: syntax error\n"); // if the input is invalid, print an error message
				continue;
			}
		}
		free(prompt); // free the prompt
	}
}
