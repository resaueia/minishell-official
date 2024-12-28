/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_start.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thfranco <thfranco@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:56:49 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/27 20:03:48 by thfranco         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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

t_init_input	*init_list(void)
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
