/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:51:08 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/20 12:42:42 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
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

extern int g_exit_status;

/* COLORS */

# define RESET "\033[m"
# define BLUE "\033[38;5;153m"
# define PURPLE "\033[38;5;183m"
# define PINK "\033[38;5;218m"
# define MAGENTA "\033[38;5;200m"

# define PROGRAM_NAME BLUE "mi" PURPLE "ni" PINK "shell" MAGENTA "> " RESET

/* DATA STRUCTS */

// for token
typedef enum e_token
{
	PIPE = 11,
	IN = 04,
	OUT = 07,
	APPEND = 77,
	HDOC = 44,
	WORD = 01,
	ARGS = 20,
	ERROR = 00,
	FLE = 21,
	BUILTIN = 03,
	EXEC = 05,
	ENDOF = 99,
}					t_token;

// for environment variables
typedef struct s_envp
{
	char			*key;
	char			*value;
	struct s_envp	*next;
}					t_envp;

// for tokenization to exec
typedef struct s_types
{
	char			*cmd; //command or argument
	int				type; //type of token
	int				fd[2]; //fd[0] = in , fd[1] = out
	struct s_types	*prev; //prev node
	struct s_types	*next; //next node
}				t_types;

// for parser
typedef struct s_init_input
{
	char					*string;
	char					**args;
	int						fd_in;
	int						fd_out;
	int						exit_status;
	t_token					token;
	t_types					*types;
	struct s_init_input		*prev;
	struct s_init_input		*next;
}					t_init_input;	


/* ---- FUNCTION PROTOTYPES ---- */

/* MAIN */
int					check_command_line(int c);
void				prompt(char **envp);
int					add_to_history(char *line);

/* INPUT CHECK */
int					is_empty_string(char *str);
int					is_double_delim(char *str);
int					has_end_delim(char *str);
int					quotes_check(char *str);
int					input_check(char *input);
int					check_node(t_types *type);

/* FROM LIBFT */
int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_strlen(char *str);
int					ft_strcmp(char *str, char *value);
int					ft_strncmp(char *s1, char *s2, size_t n);
char				*ft_strdup(char *s);
char				*ft_strchr(char *s, int c);
char				*ft_substr(char *str, unsigned int start, size_t len);
char				*ft_itoa(int n);
char				**the_split(char const *s, char c);
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				*ft_memset(void *dest, int c, size_t n);
void				ft_bzero(void *str, size_t n);
size_t				ft_strlcpy(char *dest, const char *src, size_t sz);
size_t				ft_strlcat(char *dest, const char *src, size_t sz);

/* UTILS */
int					is_whitspace(char c);
int					is_special(char c);
int					is_space(char *args);
int					is_lower(char *args);
int					to_quotes(char c, int quotes);
int					is_expander(t_types *types);
int					is_whitespace_string(const char *str);
void 				remove_quotes(char **str);
char				*joinpath(char *path, char *key, t_envp **env_list);
char				*custom_dup(char *str, int start, int finish);
char				*ft_strjoin(char *s1, char *s2);
char				*ft_strndup(char *str, int	len);
char 				*extract_key(char *str);
char				*ft_strjoin_free(char *s1, char *s2);

/* ENVP */
t_envp				*create_node(char *key, char *value);
t_envp				*get_envp(char **envp);
int					is_key(char *key, t_envp *head);
void				print_envp_list(t_envp *head);
void				lets_expander(t_types *types, t_envp *env_list, int last_exit_status);
void				*create_new_node(t_envp **env_list, char *key, char *value);
char				*get_value(char *name, t_envp *list);
char				*change_path(char *path, char *src, t_envp **head);


/* EXEC */
int 	   			to_exec(t_init_input *input_list, t_types *type, t_envp *env_list);
void				execute_builtin(t_envp *envp, t_init_input *list, t_types *types);
void				exec_cmd(t_init_input *cmd, t_types *type, char **env);
void				execute_command(t_types *type, t_envp *env_list, t_init_input *input_list, char **env);
void				find_command_path(t_types *type, t_envp *env_list);
void				clear_heredoc_files(void);

/* OTHERS */
void				process_input(t_init_input *input_list, t_types *types, char *prompt, t_envp *env_list);
t_init_input		*split_commands(char **commands, t_init_input **head, t_init_input **tail);
void				remove_quotes_from_types(t_types *types);

/* PARSER AND TOKENIZATION */
t_init_input		*add_node(char *input, t_token token);
t_init_input		*ft_split(char *s);
t_init_input		*delim_split(char *s);
void 				handle_substring(char *s, size_t *i, int *start_index, t_init_input **head, t_init_input **tail);
t_token				get_token(char *c);
void    			add_to_list(t_init_input **head, t_init_input **tail, char *substr, t_token token);

/* REPARSER */
char				**args_split(char *input);
void				insert_types(t_types **head, char *wrd);
int					what_type(char *wrd);
void				include_fds(t_init_input *input_list);
void				args_of_cmds(t_types *cmd);

/* TO CONVERT LIST TO CHAR** */
char    			**types_to_char(t_types *list);
char    			**list_to_char(t_init_input *list);
char				**env_to_char(t_envp *env_list);

/* EXEC CHECK*/
char				**lexer(char *input);
int					has_heredoc(t_init_input *cmd);
int					has_redirect(t_init_input *cmd);
int 				has_pipe(t_init_input *cmd);
int					has_builtin(t_init_input *cmd);
/* ******************************************************* */
int					is_hdoc(t_types *type);
int					is_pp(t_types *type);
int					is_rdrct(t_types *type);
int					is_btin(t_types *type);
int					is_exec(t_types *type);

/* HEREDOC */
int					is_heredoc(t_init_input *input_list, t_types *type);
int					handle_heredoc(t_init_input *input_list, t_types *type);
int					tackle_heredoc(t_types *type, char *start_delim, char *last_delim);

/* REDIRECTS */
int					handle_redirection(t_init_input *input_list, t_types *type);
int					setup_redirection(t_init_input *args_list, t_types *type);
void				remove_node(t_types **node);

/* PIPES */
int					handle_pipeline(t_init_input *input_list, t_envp *env_list, t_types *type);
int					setup_pipeline(t_init_input *input_list, t_envp *env_list);
int					to_exec_pipe(t_init_input *input_list, t_types *type, t_envp *env_list);
void				exec_cmd_pipe(t_init_input *cmd, t_types *type, char **env);

/* Built-in functions */
int					is_builtin(char *wrd);
void				ft_pwd(int fd_out);
void				ft_echo(t_types *args, t_envp **env_list, int fd_out);
void				ft_cd(t_types *path, t_envp **env_list);
void				ft_export(t_types *var, t_envp **env_list);
void				ft_unset(t_types *var, t_envp **env_list);
void				exit_shell(char *prompt, char *prompt_dup, t_init_input *input_list, t_envp *env_list);

/* TO FREE */
char				*free_char_ptr(char *ptr);
char				**free_from_split(char **str);
void				free_list(t_init_input *list);
void				free_types(t_types *types);
void				free_env(t_envp *env_list);
void				exit_mini(t_init_input *list, char *prompt, char *prompt_dup, t_envp *env_list);

/* just suport */
void				print_the_stack(t_init_input *list);
void				print_stack(t_init_input *stack);

#endif
