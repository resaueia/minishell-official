/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 16:02:16 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/21 18:49:34 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Main structure (defined on main) */

#include "minishell.h"

static int	extract_key_value(char *input, char **key, char **value)
{
	char	*delim;
	int		key_len;
	int		i;

	i = 0;
	delim = ft_strchr(input, '=');
	if (!delim)
		return (0);
	key_len = delim - input;
	*key = (char *)malloc(sizeof(char) * (key_len + 1));
	if (!*key)
		return (0);
	while (i < key_len)
	{
		(*key)[i] = input[i];
		i++;
	}
	(*key)[key_len] = '\0';
	*value = ft_strdup(delim + 1);
	if (!*value)
	{
		free(*key);
		return (0);
	}
	return (1);
}

/* Function: extract_key_value

* Extracts the `key` and `value` from an input string in the format
`key=value`. Returns 1 on success and 0 on failure.
 */

static void	add_envp_node(t_envp **head, t_envp **current, char *key,
		char *value)
{
	t_envp	*new_node;

	new_node = create_node(key, value);
	if (!*head)
		*head = new_node;
	else
		(*current)->next = new_node;
	*current = new_node;
}

/* Function: add_envp_node
 * Creates and adds a new node to the `envp` linked list.
 */

t_envp	*get_envp(char **envp)
{
	t_envp	*head;
	t_envp	*current;
	char	*key;
	char	*value;

	head = NULL;
	current = NULL;
	while (*envp)
	{
		if (extract_key_value(*envp, &key, &value))
		{
			add_envp_node(&head, &current, key, value);
			free(key);
			free(value);
		}
		envp++;
	}
	return (head);
}

/* Function: get_envp
 * Constructs a linked list of environment variables from the `envp` array.
 */