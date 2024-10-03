/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:59:21 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/10/03 17:03:15 by jparnahy         ###   ########.fr       */
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

void	ft_echo(char *args, t_envp **env_list)
{
	(void)env_list;
	int	newline;
	char	dollar;

	newline = 1;
	dollar = '$';
	while (*args == ' ' )
		args++;
	if (args == NULL) //if echo come without args, it will print just a newline
	{
		printf("\n");
		return ;
	}
	if (ft_strncmp(args, "-n", 2) == 0) //if echo come with -n, it will not print a newline
	{
		newline = 0; //flag to not print a newline
		args += 3; //incrementing the pointer to the next character for check next conditions
	}
	remove_quotes(&args); //to remove quotes from the args
	if (*args == '$') //if echo come with $, it will print the value of the env variable
	{
		args++; //incrementing the pointer to the next character for check next conditions
		if (*args == '\0') //if echo come with $ and no args, it will print just a char '$'
			args = &dollar; //changing the pointer to the char '$'
		else //if echo come with $ and args, need check what kind of args it is
		{
			if (ft_islower(args) == 1) //if echo come with $ and args in lower case, it will print just a newline
			{
				printf("\n");
				return ;
			}
			else if (ft_islower(args) == 0) //if echo come with $ and args in upper case, it will check if is a key of env list
			{
				if (is_key(args, *env_list) == 1) //if is a key, it will get the value of the key
					args = get_value(args, *env_list); //changing the pointer to the value of the key
				else //if is not a key, it will print just a newline
				{
					printf("\n");
					return ;
				}
			}
		}
	}
	if (newline == 1) //if newline is 1, it will print a newline
		printf("%s\n", args); //printing the args with a newline
	else if (newline == 0) //if newline is 0, it will not print a newline
		printf("%s", args); //printing the args without a newline
}

void	ft_cd(char *path, t_envp **env_list)
{
	while (*path == ' ' )
		path++;
	if (ft_strlen(path) >= 2)
	{
		if (ft_strncmp(path, "~/", 2) == 0)
			path = ft_joinpath(path + 2, "HOME", env_list);
		if (chdir(path) == 0)
		{
			char	cwd[1024];
			getcwd(cwd, sizeof(cwd)); // entrada do path a ser utilizado para alterar envp
			change_path(cwd, "PWD", env_list); //alterar o value do pwd do env, e atualizar o path do oldpwd
		}
		else 
			printf("cd: %s: %s\n", strerror(errno), path);
	}
	else if (!*path || *path == '~') //caminho para HOME
	{
		path = change_path(path, "HOME", env_list); //alterar o value do pwd do env, path para home
		chdir(path); //aplicar chdir para modificação do diretório.
	}
	else if (*path == '-') //caminho para o último diretório
	{
		path = change_path(path, "OLDPWD", env_list); //alterar o value do pwd do env, recebendo o path do oldpwd
		chdir(path);//aplicar chdir para modificação do diretório.
	}
}

void	ft_export(char *var, t_envp **env_list)
{
	printf("ptr of env_list: [%p]\n", *env_list);
	while (*var == ' ' )
		var++;
	char	*delim;
	t_envp	*new_node;
	t_envp	*current;

	delim = ft_strchr(var, '=');
	if (delim)
	{
		printf("delim: [%s]\n", delim);
		*delim = '\0';
		printf("delim: [%s]\n", delim);
		current = *env_list;
		printf("ptr of current: [%p]\n", current);
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
		printf("current: [%p]\n", current);
		printf("var is: [%s]\n", var);
		printf("delim is: [%s]\n", delim + 1);
		// If 'key' is not present, we create it, by adding a new node to our var list.
		new_node = create_new_node(env_list, var, delim + 1);
		printf("new_node->key: [%s]\n", new_node->key);
		printf("new_node->value: [%s]\n", new_node->value);
		printf("new_node->next: [%p]\n", new_node->next);
		printf("ptr of env_list: [%p]\n", *env_list);
		printf("new_node: [%p]\n", new_node);
		/*new_node->next = *env_list;
		printf("ptr of env_list: [%p]\n", env_list);
		printf("new_node->next: [%p]\n", new_node->next);
		*env_list = new_node;
		printf("new_node: [%p]\n", new_node);*/
		printf("ptr of env_list: [%p]\n", *env_list);
		/*printf("------------\n");
		print_envp_list(*env_list);
		printf("------------\n");*/
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