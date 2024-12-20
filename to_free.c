/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:03:25 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/20 17:51:20 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*free_char_ptr(char *ptr)
{
	if (ptr)
		free(ptr);
	ptr = NULL;
	return (ptr);
}
/* Function: free_char_ptr
 * Safely frees a dynamically allocated character pointer and sets it to NULL.
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
/* Function: free_from_split
 * Frees an array of strings (result of a split function), including each
 * individual string, and sets the array pointer to NULL.
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
/* Function: free_list
 * Iterates through a linked list of t_init_input, freeing the string
 * and each node one by one.
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
/* Function: free_types
 * Iterates through a linked list of t_types, freeing the cmd field
 * and each node individually.
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
/* Function: free_env
 * Iterates through a linked list of t_envp, freeing both key and value
 * fields, and then frees each node.
 */

void	exit_mini(t_init_input *list, char *prompt, char *prompt_dup,
		t_envp *env_list)
{
	if (list)
		free_list(list);
	if (prompt)
		free_char_ptr(prompt);
	if (prompt_dup)
		free_char_ptr(prompt_dup);
	if (env_list)
		free_env(env_list);
}
/* Function: exit_mini
 * Frees all dynamically allocated resources used during the program's
 * execution, including input lists, prompts, and environment variables.
 */