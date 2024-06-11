/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:51:08 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/06/10 22:21:47 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

/* FUNCTION PROTOTYPES */
void	prompt(void);
void	execute_command(char *cmd);
void	handle_signals(int signo);
int		check_command_line(int c);

/* Built-in commands */
int		builtin_cd(char **args);
int		builtin_pwd(char **args);
int		builtin_echo(char **args);
int		builtin_export(char **args);
int		builtin_unset(char **args);
int		builtin_env(char **args);
int		builtin_exit(char **args);

#endif
