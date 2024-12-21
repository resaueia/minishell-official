/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:39:20 by rsaueia           #+#    #+#             */
/*   Updated: 2024/12/21 16:19:53 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_types	*init_types(void)
{
	t_types	*types;

	types = (t_types *)malloc(sizeof(t_types));
	if (!types)
		return (NULL);
	types->cmd = NULL;
	types->type = 0;
	types->prev = NULL;
	types->next = NULL;
	return (types);
}

static void	split_and_insert(t_types **types, char **cmds)
{
	char	**args;
	int		i;
	int		j;

	i = -1;
	while (cmds[++i])
	{
		args = args_split(cmds[i]);
		j = -1;
		while (args[++j])
			insert_types(types, args[j]);
		args = free_from_split(args);
	}
}

/* Function: process_pipe
 * Splits the input string into commands and arguments, inserts them
 * into the types list, and executes the pipeline. Returns the last exit
 * status of the pipeline execution.
 */

static int	process_pipe(t_init_input *input_list, t_types *types,
		t_envp *env_list)
{
	char	*prompt;
	char	**cmds;
	int		last_exit_status; // isso não precisa

	prompt = ft_strdup(input_list->string);
	cmds = lexer(prompt);
	split_and_insert(&types, cmds);
	args_of_cmds(types);
	cmds = free_from_split(cmds);
	remove_quotes_from_types(types);
	// last_exit_status = to_exec(input_list, types, env_list);
		// to_exec vem normal
	last_exit_status = to_exec(input_list, types, env_list);
	free(prompt);
	return (last_exit_status);
		// retorna a função (transformar a last_exit_status numa função)
}

/* Function: handle_parent_process
 * Handles pipe management in the parent process and advances the command list.
 */

static t_init_input	*handle_parent_process(t_init_input *current)
{
	if (current->fd_in != STDIN_FILENO)
		close(current->fd_in);
	if (current->fd_out != STDOUT_FILENO)
		close(current->fd_out);
	current = current->next;
	if (current && current->token == 11)
		current = current->next;
	return (current);
}

/* Function: handle_child_process
 * Handles file descriptor duplication and calls process_pipe
 * for command execution in the child process. Exits with the last exit status.
 */

static void	handle_child_process(t_init_input *current,
		t_init_input *input_list, t_types *types, t_envp *env_list)
{
	int	last_exit_status;

	if (current == input_list)
	{
		dup2(current->fd_out, STDOUT_FILENO);
		close(current->fd_out);
	}
	else if (current->next)
	{
		dup2(current->fd_in, STDIN_FILENO);
		dup2(current->fd_out, STDOUT_FILENO);
		close(current->fd_in);
		close(current->fd_out);
	}
	else
	{
		dup2(current->fd_in, STDIN_FILENO);
		close(current->fd_in);
	}
	last_exit_status = process_pipe(current, types, env_list);
	exit(EXIT_SUCCESS);
}

/* Function: setup_pipeline
 * Sets up and executes a pipeline of commands. Handles pipe creation,
 * forking processes, and managing file descriptors.
 */

int	setup_pipeline(t_init_input *input_list, t_envp *env_list)
{
	int				pipe_fd[2];
	pid_t			pid;
	t_init_input	*current;
	t_types			*types;

	types = init_types();
	current = input_list;
	while (current)
	{
		if (current->next && current->next->token == 11)
		{
			if (pipe(pipe_fd) == -1)
				return (perror("Error creating pipe"), -1);
			current->fd_out = pipe_fd[1];
			current->next->fd_in = pipe_fd[0];
			current->next->next->fd_in = pipe_fd[0];
		}
		pid = fork();
		if (pid == -1)
			return (perror("Error during fork"), -1);
		if (pid == 0)
			handle_child_process(current, input_list, types, env_list);
		current = handle_parent_process(current);
	}
	while (wait(NULL) > 0);
	return (0);
}
