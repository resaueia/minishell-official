/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:03:25 by jparnahy          #+#    #+#             */
/*   Updated: 2025/01/07 16:04:30 by rsaueia-         ###   ########.fr       */
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
/*char	**free_from_split(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		str[i] = free_char_ptr(str[i]);
		if (str[i])
		{
			free(str[i]);
			str[i] = NULL;
		}
		i++;
	}
	free(str);
	str = NULL;
	return (NULL);
}*/

char	**free_from_split(char **str)
{
	int	i;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		free(str[i]);
		str[i] = NULL;
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
/*void	free_list(t_init_input *list)
{
	while (list)
	{
		if (list->string)
			free(list->string);
		if (list)
			free(list);
		list = list->next;
	}
}*/

// void free_list(t_init_input *list)
// {
//     t_init_input *tmp;

// 	/*if (!list)
// 		return ;*/
//     while (list)
// 	{
//         tmp = list;
//         list = list->next;
//         if (tmp->string)
//             free(tmp->string);
//         free(tmp);
//     }
//     list = NULL;  // <- GARANTE QUE O PONTEIRO É INVALIDADO
// }

void	free_list_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
	args = NULL;
}

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
