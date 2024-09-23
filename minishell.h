/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:51:08 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/09/23 17:32:22 by jparnahy         ###   ########.fr       */
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
# define BLUE "\033[38;5;153m"
# define PURPLE "\033[38;5;183m"
# define PINK "\033[38;5;218m"
# define MAGENTA "\033[38;5;200m"

# define PROGRAM_NAME BLUE "mi" PURPLE "ni" PINK "shell" MAGENTA "> " RESET

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

typedef struct s_init_input
{
	char					*string;
	struct s_init_input		*prev;
	struct s_init_input		*next;
}							t_init_input;


/* FUNCTION PROTOTYPES */
void				execute_command(char *cmd, t_envp *envp, t_init_input *list);
void				handle_signals(int sig);
void				prompt(char **envp);
void 				remove_quotes(char **str);
int					check_command_line(int c);
int					ft_strlen(char *str);
int					ft_strcmp(char *str, char *value);
int					ft_strncmp(char *s1, char *s2, size_t n);
char				*ft_strchr(char *s, int c);
char				*ft_strdup(char *s);
char				*custom_dup(char *str, int start, int finish);
char				*get_value(char *name, t_envp *list);
char				*change_path(char *path, char *src, t_envp **head);
char				*ft_joinpath(char *path, char *key, t_envp **env_list);
int					is_delimiter(char c);
t_envp				*create_node(char *key, char *value);
t_envp				*get_envp(char **envp);
t_init_input		*add_node(char *input);
t_init_input		*ft_split(char *s);
void				print_envp_list(t_envp *head);
void				print_stack(t_init_input *stack);
void				free_list(t_init_input *list);

/* Built-in functions */
void				ft_cd(char *path, t_envp **env_list);
void				ft_pwd(void);
void				ft_echo(char *args);
void				ft_export(char *var, t_envp **env_list);
void				ft_unset(char *var, t_envp **env_list);

#endif
