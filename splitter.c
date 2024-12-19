/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 18:06:59 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/18 22:31:37 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_delimiter(char c)
{
	return (c == ' ' || (c > 8 && c < 14));
}

char	*custom_dup(char *str, int start, int finish)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc(sizeof(char) * (finish - start + 1));
	while (start < finish)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

t_init_input	*add_node(char *input, t_token token)
{
	t_init_input	*new_node;

	new_node = (t_init_input *)malloc(sizeof(t_init_input));
	if (!new_node)
		return (NULL);
	new_node->string = ft_strdup(input);
	new_node->token = token;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}

static t_init_input	*make_new_node(char *substr)
{
	t_token			token;
	t_init_input	*new_node;

	token = get_token(substr);
	new_node = add_node(substr, token);
	free(substr);
	return (new_node);
}

static t_init_input	*handle_new_node(t_init_input *tail, t_init_input *new_node)
{
	if (!tail)
		return (new_node);
	tail->next = new_node;
	new_node->prev = tail;
	return (new_node);
}

void	handle_substring(char *s, size_t *i, int *start_index,
		t_init_input **head, t_init_input **tail)
{
	char			*substr;
	t_init_input	*new_node;

	substr = custom_dup(s, *start_index, *i + (s[*i + 1] == '\0'));
	if (!substr)
		return ;
	new_node = make_new_node(substr);
	if (!new_node)
		return ;
	*tail = handle_new_node(*tail, new_node);
	if (!*head)
		*head = *tail;
	*start_index = -1;
}
/* Function: handle_substring
 * Extracts a substring from the input string, creates a new node, and adds
 * it to the linked list. The start index is reset after processing.
 */

t_init_input	*ft_split(char *s)
{
	t_init_input	*head;
	t_init_input	*tail;
	size_t			i;
	int				start_index;

	head = NULL;
	tail = NULL;
	i = 0;
	start_index = -1;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (!is_delimiter(s[i]) && start_index < 0)
			start_index = i;
		else if ((is_delimiter(s[i]) || s[i + 1] == '\0') && start_index >= 0)
			handle_substring(s, &i, &start_index, &head, &tail);
		i++;
	}
	return (head);
}
/* Function: ft_split
 * Splits the input string into nodes based on delimiters (spaces or control
 * characters). Each substring is stored as a node in a linked list.
 * The function uses helper functions to handle substring extraction and node
 * creation, ensuring clarity and modularity.
 */