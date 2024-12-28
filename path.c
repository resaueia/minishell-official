/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thfranco <thfranco@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:48:55 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/26 15:22:25 by thfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_special_cases(t_envp *current, char *tmp_pwd,
		char *value_src)
{
	while (current)
	{
		if (ft_strcmp(current->key, "OLDPWD") == 0)
		{
			free(current->value);
			current->value = tmp_pwd;
		}
		else if (ft_strcmp(current->key, "PWD") == 0)
		{
			free(current->value);
			current->value = value_src;
		}
		current = current->next;
	}
}

/* Function: handle_special_cases
 * Handles special cases for `HOME` and `OLDPWD`, updating the environment
 * variables list with appropriate values.
 */

static void	update_pwd_and_oldpwd(t_envp *current, char *tmp_pwd, char *new_pwd)
{
	while (current)
	{
		if (ft_strcmp(current->key, "OLDPWD") == 0)
		{
			free(current->value);
			current->value = tmp_pwd;
		}
		else if (ft_strcmp(current->key, "PWD") == 0)
		{
			free(current->value);
			if (new_pwd)
				current->value = ft_strdup(new_pwd);
			else
				current->value = NULL;
		}
		current = current->next;
	}
}

/* Function: update_pwd_and_oldpwd
 * Updates the values of `PWD` and `OLDPWD` in the environment variables list.
 * Uses `tmp_pwd` for the old value and `new_pwd` for the new value of `PWD`.
 * No ternary operators are used.
 */

char	*change_path(char *path, char *src, t_envp **head)
{
	t_envp	*current;
	char	*tmp_pwd;
	char	*value_src;

	current = *head;
	tmp_pwd = get_value("PWD", current);
	value_src = get_value(src, current);
	if (ft_strcmp("HOME", src) == 0 || ft_strcmp("OLDPWD", src) == 0)
	{
		(void)path;
		handle_special_cases(current, tmp_pwd, value_src);
	}
	else if (ft_strcmp("PWD", src) == 0)
	{
		update_pwd_and_oldpwd(current, tmp_pwd, path);
	}
	return (value_src);
}

/* Function: change_path
 * Updates the values of `PWD` and `OLDPWD` in the environment variables list
 * based on the provided `src`. Handles special cases for `HOME` and `OLDPWD`.
 * Returns the value associated with `src`.
 */
