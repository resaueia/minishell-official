/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:56:42 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/06/12 17:01:45 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_command_line(int c)
{
	if (c > 1)
	{
		printf("Error: too many arguments\n");
		exit(0);
	}
	return (1);
}

int	main(int c, char **v, char **env)
{
	if (!check_command_line(c))
		return (0);
	(void)v;
	(void)env;
	// looping the shell
	minishell_prompt();
	return (0);
}
