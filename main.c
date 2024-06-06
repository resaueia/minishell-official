/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:56:42 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/06/06 20:07:55 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_strlen(char *str)
{
	int i;

	i = 0;
	while(str[i])
		i++;
	return (i);
}

void handle_signals(int signo) {
    if (signo == SIGINT) {
        printf("\nminishell> ");
        fflush(stdout);
    }
}


void	prompt()
{
	char	*input;

	while ((input = readline("minishell> ")) != NULL)
	{
		if (ft_strlen(input) > 0)
		{
			add_history(input);
		//	execute_command(input);
		}
		free(input);
	}
}

int		main()
{
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, SIG_IGN);

	t_envp	cat_env;

	cat_env = &envp;

	prompt();
	return (0);
}
