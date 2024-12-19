/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:38:45 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/18 22:33:01 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd_pipe(t_init_input *cmd, t_types *type, char **env)
{
	char	**args;

	(void)cmd;
	args = types_to_char(type);
	(void)args;
	if (execve(type->cmd, args, env) == -1)
	{
		perror("Execution has failed");
		exit(EXIT_FAILURE);
	}
}

static int	handle_heredoc(t_init_input *input_list, t_types *type)
{
	include_fds(input_list);
	if (is_heredoc(input_list, type) == -1)
	{
		perror("Error setting up heredoc");
		exit(EXIT_FAILURE);
	}
	free_list(input_list);
	free_types(type);
	return (0);
}

/* Function: handle_heredoc
 * Configures heredoc by including file descriptors and calling is_heredoc.
 * Exits with an error message on failure.
 */

static int	handle_redirection(t_init_input *input_list, t_types *type)
{
	if (setup_redirection(input_list, type) == -1)
	{
		perror("Error while setting up redirection");
		exit(EXIT_FAILURE);
	}
	return (0);
}

/* Function: handle_redirection
 * Calls setup_redirection to configure input/output redirection.
 * Exits with an error message on failure.
 */

static void	execute_command(t_init_input *input_list, t_types *type,
		t_envp *env_list, char **env)
{
	if (is_btin(type)) // Built-in commands
	{
		execute_builtin(type->cmd, env_list, input_list, type);
	}
	else // External commands
	{
		find_command_path(type, env_list);
		exec_cmd_pipe(input_list, type, env);
	}
}

/* Function: execute_command
 * Executes either built-in commands using execute_builtin or external
 * commands using exec_cmd_pipe.
 */

int	to_exec_pipe(t_init_input *input_list, t_types *type, t_envp *env_list)
{
	char	**env;

	// t_types *tmp;
	env = env_to_char(env_list);
	// tmp = type;
	if (is_hdoc(type)) // Heredoc
		return (handle_heredoc(input_list, type));
	if (is_rdrct(type)) // Redirection
		handle_redirection(input_list, type);
	execute_command(input_list, type, env_list, env);
	free_list(input_list);
	free_types(type);
	return (0);
}

/* Function: to_exec_pipe
 * Orchestrates the execution of commands in a pipeline:
 * - Handles heredoc setup.
 * - Handles input/output redirections.
 * - Executes built-in or external commands.
 * Frees the input list and type structures before returning.
 */

/*int    to_exec_pipe(t_init_input *input_list, t_types *type, t_envp *env_list)
{
	(void) input_list;
	char  **env;
	t_types     *tmp;

	env = env_to_char(env_list);
	tmp = type;
	(void) env;
	(void) type;
	(void) tmp;

	if (is_hdoc(type)) //heredoc
	{
		include_fds(input_list);
		if (is_heredoc(input_list, type) == -1)
		{
			perror ("Error setting up heredoc");
			//free_list(args_list);
			//free_list(input_list);
			exit(EXIT_FAILURE);
		}
		free_list(input_list);
		free_types(type);
		return (0);
	}
	if (is_rdrct(type))
	{
		if (setup_redirection(input_list, type) == -1)
		{
			perror("Error whule setting up redirection\n");
			//free_list(args_list);
			//free_list(cmd_list);
			exit(EXIT_FAILURE);
		}
	}
	if (is_btin(type)) //builtin
		execute_builtin(type->cmd, env_list, input_list, type);
	else //if (is_exec(type)) //execve
	{
		find_command_path(type, env_list);
		exec_cmd_pipe(input_list, type, env);
	}
	free_list(input_list);
	free_types(type);
	return (0);
}*/