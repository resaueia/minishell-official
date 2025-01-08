/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:38:45 by jparnahy          #+#    #+#             */
/*   Updated: 2025/01/07 21:16:28 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Function: process_pipe
 * Splits the input string into commands and arguments, inserts them
 * into the types list, and executes the pipeline. Returns the last exit
 * status of the pipeline execution.
 */

int	process_pipe(t_init_input *input_list, t_types *types,
		t_envp *env_list)
{
	char	*prompt;
	char	**cmds;
	int		last_exit_status;

	prompt = ft_strdup(input_list->string);
	cmds = lexer(prompt);
	split_and_insert(&types, cmds);
	args_of_cmds(types);
	cmds = free_from_split(cmds);
	remove_quotes_from_types(types);
	last_exit_status = to_exec(input_list, types, env_list);
	free_env(env_list);
	free(prompt);
	return (last_exit_status);
}

void	handle_child_process(int prev_fd, int *pipe_fd,
		t_init_input *current, t_envp *env_list)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (pipe_fd[1] != -1)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
	process_pipe(current, current->types, env_list);
	exit(EXIT_SUCCESS);
}

void	handle_parent_process(int *prev_fd, int *pipe_fd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
	*prev_fd = pipe_fd[0];
}
