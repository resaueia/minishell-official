/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 18:06:59 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/03 19:38:32 by rsaueia          ###   ########.fr       */
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

#include "minishell.h"

// Função auxiliar para criar um novo nó
static t_init_input *make_new_node(char *substr)
{
	t_token token;
	t_init_input *new_node;

	token = get_token(substr);
	new_node = add_node(substr, token);
	free(substr);
	return (new_node);
}

// Função auxiliar para adicionar o novo nó à lista encadeada
static t_init_input *handle_new_node(t_init_input *tail, t_init_input *new_node)
{
	if (!tail)
		return (new_node);
	tail->next = new_node;
	new_node->prev = tail;
	return (new_node);
}

// Função auxiliar para verificar e processar um delimitador
static void process_delimiter(char *s, size_t *i, int *start_index, t_init_input **head, t_init_input **tail)
{
	char *substr;

	substr = custom_dup(s, *start_index, *i + (s[*i + 1] == '\0'));
	if (!substr)
		return;
	*tail = handle_new_node(*tail, make_new_node(substr));
	if (!*head)
		*head = *tail;
	*start_index = -1;
}

// Função principal para dividir a string em nós
t_init_input *ft_split(char *s)
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


/*t_init_input	*ft_split(char *s)
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
