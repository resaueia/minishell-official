/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:50:29 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/20 16:29:17 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int  check_node(t_types *type)
{
    t_types *temp;

    temp = type;
    while (temp)
    {
        if (temp->type == 4 || temp->type == 7 || temp->type == 77 || temp->type == 44)
        {
            if (temp->prev == NULL)
                return (1);
        }
        temp = temp->next;
    }
    return (0);
}

/********** HEREDOCS **********/
// Função auxiliar para tratar heredoc
int handle_heredoc(t_init_input *input_list, t_types *type)
{
	if (is_heredoc(input_list, type) == -1)
	{   
		perror ("Error setting up heredoc");
		return (1);
	}
	if (check_node(type))
	{
		free_list(input_list);
		free_types(type);
		clear_heredoc_files();
	}
	return (0);
}

/********** PIPES **********/
// Função auxiliar para tratar pipes
int handle_pipeline(t_init_input *input_list, t_envp *env_list, t_types *type)
{
	if (setup_pipeline(input_list, env_list) == -1)
	{
		perror("Error setting up pipeline");
		free_list(input_list);
		free_types(type);
		return (-1);
	}
	free_list(input_list);
	free_types(type);
	return (0);
}

/********** REDIRECTS **********/
// Função auxiliar para tratar redirecionamentos
int handle_redirection(t_init_input *input_list, t_types *type)
{
	if (setup_redirection(input_list, type) == -1)
	{
		perror("Error setting up redirection");
		return (-1);
	}
	if (check_node(type))
	{
		free_list(input_list);
		free_types(type);
		return (0);
	}
	else
		remove_node(&type);
	return (0);
}

/********** BUILT-INS **********/
// Função para gerenciar redirecionamento de descritores de arquivo
static void handle_fd_redirection(t_types *types, int *saved_stdout)
{
	*saved_stdout = dup(STDOUT_FILENO);
	if (types->fd[1] != STDOUT_FILENO)
	{
		if (dup2(types->fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 error encountered in builtin");
			close(*saved_stdout);
			return;
		}
		close(types->fd[1]);
	}
}

// Função para executar comandos internos (dividida)
static void execute_individual_builtin(t_envp *env_list, t_init_input *list, t_types *types)
{
	t_envp *tmp;

	tmp = env_list;
	if (ft_strcmp(types->cmd, "print") == 0)
		print_stack(list);
	else if (ft_strcmp(types->cmd, "env") == 0 || ft_strcmp(types->cmd, "envp") == 0)
		print_envp_list(tmp);
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
	else
	{
		printf("minishell: %s: command not found\n", types->cmd);
		last_status(127);
	}
}

// Função principal para executar comandos internos
void execute_builtin(t_envp *env_list, t_init_input *list, t_types *types)
{
	int saved_stdout;
	
	handle_fd_redirection(types, &saved_stdout);
	execute_individual_builtin(env_list, list, types);
	if (saved_stdout != STDOUT_FILENO)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

// Função auxiliar para execução de comandos
void execute_command(t_types *type, t_envp *env_list, t_init_input *input_list, char **env)
{
	if (!type)
        return ;
	find_command_path(type, env_list);
	exec_cmd(input_list, type, env);
	clear_heredoc_files();
}

int to_exec(t_init_input *input_list, t_types *type, t_envp *env_list)
{
	char **env;

	env = env_to_char(env_list);
	if (is_hdoc(type) && handle_heredoc(input_list, type) == -1)
		return (-1);
	if (is_rdrct(type) && handle_redirection(input_list, type) == -1)
		return (-1);
	if (is_pp(type))
		return (handle_pipeline(input_list, env_list, type));
	if (is_btin(type))
		execute_builtin(env_list, input_list, type);
	if (is_exec(type))
		execute_command(type, env_list, input_list, env);
    //função para verificar fds abertos e fechar, nó por nó. 
    free_list(input_list);
    free_types(type);
	env = NULL;
    return (0);
}
