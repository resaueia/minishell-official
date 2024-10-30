/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 21:52:46 by jparnahy          #+#    #+#             */
/*   Updated: 2024/10/29 22:24:16 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
static int	is_delimiter(char c)
{
	// Checks if the character is in fact a delimiter character
	return (c == ' ' || (c > 8 && c < 14) || c == '<' || c == '>');
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

t_init_input	*ft_split(char *s)
{
	t_init_input	*head;
	t_init_input	*tail;
	t_init_input	*new_node;
	char			*substr;
	t_token			token;
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
		{
			// In case it finds a double opperand such as '>>' or '<<'
			if ((s[i] == '>' || s[i] == '<') && s[i] == s[i + 1])
				i++;
			substr = custom_dup(s, start_index, i + (s[i + 1] == '\0'));
			if (!substr)
				return (NULL);
			token = get_token(substr);
			new_node = add_node(substr, token);
			free(substr);
			if (!head)
				head = new_node;
			else
			{
				tail->next = new_node;
				new_node->prev = tail;
			}
			tail = new_node;
			start_index = -1;
		}
		i++;
	}
	return (head);
}*/

t_init_input    *parser(t_init_input *input, char **cmds)
{
    (void)input;
    (void)cmds;
    return (input);
}
