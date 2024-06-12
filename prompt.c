/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/06/11 21:56:43 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_putchar_str(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		printf("str[%i] = %c\n", i, str[i]);
	printf("------ is done ------\n");
}

void	execute_command(char *str)
{
	ft_putchar_str(str);
}

void	minishell_prompt(void)
{
	char	*prompt;

	// for signal handlin
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
		prompt = readline("minishell> ");
		printf("prompt = %s\n", prompt);
		// check if input is not null or empty or full of spaces
		if (prompt && *prompt == '\0')
		{
			add_history(prompt);
			continue ;
		}
		if (ft_strcmp(prompt, "exit") == 0 || prompt == NULL)
		{
			free(prompt);
			return ;
		}
		else
		{
			add_history(prompt);
			// execute the command
			execute_command(prompt);
		}
		free(prompt);
	}
}
