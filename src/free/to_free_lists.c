/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_free_lists.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:30:34 by jparnahy          #+#    #+#             */
/*   Updated: 2025/01/07 18:31:17 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Function: free_list
 * Iterates through a linked list of t_init_input, freeing the string
 * and each node one by one.
 */

void	free_list(t_init_input *list)
{
	t_init_input	*temp;

	if (!list)
		return ;
	while (list)
	{
		temp = list->next;
		if (list->string)
		{
			free(list->string);
			list->string = NULL;
		}
		free_list_args(list->args);
		free(list);
		list = temp;
	}
}

/* Function: free_types
 * Iterates through a linked list of t_types, freeing the cmd field
 * and each node individually.
 */

void	free_types(t_types **head)
{
	t_types	*tmp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		tmp = (*head)->next;
		if ((*head)->cmd)
			free((*head)->cmd);
		free(*head);
		*head = tmp;
	}
	*head = NULL;
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
	env_list = NULL;
}
