/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thfranco <thfranco@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:59:21 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/26 14:35:27 by thfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(int fd_out)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putstr_fd(cwd, fd_out);
		ft_putchar_fd('\n', fd_out);
	}
	else
		perror("getcwd() incurred in unexpected error");
}

/*void	ft_echo(t_types *cmds, t_envp **env_list, int fd_out)
{
	(void)env_list;
	char	*args;
	int		newline;
	char	*tmp;

	newline = 1;
	if (cmds->cmd == NULL || cmds->next == NULL)
	{
		ft_putstr_fd("\n", fd_out);
		return ;
	}
	args = args_to_str(cmds);
	if (ft_strncmp(args, "-n", 2) == 0)
	{
		tmp = args;
		args++;
		while (*args == 'n')
			args++;
		if (*args == ' ' || *args == '\0')
		{
			newline = 0;
			args++;
		}
		else
			args = tmp;
	}
	ft_putstr_fd(args, fd_out);
	if (newline == 1)
		ft_putchar_fd('\n', fd_out);
}*/

static int	validate_export(char *arg)
{
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	else
		return (1);
}

int	update_env_var(t_envp *env_list, char *key, char *value)
{
	t_envp	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return (1);
		}
		current = current->next;
	}
	return (0);
}

void	ft_export(t_types *cmds, t_envp **env_list)
{
	char	*var;
	char	*delim;

	var = args_to_str(cmds);
	if (!validate_export(var))
	{
		last_status(1);
		printf("minishell: export: `%s': not a valid identifier\n", var);
		free(var);
		return ;
	}
	delim = ft_strchr(var, '=');
	if (delim)
	{
		*delim = '\0';
		if (update_env_var(*env_list, var, delim + 1))
		{
			free(var);
			return ;
		}
		create_new_node(env_list, var, delim + 1);
	}
	else
		last_status(1);
	free(var);
}

void	ft_unset(t_types *cmds, t_envp **env_list)
{
	char	*var;
	t_envp	*current;
	t_envp	*prev;

	var = args_to_str(cmds);
	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, var) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env_list = current->next;
			free(current->key);
			free(current->value);
			free(current);
			free(var);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
