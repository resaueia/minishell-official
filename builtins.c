/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:59:21 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/09/26 15:23:26 by jparnahy         ###   ########.fr       */
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
	if (ft_strncmp(args, "-n", 2) == 0)
	{
		newline = 0;
		args += 3;
	}
	remove_quotes(&args);
	if (newline == 1)
		printf("%s\n", args);
	else if (newline == 0)
		printf("%s", args);
}

void	ft_cd(char *path, t_envp **env_list)
{
	while (*path == ' ' )
		path++;
	printf("path in: [%s]\n", path);
	if (!*path || *path == '~') //caminho para HOME
	{
		path = change_path(path, "HOME", env_list); //alterar o value do pwd do env, path para home
		chdir(path); //aplicar chdir para modificação do diretório.
		
	}
	else if (*path == '-') //caminho para o último diretório
	{
		path = change_path(path, "OLDPWD", env_list); //alterar o value do pwd do env, recebendo o path do oldpwd
		chdir(path);//aplicar chdir para modificação do diretório.
	}
	else //para as outras condições com path relativo ou absoluto
	{
		printf("no else\n");
		if (chdir(path) == 0)
		{
			char	cwd[1024];
			printf("pointer of get: [%p]\n", getcwd(cwd, sizeof(cwd))); // corrigir entrada do path a ser utilizado para alterar envp
			printf("path out: [%s]\n", cwd);
			printf("pointer of path: [%p]\n", path);
			printf("pointer of cwd: [%p]\n", &cwd);
			change_path(cwd, "PWD", env_list); //alterar o value do pwd do env, e atualizar o path do oldpwd
		}
		else 
			printf("cd: %s: %s\n", strerror(errno), path);
	}
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