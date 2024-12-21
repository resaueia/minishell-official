/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:19:50 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/21 19:46:35 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_with_error(const char *message)
{
	if (message && *message)
	{	
		last_status(1);
		printf("minishell: %s: %s\n", message, strerror(errno));
	}
	else
	{
		last_status(1);
		printf("minishell: %s\n", strerror(errno));
	}
	exit(0);
}

char	*ft_strtok_r(char *str, char *delim, char **save_ptr)
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

void	construct_path(char *full_path, char *dir, char *cmd)
{
	size_t	dir_len;
	size_t	cmd_len;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	ft_strlcpy(full_path, dir, dir_len + 1);
	ft_strlcat(full_path, "/", dir_len + 2);
	ft_strlcat(full_path, cmd, dir_len + cmd_len + 2);
}

int	generate_full_path(char **full_path, char *dir, char *cmd)
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

char	*duplicate_path(char *path)
{
	char	*path_dup;

	path_dup = ft_strdup(path);
	if (!path_dup)
		exit_with_error("Error duplicating PATH");
	return (path_dup);
}
