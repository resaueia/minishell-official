/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:40:25 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/22 21:58:43 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*args_to_str(t_types *args)
{
	char	*str;
	t_types	*temp;

	temp = args;
	if (temp->next == NULL)
	{
		str = ft_strdup("");
		return (str);
	}
	if (temp->type == 3 && temp->next)
		temp = temp->next;
	str = ft_strdup(temp->cmd);
	temp = temp->next;
	while (temp && temp->type == 20)
	{
		str = ft_strjoin(str, " ");
		str = ft_strjoin(str, temp->cmd);
		temp = temp->next;
	}
	return (str);
}

// Handles "-n" flag logic for ft_echo
static int	handle_newline_flag(char **args)
{
	char	*tmp;

	tmp = *args;
	if (ft_strncmp(*args, "-n", 2) == 0)
	{
		(*args)++;
		while (**args == 'n')
			(*args)++;
		if (**args == ' ' || **args == '\0')
		{
			(*args)++;
			return (0);
		}
		*args = tmp;
	}
	return (1);
}

// Outputs the echo arguments to the specified fd
static void	echo_output(char *args, int newline, int fd_out)
{
	ft_putstr_fd(args, fd_out);
	if (newline)
		ft_putchar_fd('\n', fd_out);
}

// Main ft_echo function
void	ft_echo(t_types *cmds, t_envp **env_list, int fd_out)
{
	char	*args;
	int		newline;

	(void)env_list;
	if (!cmds->cmd || !cmds->next)
	{
		ft_putstr_fd("\n", fd_out);
		return ;
	}
	args = args_to_str(cmds);
	newline = handle_newline_flag(&args);
	echo_output(args, newline, fd_out);
	free(args);
}

/*
 * ft_echo: Implements the shell's echo command with support for the "-n" flag.
 * Converts arguments into a single string, checks for the "-n" flag, and outputs
 * the resulting string to the specified file descriptor.
 */