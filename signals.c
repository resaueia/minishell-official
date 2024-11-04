/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:35:49 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/04 17:54:25 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_signal_status(int sig)
{
	static int	g_status;

	g_status = 0;
	if (sig >= 0)
		g_status = sig;
	return (g_status);
}

void	handle_signal(int sig)
{
	/*if (sig == SIGINT)
	{
		if (RL_ISSTATE(RL_STATE_READCMD))
			ioctl(STDIN_FILENO, TIOCSTI, "\n"); // This simulates pressing an enter
		else
			write(1, "\n", 1);
		rl_replace_line("", 1);
		rl_on_new_line();
		g_signal_status(130);
		g_signal_received = 130;		
	}*/
	
	if (sig == SIGINT)
    {
        g_signal_received = 130;
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
	/*if (sig == SIGINT)
	{
		g_signal_received = 1;
		write(STDOUT_FILENO, "\n", 1);
	}*/
	return ;
}

/*When CTRL+C is received, I use the ioctl function to emulate the pressing
of the enter key (else I just print a newline). Then, I proceed on to call upon
the rl_replace_line function and the rl_on_new_line() to both clean the prompt line
and update the readline function into a new line. Finally, the global variable is set to
130, which is the standard interruption code.*/