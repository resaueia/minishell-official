/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:17:37 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/22 21:08:34 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_full_path(char *dir, char *cmd)
{
	char	*full_path;

	if (generate_full_path(&full_path, dir, cmd) == -1)
		exit_with_error("Error constructing path");
	return (full_path);
}

void	find_command_path(t_types *type, t_envp *env_list)
{
	printf("cmd: %s\n", type->cmd);
	char	*path;
	char	*path_dup;
	char	*dir;
	char	*save_ptr;
	char	*full_path;

	path = get_value("PATH", env_list);
	printf("path: %s\n", path);
	if (!path)
	{
		last_status(127);
		printf("minishell: %s: %s\n", strerror(errno), type->cmd);
		return ;
	}
	path_dup = duplicate_path(path);
	dir = ft_strtok_r(path_dup, ":", &save_ptr);
	while (dir)
	{
		full_path = build_full_path(dir, type->cmd);
		if (check_access_and_set(full_path, type, &path_dup))
			return ;
		dir = ft_strtok_r(NULL, ":", &save_ptr);
	}
	printf("minishell: %s: %s\n", strerror(errno), type->cmd);
	free(path_dup);
}

void	setup_io_redirection(t_types *type)
{
	if (type->fd[0] != STDIN_FILENO)
	{
		if (dup2(type->fd[0], STDIN_FILENO) == -1)
			exit_with_error("dup2 fd_in failed");
		close(type->fd[0]);
	}
	if (type->fd[1] != STDOUT_FILENO)
	{
		if (dup2(type->fd[1], STDOUT_FILENO) == -1)
			exit_with_error("dup2 fd_out failed");
		close(type->fd[1]);
	}
}

int	check_access_and_set(char *full_path, t_types *type,
char **path_dup)
{
	if (access(full_path, X_OK) == 0)
	{
		free(type->cmd);
		type->cmd = full_path;
		free(*path_dup);
		return (1);
	}
	free(full_path);
	return (0);
}

void	exec_cmd(t_init_input *cmd, t_types *type, char **env)
{
	char	**args;
	pid_t	pid;
	int		status;

	(void)cmd;
	args = types_to_char(type);
	pid = fork();
	if (pid == -1)
		exit_with_error("Fork failed");
	if (pid == 0)
	{
		setup_io_redirection(type);
		if (execve(type->cmd, args, env) == -1)
			exit(127);
	}
	else
	{
		waitpid(pid, &status, 0);
		status = WEXITSTATUS(status);
		last_status(status);
	}
	free_from_split(args);
}
