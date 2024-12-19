/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:55:24 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/18 22:39:49 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*identify heredoc:
if heredoc, get a function to process it. how?
start capturing all input lines from the user until you come across
the initial delimiter.
write said lines in a pipe
	- pipe needs to get redirectioned as an entry for the cmd;
finally, those lines will get written on the terminal once said cmd is executed.
*/

static void	include_eof(t_types *cmd)
{
	t_types	*head;

	head = cmd;
	while (cmd)
	{
		if (ft_strcmp(cmd->cmd, "<<") == 0 && cmd->next->cmd)
		{
			cmd = cmd->next;
			cmd->type = ENDOF;
		}
		cmd = cmd->next;
	}
	cmd = head;
}

static char	**split_heredoc(t_types *type, int here_count)
{
	t_types	*head;
	char	**delim;
	int		i;

	printf("here_count: [%d]\n", here_count);
	(void)head;
	head = type;
	delim = (char **)malloc(sizeof(*delim) * (here_count + 1));
	i = 0;
	while (type)
	{
		if (ft_strcmp(type->cmd, "<<") == 0 && type->next->cmd)
		{
			delim[i] = ft_strdup(type->next->cmd);
			i++;
		}
		type = type->next;
	}
	delim[i] = NULL;
	return (delim);
}

static void	change_value(t_types *type, char *value)
{
	t_types	*head;

	head = type;
	while (type)
	{
		if (ft_strcmp(type->cmd, "<<") == 0 && type->next->cmd)
		{
			free(type->cmd);
			type->cmd = ft_strdup(value);
			type->next->cmd = NULL;
			return ;
		}
		type = type->next;
	}
	type = head;
}

static int	count_heredocs(t_types *type)
{
	int	here_count;

	here_count = 0;
	while (type)
	{
		if (ft_strcmp(type->cmd, "<<") == 0 && type->next->cmd)
			here_count++;
		type = type->next;
	}
	return (here_count);
}

/* Function: count_heredocs
 * Counts the number of heredoc delimiters in the command list.
 */

static int	process_single_heredoc(t_types *type, t_types *head)
{
	int	heredoc_fd;

	include_eof(type);
	while (type)
	{
		if (ft_strcmp(type->cmd, "<<") == 0 && type->next->cmd)
		{
			heredoc_fd = tackle_heredoc(head, NULL, type->next->cmd);
			if (heredoc_fd == -1)
			{
				perror("Error setting up heredoc");
				return (-1);
			}
			type->fd[0] = heredoc_fd;
			return (1);
		}
		type = type->next;
	}
	return (0);
}

/* Function: process_single_heredoc
 * Handles the setup and processing of a single heredoc, updating
 * the file descriptor with the temporary file.
 */

static int	process_multiple_heredocs(t_types *type, t_types *head,
		int here_count)
{
	char	**delim;
	int		heredoc_fd;

	include_eof(type);
	delim = split_heredoc(type, here_count);
	while (type)
	{
		if (ft_strcmp(type->cmd, "<<") == 0 && type->next->cmd)
		{
			heredoc_fd = tackle_heredoc(head, delim[here_count - 2],
					delim[here_count - 1]);
			if (heredoc_fd == -1)
			{
				perror("Error setting up heredoc");
				return (-1);
			}
			type->fd[0] = heredoc_fd;
			return (1);
		}
		type = type->next;
	}
	return (0);
}

/* Function: process_multiple_heredocs
 * Handles the setup and processing of multiple heredocs, updating
 * the file descriptor with the temporary file.
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

/* Function: is_heredoc
 * Determines if heredoc processing is required, and handles either
 * a single heredoc or multiple heredocs using helper functions.
 */

static void	create_temp_filename(char *temp_file, int pid)
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

/* Function: create_temp_filename
 * Generates a unique temporary filename using the PID and stores it
 * in the provided buffer.
 */

static int	open_temp_file(char *temp_file)
{
	int	temp_fd;

	temp_fd = open(temp_file, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (temp_fd == -1)
		perror("Error creating temporary file");
	return (temp_fd);
}

/* Function: open_temp_file
 * Opens the temporary file for the heredoc and returns the file descriptor.
 */

static void	write_line_to_file(int temp_fd, char *line)
{
	write(temp_fd, line, ft_strlen(line));
	write(temp_fd, "\n", 1);
}

/* Function: write_line_to_file
 * Writes a single line and a newline character to the given file descriptor.
 */

static int	process_delimiters(char *line, char *start_delim, char *last_delim,
		int *start_write)
{
	if (ft_strcmp(line, last_delim) == 0 && *start_write == 1)
		return (1);
	if (start_delim && ft_strcmp(line, start_delim) == 0)
		*start_write = 1;
	return (0);
}

/* Function: process_delimiters
 * Manages the logic for heredoc delimiters, updating the start_write flag
 * and indicating when to stop processing lines.
 */

static void	process_lines(int temp_fd, char *start_delim, char *last_delim)
{
	char	*line;
	int		start_write;

	start_write = 0;
	if (start_delim == NULL)
		start_write = 1;
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			perror("Error reading line");
			break ;
		}
		if (process_delimiters(line, start_delim, last_delim, &start_write))
		{
			free(line);
			break ;
		}
		if (start_write == 1)
			write_line_to_file(temp_fd, line);
		free(line);
	}
}

/* Function: process_lines
 * Reads user input line by line, processes delimiters, and writes to
 * the temporary file as necessary.
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
		// Atualiza o cmd do tipo com o arquivo temporário.
	return (temp_fd);
}

/* Function: tackle_heredoc
 * Handles heredoc functionality by creating a temporary file, reading
 * user input, and updating the command list with the file path.
 */
