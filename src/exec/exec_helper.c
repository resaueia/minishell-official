/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:50:21 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/22 01:14:59 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_node(t_types *type)
{
	t_types	*temp;

	temp = type;
	while (temp)
	{
		if (temp->type == 4 || temp->type == 7 || temp->type == 77
			|| temp->type == 44)
		{
			if (temp->prev == NULL)
				return (1);
		}
		temp = temp->next;
	}
	return (0);
}

/********** BUILT-INS **********/
// Função para gerenciar redirecionamento de descritores de arquivo
static void	handle_fd_redirection(t_types *types, int *saved_stdout)
{
	*saved_stdout = dup(STDOUT_FILENO);
	if (types->fd[1] != STDOUT_FILENO)
	{
		if (dup2(types->fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 error encountered in builtin");
			close(*saved_stdout);
			return ;
		}
		close(types->fd[1]);
	}
}

// Função para verificar se os argumentos do comando "env" estão corretos
static int	check_args_env(t_types *types)
{
	t_types	*temp;

	temp = types;
	while (temp)
	{
		if (ft_strcmp(temp->cmd, "env") == 0 || ft_strcmp(temp->cmd,
				"envp") == 0)
			temp = temp->next;
		else
		{
			printf("env: ‘%s’: No such file or directory\n", temp->cmd);
			last_status(127);
			return (0);
		}
	}
	return (1);
}

// Função para executar comandos internos (dividida)
static void	execute_individual_builtin(t_envp *env_list, t_init_input *list,
		t_types *types)
{
	t_envp	*tmp;

	tmp = env_list;
	if (ft_strcmp(types->cmd, "print") == 0)
		print_stack(list);
	else if (ft_strcmp(types->cmd, "env") == 0 || ft_strcmp(types->cmd,
			"envp") == 0)
	{
		if (check_args_env(types))
			print_envp_list(tmp);
	}
	else if (ft_strcmp(types->cmd, "pwd") == 0)
		ft_pwd(STDOUT_FILENO);
	else if (ft_strncmp(types->cmd, "echo", 4) == 0)
		ft_echo(types, &tmp, STDOUT_FILENO);
	else if (ft_strncmp(types->cmd, "cd", 2) == 0)
		ft_cd(types, &tmp);
	else if (ft_strncmp(types->cmd, "export", 6) == 0)
		ft_export(types, &tmp);
	else if (ft_strncmp(types->cmd, "unset", 5) == 0)
		ft_unset(types, &tmp);
}

// Função principal para executar comandos internos
void	execute_builtin(t_envp *env_list, t_init_input *list, t_types *types)
{
	int	saved_stdout;

	handle_fd_redirection(types, &saved_stdout);
	execute_individual_builtin(env_list, list, types);
	if (saved_stdout != STDOUT_FILENO)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}
