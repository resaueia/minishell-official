/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:51:08 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/08/15 19:06:56 by rsaueia-         ###   ########.fr       */
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

/* COLORS */

# define RESET "\033[m"
# define PASTEL_BLUE "\033[38;5;153m"
# define PASTEL_PURPLE "\033[38;5;183m"
# define PASTEL_PINK "\033[38;5;218m"
# define MAGENTA "\033[38;5;200m"

# define PROGRAM_NAME PASTEL_BLUE "mi" PASTEL_PURPLE "ni" PASTEL_PINK "shell" MAGENTA "> " RESET

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

// for environment variables
typedef struct s_envp
{
	char			*key;
	char			*value;
	struct s_envp	*next;
}					t_envp;

/* FUNCTION PROTOTYPES */
void				execute_command(char *cmd, char **envp);
void				handle_signals(int sig);
void				prompt(char **envp);
int					check_command_line(int c);
int					ft_strlen(char *str);
int					ft_strcmp(char *str, char *value);
int					ft_strncmp(char *s1, char *s2, size_t n);
char				*ft_strchr(char *s, int c);
char				*ft_strdup(char *s);
t_envp				*create_node(char *key, char *value);
t_envp				*get_envp(char **envp);
void				print_envp_list(t_envp *head);

/* Built-in functions */
void				ft_cd(char *path);
void				ft_pwd(void);
void				ft_echo(char *args);
void				ft_export(char *var, t_envp **env_list);
void				ft_unset(char *var, t_envp **env_list);

#endif
