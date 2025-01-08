/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:39:20 by rsaueia           #+#    #+#             */
/*   Updated: 2025/01/07 21:09:15 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pipe_creation(int *pipe_fd, t_init_input *input_list)
{
	if (pipe(pipe_fd) == -1)
	{
		perror("Error creating pipe");
		free_types(&input_list->types);
		return (-1);
	}
	return (0);
}

int	check_pipe(t_init_input *current, t_init_input *input_list, int *pipe_fd)
{
	if (current->next && current->next->token == 11)
	{
		if (handle_pipe_creation(pipe_fd, input_list) == -1)
			return (-1);
	}
	else
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
	}
	return (0);
}

int	handle_fork_error(t_init_input *input_list)
{
	perror("Error during fork");
	free_types(&input_list->types);
	return (-1);
}

/* Function: wait_for_children
 * Waits for all child processes to finish execution.
 */
void	wait_for_children(void)
{
	while (wait(NULL) > 0)
		;
}

/* Function: setup_pipeline
 * Sets up and executes a pipeline of commands. Handles pipe creation,
 * forking processes, and managing file descriptors.
 */
int	setup_pipeline(t_init_input *input_list, t_envp *env_list)
{
	int				pipe_fd[2];
	int				prev_fd;
	pid_t			pid;
	t_init_input	*current;

	prev_fd = -1;
	current = input_list;
	while (current)
	{
		if (check_pipe(current, input_list, pipe_fd) == -1)
			return (-1);
		pid = fork();
		if (pid == -1)
			return (handle_fork_error(input_list));
		if (pid == 0)
			handle_child_process(prev_fd, pipe_fd, current, env_list);
		handle_parent_process(&prev_fd, pipe_fd);
		if (current->next)
			current = current->next->next;
		else
			current = NULL;
	}
	wait_for_children();
	free_types(&input_list->types);
	return (0);
}
