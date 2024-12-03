/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:50:29 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/03 19:22:31 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// Função para executar um comando com execve (dividida)
static void execve_child_process(t_types *type, char **args, char **env)
{
	if (type->fd[0] != STDIN_FILENO)
	{
		if (dup2(type->fd[0], STDIN_FILENO) == -1)
		{
			perror("dup2 fd_in failed");
			exit(EXIT_FAILURE);
		}
		close(type->fd[0]);
	}
	if (type->fd[1] != STDOUT_FILENO)
	{
		if (dup2(type->fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 fd_out failed");
			exit(EXIT_FAILURE);
		}
		close(type->fd[1]);
	}
	if (execve(type->cmd, args, env) == -1)
	{
		perror("Execution failed");
		exit(EXIT_FAILURE);
	}
}

// Função para executar um comando com execve
void exec_cmd(t_init_input *cmd, t_types *type, char **env)
{
	char **args;
	pid_t pid;
	int status;

	(void)cmd;
    args = types_to_char(type);
	pid = fork();
	if (pid == -1)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		execve_child_process(type, args, env);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else
			g_exit_status = 1;
	}
}

// Função para encontrar o caminho do comando (dividida)
static char *allocate_full_path(const char *dir, const char *cmd)
{
	char *full_path;

	full_path = malloc(strlen(dir) + strlen(cmd) + 2);
	if (!full_path)
	{
		perror("Error allocating memory for full_path");
		return NULL;
	}
	sprintf(full_path, "%s/%s", dir, cmd);
	return full_path;
}


// Função para executar comandos internos (dividida)
static void execute_individual_builtin(char *cmd, t_envp *env_list, t_init_input *list, t_types *types)
{
	if (ft_strcmp(cmd, "print") == 0)
		print_stack(list);
	else if (ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "envp") == 0)
		print_envp_list(env_list);
	else if (ft_strcmp(cmd, "pwd") == 0)
		ft_pwd(STDOUT_FILENO);
	else if (ft_strncmp(cmd, "echo", 4) == 0)
		ft_echo(types->next->cmd, &env_list, STDOUT_FILENO);
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		ft_cd(types->next->cmd, &env_list);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		ft_export(types->next->cmd, &env_list);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		ft_unset(types->next->cmd, &env_list);
	else
		printf("minishell: %s: command not found\n", cmd);
}

// Função principal para executar comandos internos
void execute_builtin(char *cmd, t_envp *env_list, t_init_input *list, t_types *types)
{
	int saved_stdout;

    (void)cmd;
	handle_fd_redirection(types, &saved_stdout);
	execute_individual_builtin(types->cmd, env_list, list, types);
	if (saved_stdout != STDOUT_FILENO)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}
#include "minishell.h"

// Função para verificar se um caminho é executável
static int check_executable_path(char *full_path, t_types *type)
{
	if (access(full_path, X_OK) == 0)
	{
		free(type->cmd);
		type->cmd = full_path;
		return (1);
	}
	free(full_path);
	return (0);
}

// Função auxiliar para percorrer diretórios no PATH
static int search_in_path(char *path_dup, t_types *type)
{
	char *dir;
	char *full_path;

	dir = strtok(path_dup, ":");
	while (dir != NULL)
	{
		full_path = allocate_full_path(dir, type->cmd);
		if (!full_path)
			return (0);
		if (check_executable_path(full_path, type))
			return (1);
		dir = strtok(NULL, ":");
	}
	return (0);
}

// Função para encontrar o caminho de um comando
void find_command_path(t_types *type, t_envp *env_list)
{
	char *path;
	char *path_dup;

	(void)env_list;
	path = getenv("PATH");
	if (!path)
	{
		fprintf(stderr, "minishell: PATH not found\n");
		return;
	}
	path_dup = strdup(path);
	if (!path_dup)
	{
		perror("Error duplicating PATH");
		exit(EXIT_FAILURE);
	}
	if (!search_in_path(path_dup, type))
		fprintf(stderr, "minishell: command not found: %s\n", type->cmd);
	free(path_dup);
}

#include "minishell.h"

// Função auxiliar para tratar heredoc
static int handle_heredoc(t_init_input *input_list, t_types *type)
{
	if (is_heredoc(input_list, type) == -1)
	{
		perror("Error setting up heredoc");
		return (-1);
	}
	return (0);
}

// Função auxiliar para tratar pipes
static int handle_pipeline(t_init_input *input_list, t_envp *env_list, t_types *type)
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

// Função auxiliar para tratar redirecionamentos
static int handle_redirection(t_init_input *input_list, t_types *type)
{
	if (setup_redirection(input_list, type) == -1)
	{
		perror("Error setting up redirection");
		return (-1);
	}
	remove_node(&type);
	return (0);
}

// Função auxiliar para execução de comandos
static void execute_command(t_types *type, t_envp *env_list, t_init_input *input_list, char **env)
{
	if (is_btin(type))
		execute_builtin(type->cmd, env_list, input_list, type);
	else
	{
		find_command_path(type, env_list);
		exec_cmd(input_list, type, env);
		clear_heredoc_files();
	}
}

