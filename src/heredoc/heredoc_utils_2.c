/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:39:17 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/21 19:51:21 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**split_heredoc(t_types *type, int here_count)
{
	t_types	*head;
	char	**delim;
	int		i;

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

void	change_value(t_types *type, char *value)
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

/* Function: count_heredocs
 * Counts the number of heredoc delimiters in the command list.
 */

int	count_heredocs(t_types *type)
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

/* Function: process_single_heredoc
 * Handles the setup and processing of a single heredoc, updating
 * the file descriptor with the temporary file.
 */

int	process_single_heredoc(t_types *type, t_types *head)
{
	int	heredoc_fd;

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

/* Function: process_multiple_heredocs
 * Handles the setup and processing of multiple heredocs, updating
 * the file descriptor with the temporary file.
 */

int	process_multiple_heredocs(t_types *type, t_types *head,
		int here_count)
{
	char	**delim;
	int		heredoc_fd;

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
