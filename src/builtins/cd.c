/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:42:25 by rsaueia-          #+#    #+#             */
/*   Updated: 2025/01/07 18:58:58 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_cd_special_paths(char *path, char *src, t_envp **env_list)
{
	path = change_path(path, src, env_list);
	chdir(path);
	last_status(0);
}

/* Function: handle_cd_special_paths
 * Handles directory changes for special cases like `HOME` or `OLDPWD`.
 */

static void	handle_cd_path_change(char *path, t_envp **env_list)
{
	char	cwd[1024];
	char	*tmp_path;

	if (chdir(path) == 0)
	{
		getcwd(cwd, sizeof(cwd));
		tmp_path = change_path(cwd, "PWD", env_list);
		free(tmp_path);
		last_status(0);
	}
	else
	{
		last_status(1);
		printf("cd: %s: %s\n", strerror(errno), path);
	}
}

/* Function: handle_cd_path_change
 * Changes the current working directory to the specified path and updates
 * the `PWD` in the environment variables. Prints an error if the change fails.
 */

/*void	ft_cd(t_types *cmds, t_envp **env_list)
{
	char	*path;
	char	*tmp_pwd;

	if (check_args(cmds))
		return ;
	path = args_to_str(cmds);
	if (ft_strlen(path) == 1 && *path == '/')
	{
		chdir("/");
		tmp_pwd = change_path("/", "PWD", env_list);
		free(tmp_pwd);
	}
	else if (ft_strlen(path) >= 2)
	{
		if (ft_strncmp(path, "~/", 2) == 0)
		{
			tmp_pwd = joinpath(path + 2, "HOME", env_list);
			if (path)
				free(path);
			path = tmp_pwd;
		}
		handle_cd_path_change(path, env_list);
	}
	else if (!*path || *path == '~')
		handle_cd_special_paths(path, "HOME", env_list);
	else if (*path == '-')
		handle_cd_special_paths(path, "OLDPWD", env_list);
	free(path);
}*/

/* Function: ft_cd
 * Changes the current working directory based on the provided path.
 * Supports paths for root (`/`), specific directories, `HOME`, or `OLDPWD`.
 */

static void	ft_cd_root_or_path(char **path, t_envp **env_list)
{
	char	*tmp_pwd;

	if (ft_strlen(*path) == 1 && **path == '/')
	{
		chdir("/");
		tmp_pwd = change_path("/", "PWD", env_list);
		free(tmp_pwd);
	}
	else if (ft_strlen(*path) >= 2)
	{
		if (ft_strncmp(*path, "~/", 2) == 0)
		{
			tmp_pwd = joinpath(*path + 2, "HOME", env_list);
			free(*path);
			*path = tmp_pwd;
		}
		handle_cd_path_change(*path, env_list);
	}
}

void	ft_cd(t_types *cmds, t_envp **env_list)
{
	char	*path;

	if (check_args(cmds))
		return ;
	path = args_to_str(cmds);
	ft_cd_root_or_path(&path, env_list);
	if (!*path || *path == '~')
		handle_cd_special_paths(path, "HOME", env_list);
	else if (*path == '-')
		handle_cd_special_paths(path, "OLDPWD", env_list);
	free(path);
}
