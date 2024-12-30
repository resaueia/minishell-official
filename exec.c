/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:50:29 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/30 17:09:33 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/********** HEREDOCS **********/
// Função auxiliar para tratar heredoc
int	handle_heredoc(t_init_input *input_list, t_types *type)
{
	if (is_heredoc(input_list, type) == -1)
	{
		perror("Error setting up heredoc");
		return (1);
	}
	if (check_node(type))
	{
		//free_list(input_list);
		//free_types(&type);
		clear_heredoc_files();
	}
	return (0);
}

/********** PIPES **********/
// Função auxiliar para tratar pipes
int	handle_pipeline(t_init_input *input_list, t_envp *env_list, t_types *type)
{
	if (type)
	{
		free_types(&type);
		type = NULL;
	}
	if (setup_pipeline(input_list, env_list) == -1)
	{
		perror("Error setting up pipeline");
		free_list(input_list);
		free_types(&type);
		return (-1);
	}
	//free_list(input_list);
	//free_types(&type);
	fd_closer(input_list, type);
	if (input_list)
		free_list(input_list);
	return (0);
}

/********** REDIRECTS **********/
// Função auxiliar para tratar redirecionamentos
int	handle_redirection(t_init_input *input_list, t_types *type)
{
	if (setup_redirection(type) == -1)
	{
		perror("Error setting up redirection");
		return (-1);
	}
	if (check_node(type))
	{
		free_list(input_list);
		free_types(&type);
		return (0);
	}
	else
		remove_node(&type);
	return (0);
}

// Função auxiliar para execução de comandos
void	execute_command(t_types *type, t_envp *env_list,
		t_init_input *input_list, char **env)
{
	if (!type)
		return ;
	if (type->cmd[0] == '.' || type->cmd[0] == '/')
	{
		if (access(type->cmd, X_OK) != 0)
		{
			if (errno == EACCES)
				printf("minishell: %s: Permission denied\n", type->cmd);
			else if (errno == ENOENT)
				printf("minishell: %s: No such file or directory\n", type->cmd);
			else
				printf("minishell: %s: %s\n", strerror(errno), type->cmd);
			last_status(127);
			return ;
		}
	}
	else if (find_command_path(type, env_list))
		return ;
	exec_cmd(input_list, type, env, env_list);
	clear_heredoc_files();
}

int	to_exec(t_init_input *input_list, t_types *type, t_envp *env_list)
{
	char	**env;

	env = NULL;
	if (is_hdoc(type) && handle_heredoc(input_list, type) == -1)
		return (-1);
	if (is_rdrct(type) && handle_redirection(input_list, type) == -1)
		return (-1);
	if (is_pp(type))
		return (handle_pipeline(input_list, env_list, type));
	if (is_btin(type))
		execute_builtin(env_list, input_list, type);
	if (is_exec(type))
	{
		env = env_to_char(env_list);
		execute_command(type, env_list, input_list, env);
		env = free_from_split(env);
	}
	fd_closer(input_list, type);
	if (input_list)
	{
		free_list(input_list);
	}
	if (type)
		free_types(&type);
	return (0);
}
