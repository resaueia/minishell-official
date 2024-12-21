/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:55:24 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/21 19:40:45 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*identify heredoc:
if heredoc, get a function to process it. how?
start capturing all input lines from the user until you come across
the initial delimiter.
write said lines in a pipe - pipe needs to get redirectioned as an entry for
the cmd; finally, those lines will get written on the terminal once said cmd
is executed.
*/

/* Function: write_line_to_file
 * Writes a single line and a newline character to the given file descriptor.
 */

void	write_line_to_file(int temp_fd, char *line)
{
	write(temp_fd, line, ft_strlen(line));
	write(temp_fd, "\n", 1);
}

void	single_delimiters(int temp_fd, char *last_delim, int start_write)
{
	char	*line;

	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			printf("minishell: warning: ");
			printf("here-document delimited by end-of-file ");
			printf("(wanted `%s')\n", last_delim);
			break ;
		}
		if (ft_strcmp(line, last_delim) == 0 && start_write == 1)
		{
			free(line);
			break ;
		}
		if (start_write == 1)
			write_line_to_file(temp_fd, line);
		free(line);
		start_write = 1;
	}
}

void	multiple_delimiters(int temp_fd, char *start_delim,
		char *last_delim, int start_write)
{
	char	*line;

	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			perror("Error reading line");
			break ;
		}
		if (ft_strcmp(line, last_delim) == 0 && start_write == 1)
		{
			free(line);
			break ;
		}
		if (start_delim == NULL || ft_strcmp(line, start_delim) == 0)
			start_write = 1;
		else if (start_write == 1)
		{
			write_line_to_file(temp_fd, line);
		}
		free(line);
	}
}

/* Function: process_lines
 * Reads user input line by line, processes delimiters, and writes to
 * the temporary file as necessary.
 */

void	process_lines(int temp_fd, char *start_delim, char *last_delim)
{
	if (start_delim == NULL)
		single_delimiters(temp_fd, last_delim, 1);
	else
		multiple_delimiters(temp_fd, start_delim, last_delim, 2);
}

/* Function: tackle_heredoc
 * Handles heredoc functionality by creating a temporary file, reading
 * user input, and updating the command list with the file path.
 */

int	tackle_heredoc(t_types *type, char *start_delim, char *last_delim)
{
	char	temp_file[64];
	int		temp_fd;
	int		pid;

	pid = getpid();
	create_temp_filename(temp_file, pid);
	temp_fd = open_temp_file(temp_file);
	if (temp_fd == -1)
		return (-1);
	process_lines(temp_fd, start_delim, last_delim);
	change_value(type, temp_file);
	return (temp_fd);
}
