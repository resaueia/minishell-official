/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:57:07 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/21 19:41:41 by rsaueia-         ###   ########.fr       */
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
/* Function: is_heredoc
 * Determines if heredoc processing is required, and handles either
 * a single heredoc or multiple heredocs using helper functions.
 */

int	is_heredoc(t_init_input *input_list, t_types *type)
{
	t_types	*head;
	int		here_count;

	(void)input_list;
	head = type;
	here_count = count_heredocs(type);
	if (here_count == 1)
		return (process_single_heredoc(type, head));
	else if (here_count > 1)
		return (process_multiple_heredocs(type, head, here_count));
	return (0);
}

/* Function: create_temp_filename
 * Generates a unique temporary filename using the PID and stores it
 * in the provided buffer.
 */

void	create_temp_filename(char *temp_file, int pid)
{
	int	i;

	i = 0;
	temp_file[i++] = '/';
	temp_file[i++] = 't';
	temp_file[i++] = 'm';
	temp_file[i++] = 'p';
	temp_file[i++] = '/';
	temp_file[i++] = 'h';
	temp_file[i++] = 'd';
	temp_file[i++] = 'o';
	temp_file[i++] = 'c';
	temp_file[i++] = '_';
	while (pid > 0)
	{
		temp_file[i++] = (pid % 10) + '0';
		pid /= 10;
	}
	temp_file[i++] = '.';
	temp_file[i++] = 't';
	temp_file[i++] = 'x';
	temp_file[i++] = 't';
	temp_file[i] = '\0';
}

/* Function: open_temp_file
 * Opens the temporary file for the heredoc and returns the file descriptor.
 */

int	open_temp_file(char *temp_file)
{
	int	temp_fd;

	temp_fd = open(temp_file, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (temp_fd == -1)
		perror("Error creating temporary file");
	return (temp_fd);
}
