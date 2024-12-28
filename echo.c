/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thfranco <thfranco@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:40:25 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/24 17:56:16 by thfranco         ###   ########.fr       */
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
		str = ft_strjoin_free(str, " ");
		str = ft_strjoin_free(str, temp->cmd);
		temp = temp->next;
	}
	return (str);
}

// Handles "-n" flag logic for ft_echo
static int	handle_newline_flag(char **args)
{
	char	*tmp;
	char	*new_args;

	tmp = *args;
	if (ft_strlen(*args) == 0)
		return (1);
	if (ft_strncmp(*args, "-n", 2) == 0)
	{
		(*args)++;
		while (**args == 'n')
			(*args)++;
		if (**args == ' ' || **args == '\0')
		{
			(*args)++;
			new_args = ft_strdup(*args);
			free(tmp);
			if (!new_args)
				return (1);
			*args = new_args;
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
	args = NULL;
	if (!cmds->cmd || !cmds->next)
	{
		ft_putstr_fd("\n", fd_out);
		return ;
	}
	args = args_to_str(cmds);
	newline = handle_newline_flag(&args);
	echo_output(args, newline, fd_out);
	if (args)
	{
		free(args);
		args = NULL;
	}
}

/*
 * ft_echo: Implements the shell's echo command with support for the "-n" flag.
 * Converts arguments into a single string, checks for the "-n" flag, and outputs
 * the resulting string to the specified file descriptor.
 */
