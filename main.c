/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 21:02:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/06/11 21:54:58 by jparnahy         ###   ########.fr       */
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