/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:03:25 by jparnahy          #+#    #+#             */
/*   Updated: 2025/01/07 19:36:59 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Function: free_char_ptr
 * Safely frees a dynamically allocated character pointer and sets it to NULL.
 */
char	*free_char_ptr(char *ptr)
{
	if (ptr)
		free(ptr);
	ptr = NULL;
	return (ptr);
}

/* Function: free_from_split
 * Frees an array of strings (result of a split function), including each
 * individual string, and sets the array pointer to NULL.
 */
char	**free_from_split(char **str)
{
	int	i;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	free(str);
	str = NULL;
	return (NULL);
}

void	free_list_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
	args = NULL;
}

t_types	*remove_null_nodes(t_types *head)
{
	t_types	*current;
	t_types	*to_remove;

	current = head;
	to_remove = NULL;
	while (current)
	{
		if (current->cmd == NULL)
		{
			to_remove = current;
			if (to_remove->prev)
				to_remove->prev->next = to_remove->next;
			else
				head = to_remove->next;
			if (to_remove->next)
				to_remove->next->prev = to_remove->prev;
			current = to_remove->next;
			free(to_remove);
		}
		else
			current = current->next;
	}
	return (head);
}

void	free_heredoc_delims(char **delim)
{
	int	i;

	i = 0;
	if (!delim)
		return ;
	while (delim[i])
	{
		free(delim[i]);
		i++;
	}
	free(delim);
}
