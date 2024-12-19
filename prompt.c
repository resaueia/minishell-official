/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/18 21:58:26 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_signals(int sig, siginfo_t *info, void *context)
{
	(void) context;
	printf("\n");
	if (sig == SIGINT && info->si_pid)
	{
		rl_on_new_line ();
		rl_replace_line ("", 0);
		rl_redisplay ();
	}
}

static t_init_input	*init_list(void)
{
	t_init_input	*list;
	struct	sigaction	act;

	ft_bzero(&act, sizeof(act));
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handle_signals;
	sigaction(SIGINT, &act, NULL);
	signal(SIGQUIT, SIG_IGN);
	list = (t_init_input *)malloc(sizeof(t_init_input));
	if (!list)
		return (NULL);
	list->string = NULL;
	list->args = NULL;
	list->fd_in = 0;
	list->fd_out = 1;
	list->exit_status = 0;
	list->token = (t_token){0};
	list->types = NULL;
	list->prev = NULL;
	list->next = NULL;
	return (list);
}

int	add_to_history(char *line)
{
	int i;

	i = 0;
	if (!line)
		return (0);
	while (line[i] != '\0')
	{
		if (!is_whitspace(line[i]))
		{
			add_history(line);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	handle_empty_or_exit(char *prompt, t_init_input *input_list, t_envp *env_list)
{
	if (!prompt) // if the prompt is empty
	{
		exit_shell(NULL, NULL, input_list, env_list); // exit the shell
		return (1);
	}
	if (!*prompt || is_whitespace_string(prompt)) // if the prompt is empty or has only spaces
	{
		free(prompt); // free the prompt
		return (1);
	}
	if (ft_strncmp(prompt, "exit", 4) == 0) // if the prompt is exit
	{
		exit_shell(prompt, NULL, input_list, env_list); // exit the shell
		return (1);
	}
	return (0); // return 0 if the prompt is not empty or exit
}

static void	exec_shell(char *prompt_dup, t_init_input *input_list, t_envp *env_list)
{
	if (!input_check(prompt_dup)) // check if the input is valid
		process_input(input_list, input_list->types, prompt_dup, env_list);
	else
	{
		printf("minishell: syntax error\n"); // if the input is invalid, print an error message
	}
}

static void	process_command(char *prompt, t_init_input *input_list, t_envp *env_list)
{
	char	*prompt_dup;

	if (add_to_history(prompt)) // add the prompt to the history
		prompt_dup = ft_strdup(prompt); // duplicate the prompt
	exec_shell(prompt_dup, input_list, env_list); // execute the shell
	free(prompt); // free the prompt
	free(prompt_dup); // free the prompt_dup
}

void	prompt(char **envp)
{
	char			*prompt;
	t_init_input	*input_list;
	t_envp			*env_list;

	env_list = get_envp(envp); 	// get the envp list
	input_list = init_list(); // initialize the input list
	while (1) // loop the shell.
	{
		prompt = readline("minishell> "); // the prompt
		if (handle_empty_or_exit(prompt, input_list, env_list)) // handle the empty prompt or exit
			continue;
		process_command(prompt, input_list, env_list); // process the command
	}
}

/*static void	handle_exit_command(char *prompt, t_init_input *input_list, 
                                char *prompt_dup, t_envp *env_list)
{
    int ret = 0;
    int i = 4;

    while (prompt[i] == ' ')
        i++;
    while (prompt[i])
    {
        if (is_whitspace(prompt[i]))
        {
            printf("exit\nminishell: exit: too many arguments\n");
            exit(1);
        }
        if (!ft_isdigit(prompt[i]))
        {
            printf("exit\nminishell: exit: %s: numeric argument required\n", prompt + 4);
            exit(255);
        }
        ret = ret * 10 + prompt[i] - '0';
        i++;
    }
    exit_mini(input_list, prompt, prompt_dup, env_list);
    exit(ret);
}

static void process_shell_input(char *prompt_dup, t_init_input *input_list, t_envp *env_list)
{
    if (!input_check(prompt_dup))
    {
        process_input(input_list, input_list->types, prompt_dup, env_list);
    }
    else
    {
        printf("minishell: syntax error\n");
    }
}
static void handle_prompt_input(char *prompt, char **prompt_dup, t_init_input *input_list, t_envp *env_list)
{
    if (add_to_history(prompt))
    {
        if (*prompt_dup)
            free(*prompt_dup);
        *prompt_dup = ft_strdup(prompt);
    }

    if (ft_strncmp(prompt, "exit", 4) == 0)
    {
        handle_exit_command(prompt, input_list, *prompt_dup, env_list);
    }
    else
    {
        process_shell_input(*prompt_dup, input_list, env_list);
    }
}*/

/* Function: handle_prompt_input
 * Handles user input:
 * - Adds the command to history.
 * - Duplicates the input for processing.
 * - Calls appropriate functions for "exit" or other shell commands.
 */

/*void prompt(char **envp)
{
    char            *prompt;
    char            *prompt_dup;
    t_init_input    *input_list;
    t_envp          *env_list;

    env_list = get_envp(envp);
    input_list = init_list();
    prompt_dup = NULL;
    while (1)
    {
        prompt = readline("minishell> ");
        if (!prompt) // Handle EOF (CTRL+D)
        {
            printf("exit\n");
            rl_clear_history();
            free_env(env_list);
            free_list(input_list);
            break;
        }
        handle_prompt_input(prompt, &prompt_dup, input_list, env_list);
        free(prompt);
    }
    if (prompt_dup)
        free(prompt_dup);
}*/
