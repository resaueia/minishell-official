/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 20:51:06 by rsaueia-          #+#    #+#             */
/*   Updated: 2025/01/07 20:56:37 by rsaueia-         ###   ########.fr       */
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
