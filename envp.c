/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 16:02:16 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/06/24 18:10:20 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Main structure (defined on main) */

#include "minishell.h"

t_envp	*create_node(char *key, char *value)
{
	t_envp	*new_node;

	new_node = (t_envp *)malloc(sizeof(t_envp));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	return (new_node);
}

t_envp	*get_envp(char **envp)
{
	t_envp	*head;
	t_envp	*current;
	t_envp	*new_node;
	char	*input;
	char	*delim;
	char	*key;
	char	*value;
	int		key_len;
	int		i;

	head = NULL;
	current = NULL;
	while (*envp)
	{
		input = *envp;
		delim = ft_strchr(input, '=');
		if (delim)
		{
			key_len = delim - input;
			key = (char *)malloc(sizeof(char) * (key_len + 1));
			if (!key)
				return (NULL);
			for (i = 0; i < key_len; i++)
				key[i] = input[i];
			key[key_len] = '\0';
			value = ft_strdup(delim + 1);
			new_node = create_node(key, value);
			if (!head)
				head = new_node;
			else
				current->next = new_node;
			current = new_node;
			free(key);
			free(value);
		}
		envp++;
	}
	return (head);
}

void	print_envp_list(t_envp *head)
{
	t_envp *current;

	current = head;
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}
