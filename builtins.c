/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:59:21 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/09/03 17:02:45 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(void)
{
	char cwd[1024]; // variable where we'll store the path of the current dir
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("getcwd() incurred in unexpected error");
}

void	ft_echo(char *args)
{
	int	newline;

	newline = 1;
	if (*args == ' ' && (args + 1 == NULL))
	{
		printf("\n");
		return ;
	}
	if (*args == ' ' && (args + 1 != NULL))
		args++;
	if (ft_strncmp(args, "\"", 1) == 0)
	{
		remove_quotes(&args);
			printf("%s$\n", args);
	}
	else if (ft_strncmp(args, "\"", 1) != 0)
	{
		if (ft_strncmp(args, "-n", 2) == 0)
			newline = 0;
		args += 3;
		if (newline == 1)
			printf("%s$\n", args);
		else if (newline == 0)
			printf("%s", args);
	}	
}

void	ft_cd(char *path)
{
	if (chdir(path) != 0)
		perror("cd incurred in an unexpected error");
}

void	ft_export(char *var, t_envp **env_list)
{
	char	*delim;
	t_envp	*new_node;
	t_envp	*current;

	delim = ft_strchr(var, '=');
	if (delim)
	{
		*delim = '\0';
		current = *env_list;
		while (current)
		{
			if (ft_strcmp(current->key, var) == 0) // Here, we traverse the list to check for the existence of 'key'. If it's already there, we update its value and return.
				{
					free(current->value);
					current->value = ft_strdup(delim + 1);
					return ;
				}
			current = current->next;
		}
		// If 'key' is not present, we create it, by adding a new node to our var list.
		new_node = create_node(var, delim + 1);
		new_node->next = *env_list;
		*env_list = new_node;
	}
}

void	ft_unset(char *var, t_envp **env_list)
{
	t_envp *current;
	t_envp *prev;

	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, var) == 0)
			// We traverse the list and look for the desired key. If found, we remove it from the list and free all memory associated with it.
			{
				if (prev)
					prev->next = current->next;
				else
					*env_list = current->next;
				free(current->key);
				free(current->value);
				free(current);
				return ;
			}
		prev = current;
		current = current->next;
	}
}