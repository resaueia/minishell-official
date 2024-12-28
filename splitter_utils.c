/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thfranco <thfranco@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 18:06:59 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/22 19:13:24 by thfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	new_node->args = NULL;
	new_node->token = token;
	new_node->types = NULL;
	new_node->prev = NULL;
	new_node->next = NULL;
	new_node->exit_status = 0;
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

/* Function: handle_substring
 * Extracts a substring from the input string, creates a new node, and adds
 * it to the linked list. The start index is reset after processing.
 */

void	handle_substring(char *s, size_t *i, int *start_index,
		t_init_input **head)
{
	char			*substr;
	t_init_input	*new_node;
	t_init_input	*tail;

	tail = NULL;
	substr = custom_dup(s, *start_index, *i + (s[*i + 1] == '\0'));
	if (!substr)
		return ;
	new_node = make_new_node(substr);
	if (!new_node)
		return ;
	tail = handle_new_node(tail, new_node);
	if (!*head)
		*head = tail;
	*start_index = -1;
}
