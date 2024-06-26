/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:51:08 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/06/24 19:10:36 by rsaueia-         ###   ########.fr       */
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

/* DATA STRUCTS */
typedef enum e_token
{
	PIPE,
	IN,
	OUT,
	APPEND,
	HDOC,
	WORD,
	ERROR,
}					t_token;

// for evironment variables
typedef struct s_envp
{
	char			*key;
	char			*value;
	struct s_envp	*next;
}					t_envp;

/* FUNCTION PROTOTYPES */
void				ft_pwd(void);
void				execute_command(char *cmd, char **envp);
void				handle_signals(int sig);
void				prompt(char **envp);
int					check_command_line(int c);
int					ft_strlen(char *str);
int					ft_strcmp(char *str, char *value);
char				*ft_strchr(char *s, int c);
char				*ft_strdup(char *s);
t_envp				*create_node(char *key, char *value);
t_envp				*get_envp(char **envp);
void				print_envp_list(t_envp *head);

/* Built-in commands */
int					builtin_cd(char **args);
int					builtin_pwd(char **args);
int					builtin_echo(char **args);
int					builtin_export(char **args);
int					builtin_unset(char **args);
int					builtin_env(char **args);
int					builtin_exit(char **args);

#endif
