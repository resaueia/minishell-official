/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:35:49 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/07 17:20:52 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		if (RL_ISSTATE(RL_STATE_READCMD))
			ioctl(STDIN_FILENO, TIOCSTI, "\n"); // This simulates pressing an enter
		else
			write(1, "\n", 1);
		rl_replace_line("", 1);
		rl_on_new_line();
		g_signal_received = 130;	
	}*/

void			handle_signals(int sig, siginfo_t *info, void *context)
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

t_init_input	*init_list(void)
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
	list->fd_in = -1;
	list->fd_out = -1;
	list->token = (t_token){0};
	list->types = NULL;
	list->prev = NULL;
	list->next = NULL;
	return (list);
}

	/*
	if (sig == SIGINT)
    {
        g_signal_received = 130;
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }*/
	/*if (sig == SIGINT)
	{
		g_signal_received = 1;
		write(STDOUT_FILENO, "\n", 1);
	}
	return ;
}*/

/*When CTRL+C is received, I use the ioctl function to emulate the pressing
of the enter key (else I just print a newline). Then, I proceed on to call upon
the rl_replace_line function and the rl_on_new_line() to both clean the prompt line
and update the readline function into a new line. Finally, the global variable is set to
130, which is the standard interruption code.*/