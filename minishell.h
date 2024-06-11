/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:51:08 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/06/06 20:07:52 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum    e_token
{
    PIPE,
    IN,
    OUT,
    APPEND,
    HDOC,
    WORD,
    ERROR,
}               t_token;

typedef struct s_envp
{
	char	*key;
	char	*value;
	s_envp	*next;
}				t_envp;

/* FUNCTION PROTOTYPES */
void	prompt();
void	execute_command(char *cmd);
void	handle_signals(int signo);
char	*ft_strchr(char *s, int c);
char    *ft_strdup(char *s);
int     ft_strlen(char *str);

#endif