// Função principal para execução de comandos
int to_exec(t_init_input *input_list, t_types *type, t_envp *env_list)
{
	char **env;

	env = env_to_char(env_list);
	if (is_hdoc(type) && handle_heredoc(input_list, type) == -1)
		return (-1);
	if (is_pp(type))
		return (handle_pipeline(input_list, env_list, type));
	if (is_rdrct(type) && handle_redirection(input_list, type) == -1)
		return (-1);
	execute_command(type, env_list, input_list, env);
	free_list(input_list);
	free_types(type);
	return (0);
}


/*
void	execute_builtin(char *cmd, t_envp *env_list, t_init_input *list, t_types *types)
{
    (void)cmd;
	t_envp	*tmp;
    int     saved_stdout;

	tmp = env_list;
	saved_stdout = dup(STDOUT_FILENO);
    if (types->fd[1] != STDOUT_FILENO)
    {
		if (dup2(types->fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 error encountered in builtin");
			close(saved_stdout);
			return ;
		}
        close(types->fd[1]);
    }
	if (ft_strcmp(types->cmd, "print") == 0)
		print_stack(list);
	else if (ft_strcmp(types->cmd, "env") == 0 || ft_strcmp(cmd, "envp") == 0)
		print_envp_list(tmp);
	else if (ft_strcmp(types->cmd, "pwd") == 0)
		ft_pwd(STDOUT_FILENO);
	else if (ft_strncmp(types->cmd, "echo", 4) == 0)
		ft_echo(types->next->cmd, &tmp, STDOUT_FILENO);
	else if (ft_strncmp(types->cmd, "cd", 2) == 0)
		ft_cd(types->next->cmd, &tmp);
	else if (ft_strncmp(types->cmd, "export", 6) == 0)
		ft_export(types->next->cmd, &tmp);
	else if (ft_strncmp(types->cmd, "unset", 5) == 0)
		ft_unset(types->next->cmd, &tmp);
    dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
}

void find_command_path(t_types *type, t_envp *env_list) 
{
    (void)env_list;
    char    *path = getenv("PATH"); // Obtém o PATH do sistema
    char    *path_dup;
    char    *dir;
    char    *full_path;

    if (!path)
    {
        fprintf(stderr, "minishell: PATH not found\n");
        return ;
    }
    path_dup = strdup(path);
    if (!path_dup)
    {
        perror("Erro ao duplicar PATH");
        exit(EXIT_FAILURE);
    }
    dir = strtok(path_dup, ":");
    while (dir != NULL) 
    {
        full_path = malloc(strlen(dir) + strlen(type->cmd) + 2);
        if (!full_path) 
        {
            perror("Erro ao alocar memória para full_path");
            free(path_dup);
            exit(EXIT_FAILURE);
        }
        sprintf(full_path, "%s/%s", dir, type->cmd);
        if (access(full_path, X_OK) == 0)
        { 
            free(type->cmd);   
            type->cmd = full_path;
            free(path_dup);
            return;
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }
    free(path_dup);
}


void	exec_cmd(t_init_input *cmd, t_types *type, char **env)
{
    char    **args;
    pid_t	pid;
    int		status;

    args = types_to_char(type);
    (void)args;
    (void)cmd;
    
    pid = fork();
    if (pid == -1)
    {
        perror("Fork in exec function has failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        if (type->fd[0] != STDIN_FILENO)
        {
            if (dup2(type->fd[0], STDIN_FILENO) == -1)
            {
                perror("dup2 fd_in has failed in exec function");
                exit(EXIT_FAILURE);
            }
            close(type->fd[0]);
        }
        if (type->fd[1] != STDOUT_FILENO)
        {
            if (dup2(type->fd[1], STDOUT_FILENO) == -1)
            {
                perror("dup2 fd_out has failed in exec function");
                exit(EXIT_FAILURE);
            }
            close(type->fd[1]);
        }
        if (execve(type->cmd, args, env) == -1)
        {
            perror("Execution has failed");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            g_exit_status = WEXITSTATUS(status);
        else
            g_exit_status = 1;
    }
}

int    to_exec(t_init_input *input_list, t_types *type, t_envp *env_list)
{
    (void) input_list;
    char  **env;
    t_types     *tmp;
    
    env = env_to_char(env_list);
    tmp = type;
    (void) env;
    (void) type;
    (void) tmp;
    if (is_hdoc(type))
    {
        if (is_heredoc(input_list, type) == -1)
        {   
            perror ("Error setting up heredoc");
            exit(EXIT_FAILURE);
        }
    }
    if (is_pp(type))
    {
        if (setup_pipeline(input_list, env_list) == -1) 
        {
            perror("Error while setting up pipeline");
            return (-1);
        }
        free_list(input_list);
        free_types(type);
        return (0);
    }
    if (is_rdrct(type))
    {
        if (setup_redirection(input_list, type) == -1)
        {
            perror("Error whule setting up redirection\n");
            exit(EXIT_FAILURE);
        }
        remove_node(&type);
    }
    if (is_btin(type))
        execute_builtin(type->cmd, env_list, input_list, type);
    else
    {
        find_command_path(type, env_list); 
        exec_cmd(input_list, type, env);
        clear_heredoc_files();
    }
    //função para verificar fds abertos e fechar, nó por nó. 
    free_list(input_list);
    free_types(type);
    return (0);
}*/

