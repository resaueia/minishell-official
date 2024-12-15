/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 18:06:59 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/14 20:01:37 by jparnahy         ###   ########.fr       */
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

//auxiliary function to create new node
static t_init_input	*make_new_node(char *substr)
{
	t_token			token;
	t_init_input	*new_node;
	
	token = get_token(substr);
	new_node = add_node(substr, token);
	free(substr);
	return (new_node);
}

//auxiliary function to add the new node to the list
static t_init_input	*handle_new_node(t_init_input *tail, t_init_input *new_node)
{
	if (!tail)
		return (new_node);
	tail->next = new_node;
	new_node->prev = tail;
	return (new_node);
}

//auxiliary function to check and process the delimiter
static void	process_delimiter(char *s, size_t *i, int *start_index, t_init_input **head, t_init_input **tail)
{
	char	*substr;

	if (*start_index >= 0)
	{
		substr = custom_dup(s, *start_index, *i + (s[*i + 1] == '\0'));
		if (!substr)
			return ;
		*tail = handle_new_node(*tail, make_new_node(substr));
		if (!*head)
			*head = *tail;
		*start_index = -1;
	}
}

//main function to split the string into nodes
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
			process_delimiter(s, &i, &start_index, &head, &tail);
		i++;
	}
	return (head);
}
