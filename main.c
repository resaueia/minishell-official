/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 21:02:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/06/10 22:26:42 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	handle_signals(int sign)
{
	if (sign == SIGINT)
	{
		printf("\nminishell> ");
	}
	else if (sign == SIGQUIT)
	{
		printf("\n");
	}
}

int	check_command_line(int c)
{
	if (c > 1)
	{
		printf("Error: too many arguments\n");
		return (0);
	}
	return (1);
}

int	ft_strcmp(char *str, char *value)
{
	int	i;

	i = 0;
	while (str[i] == value[i] && str[i] && value[i])
		i++;
	if (value[i] == '\0')
		return (0);
	else
		return (1);
}

void	ft_putchar_str(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		printf("str[%i] = %c\n", i, str[i++]);
	printf("------ is done ------\n");
}

void	execute_command(char *str)
{
	ft_putchar_str(str);
}

int	main(int c, char *v, char *env[])
{
	char *input;

	if (!check_command_line(c))
		return (0);
	(void)v;
	(void)env;
	// for signal handlin, like ctrl+c and ctrl+d
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, handle_signals);
	// looping the shell
	while (1)
	{
		// the prompt
		input = readline("minishell> ");
		// check if input is not null or empty or full of spaces
		if (input && *input == '\0')
		{
			add_history(input);
			continue ;
		}
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			return (0);
		}
		else
		{
			add_history(input);
			// execute the command
			execute_command(input);
		}
		free(input);
	}
	return (0);
}