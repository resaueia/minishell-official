/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delim_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:40:07 by rsaueia           #+#    #+#             */
/*   Updated: 2024/12/21 19:56:53 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_pipe_token(t_init_input **head, t_init_input **tail,
		char *s, size_t i)
{
	extract_token(head, tail, s, i, i + 1);
}

/* Function: process_pipe_token
 * Handles the processing of pipe or operator tokens by extracting
 * and adding them to the linked list.
 */

void	process_current_character(t_init_input **head, t_init_input **tail,
		char *s, int *start, size_t i)
{
	if (is_pipe(s[i]) || s[i + 1] == '\0')
	{
		if (*start >= 0)
		{
			extract_token(head, tail, s, *start, i + (s[i + 1] == '\0'));
			*start = -1;
		}
		if (is_pipe(s[i]))
			process_pipe_token(head, tail, s, i);
	}
}

/* Function: process_current_character
 * Processes the current character in the input string `s`. If it is a
 * pipe or the end of the string, it handles token extraction and delimiter
 * processing. Resets the start index after processing a WORD token.
 */

t_init_input	*delim_split(char *s)
{
	t_init_input	*head;
	t_init_input	*tail;
	size_t			i;
	int				start;

	head = NULL;
	tail = NULL;
	i = 0;
	start = -1;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (!is_pipe(s[i]) && start < 0)
			start = i;
		else
			process_current_character(&head, &tail, s, &start, i);
		i++;
	}
	return (head);
}

/* Function: delim_split
 * Splits the input string `s` into tokens based on delimiters like `|`, `>`,
 * and `<`. Adds each token to a linked list and returns the head of the list.
 */
