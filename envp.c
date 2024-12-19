/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 16:02:16 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/18 19:47:19 by rsaueia          ###   ########.fr       */
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
void	*create_new_node(t_envp **env_list, char *key, char *value)
{
	t_envp	*new_node;
	t_envp	*current;

	current = *env_list;
	new_node = (t_envp *)malloc(sizeof(t_envp));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	if (!current)
	{
		current = new_node;
	}
	else
	{
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
	return (new_node);
}
int	is_key(char *key, t_envp *head)
{
	t_envp *current;

	current = head;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}
char	*get_value(char *name, t_envp *list)
{
	t_envp	*current;
	char	*new_value;

	current = list;
	while (current)
	{
		if (ft_strcmp(current->key, name) == 0)
			new_value = ft_strdup(current->value);			
		current = current->next;
	}
	return (new_value);
}

static int extract_key_value(char *input, char **key, char **value)
{
    char *delim;
    int key_len;
    int i;

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
 * Extracts the `key` and `value` from an input string in the format `key=value`.
 * Returns 1 on success and 0 on failure.
 */

static void add_envp_node(t_envp **head, t_envp **current, char *key, char *value)
{
    t_envp *new_node;

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

t_envp *get_envp(char **envp)
{
    t_envp *head;
    t_envp *current;
    char *key;
    char *value;

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

static void handle_special_cases(t_envp *current, char *tmp_pwd, char *value_src)
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

static void update_pwd_and_oldpwd(t_envp *current, char *tmp_pwd, char *new_pwd)
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

char *change_path(char *path, char *src, t_envp **head)
{
    t_envp *current;
    char *tmp_pwd;
    char *value_src;

    current = *head;
    tmp_pwd = get_value("PWD", current);
    value_src = get_value(src, current);

    if (ft_strcmp("HOME", src) == 0 || ft_strcmp("OLDPWD", src) == 0)
    {
        (void)path; // Path não é usado nesses casos
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
