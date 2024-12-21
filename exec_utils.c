/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:17:37 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/20 19:12:20 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_with_error(const char *message)
{
	if (message && *message)
		printf("minishell: %s: %s\n", message, strerror(errno));
	else
		printf("minishell: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
}

static char	*ft_strtok_r(char *str, char *delim, char **save_ptr)
{
	char	*start;

	if (str)
		*save_ptr = str;
	if (!*save_ptr)
		return (NULL);
	start = *save_ptr;
	while (*start && ft_strchr(delim, *start))
		start++;
	if (!*start)
		return (*save_ptr = NULL, NULL);
	*save_ptr = start;
	while (**save_ptr && !ft_strchr(delim, **save_ptr))
		(*save_ptr)++;
	if (**save_ptr)
	{
		**save_ptr = '\0';
		(*save_ptr)++;
	}
	else
		*save_ptr = NULL;
	return (start);
}

static void	construct_path(char *full_path, char *dir, char *cmd)
{
	size_t	dir_len;
	size_t	cmd_len;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	ft_strlcpy(full_path, dir, dir_len + 1);
	ft_strlcat(full_path, "/", dir_len + 2);
	ft_strlcat(full_path, cmd, dir_len + cmd_len + 2);
}

static int	generate_full_path(char **full_path, char *dir, char *cmd)
{
	*full_path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
	if (!*full_path)
	{
		perror("Error allocating memory");
		return (-1);
	}
	construct_path(*full_path, dir, cmd);
	return (0);
}

static int	check_access_and_update(char **cmd, char *full_path, char *path_dup)
{
	if (access(full_path, X_OK) == 0)
	{
		free(*cmd);
		*cmd = full_path;
		free(path_dup);
		return (1);
	}
	free(full_path);
	return (0);
}

/* Function: check_access_and_update
 * Checks if the constructed path is executable and updates the `cmd`.
 * Returns 1 if the path is valid and updated, otherwise returns 0.
 */

static int	process_path_directory(char *dir, char **cmd, char *path_dup)
{
	char	*full_path;

	if (generate_full_path(&full_path, dir, *cmd) == -1)
		exit_with_error("Error constructing path");
	if (check_access_and_update(cmd, full_path, path_dup))
		return (1);
	return (0);
}

/* Function: process_path_directory
 * Processes a single directory in the PATH, attempting to construct and verify
 * the full path for the command. Returns 1 if a valid command is found,
	otherwise 0.
 */

void	find_command_path(t_types *type, t_envp *env_list)
{
	char	*path;
	char	*path_dup;
	char	*dir;
	char	*save_ptr;

	path = get_value("PATH", env_list);
	if (!path)
	{
		last_status(1);
		printf("minishell: No such file or directory: %s\n", type->cmd);
		return ;
	}
	path_dup = ft_strdup(path);
	if (!path_dup)
		exit_with_error("Error duplicating PATH");
	dir = ft_strtok_r(path_dup, ":", &save_ptr);
	while (dir)
	{
		if (process_path_directory(dir, &type->cmd, path_dup))
			return ;
		dir = ft_strtok_r(NULL, ":", &save_ptr);
	}
	free(path_dup);
}

/* Function: find_command_path

* Searches for the command in the directories specified by the PATH 
environment variable.
* Updates the command with the valid full path if found,
or prints an error message.
*/

static void	setup_io_redirection(t_types *type)
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
			exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
	}
}
