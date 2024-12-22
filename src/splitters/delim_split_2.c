/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delim_split_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:56:18 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/21 19:56:58 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_pipe(char c)
{
	return (c == '|');
}

t_token	get_token(char *c)
{
	int	i;

	i = 0;
	if (c[i] == '|' && c[i + 1] != '|')
		return (PIPE);
	else if (c[i] == '>' && c[i + 1] == '>')
		return (APPEND);
	else if (c[i] == '<' && c[i + 1] == '<')
		return (HDOC);
	else if (c[i] == '>' && c[i + 1] != '>')
		return (OUT);
	else if (c[i] == '<' && c[i + 1] != '<')
		return (IN);
	else if (is_builtin(c))
		return (BUILTIN);
	else
		return (WORD);
}

void	add_to_list(t_init_input **head, t_init_input **tail, char *substr,
		t_token token)
{
	t_init_input	*new_node;

	new_node = add_node(substr, token);
	if (!new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		*tail = new_node;
	}
	else
	{
		(*tail)->next = new_node;
		new_node->prev = *tail;
		*tail = new_node;
	}
}

void	extract_token(t_init_input **head, t_init_input **tail, char *s,
		int start, int end)
{
	char	*token;

	token = custom_dup(s, start, end);
	add_to_list(head, tail, token, get_token(token));
	free(token);
}
/* Function: extract_token
 * Extracts a token from the string `s` between indices `start` and `end`,
 * determines its type, and adds it to the linked list.
 */