/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:03:25 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/21 18:44:59 by jparnahy         ###   ########.fr       */
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

	i = 0;
	while (str[i])
	{
		str[i] = free_char_ptr(str[i]);
		i++;
	}
	free(str);
	str = NULL;
	return (NULL);
}

/* Function: free_list
 * Iterates through a linked list of t_init_input, freeing the string
 * and each node one by one.
 */
void	free_list(t_init_input *list)
{
	t_init_input	*temp;

	while (list)
	{
		temp = list;
		list = list->next;
		if (temp->string)
			free(temp->string);
		free(temp);
	}
}

/* Function: free_types
 * Iterates through a linked list of t_types, freeing the cmd field
 * and each node individually.
 */
void	free_types(t_types *types)
{
	t_types	*temp;

	while (types)
	{
		temp = types;
		types = types->next;
		if (temp->cmd)
			free(temp->cmd);
		free(temp);
	}
}

/* Function: free_env
 * Iterates through a linked list of t_envp, freeing both key and value
 * fields, and then frees each node.
 */
void	free_env(t_envp *env_list)
{
	t_envp	*temp;

	while (env_list)
	{
		temp = env_list;
		env_list = env_list->next;
		if (temp->key)
			free(temp->key);
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}
