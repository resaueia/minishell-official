/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:57:07 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/20 18:03:16 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Function: remove_heredoc_file
 * Constructs the full path of a heredoc file and attempts to delete it.
 * Handles errors related to path truncation and file deletion.
 */

static void	remove_heredoc_file(const char *filename)
{
	char	filepath[64];

	if (ft_strlcpy(filepath, "/tmp/", sizeof(filepath)) >= sizeof(filepath))
	{
		printf("Path truncation error: %s: %s\n", "/tmp/", strerror(errno));
		return ;
	}
	if (ft_strlcat(filepath, filename, sizeof(filepath)) >= sizeof(filepath))
	{
		printf("Path truncation error: %s: %s\n", filename, strerror(errno));
		return ;
	}
	if (unlink(filepath) == -1)
		perror("Failed to delete heredoc file");
}

/* Function: clear_heredoc_files
 * Iterates over the files in the /tmp directory and deletes any files
 * that start with the "hdoc_" prefix using the remove_heredoc_file function.
 */

void	clear_heredoc_files(void)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir("/tmp");
	if (!dir)
	{
		perror("Failed to open /tmp directory");
		return ;
	}
	while ((entry = readdir(dir)) != NULL)
	{
		if (ft_strncmp(entry->d_name, "hdoc_", 5) == 0)
			remove_heredoc_file(entry->d_name);
	}
	closedir(dir);
}
