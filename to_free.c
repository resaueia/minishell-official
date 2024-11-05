/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:03:25 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/04 22:12:32 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *free_char_ptr(char *ptr)
{
    if (ptr)
        free(ptr);
    ptr = NULL;
    return (ptr);
}

char	**free_from_split(char **str)
{
	int i;

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

void	free_list(t_init_input *list)
{
	t_init_input *temp;

	while (list)
	{
		temp = list;
		list = list->next;
		if (temp->string)
			free(temp->string);
		free(temp);
	}
}

void	free_env(t_envp *env_list)
{
	t_envp *temp;

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

void	exit_mini(t_init_input *list, char *prompt, char *prompt_dup, t_envp *env_list)
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

