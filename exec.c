/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:50:29 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/19 01:57:40 by jparnahy         ###   ########.fr       */
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
	if (check_node(type)) //verifica se é o primeiro nó da lista
	{
		free_list(input_list);
		free_types(type);
		clear_heredoc_files(); //verificar se tem algum temporário heredoc_*.tmp e deleta
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
	if (check_node(type)) //verifica se é o primeiro nó da lista
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
		printf("minishell: %s: command not found\n", types->cmd);
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

/********** EXECVE **********/
// Função para executar um comando com execve (dividida)
/*static void execve_child_process(t_types *type, char **args, char **env)
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
		exit(EXIT_FAILURE);
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
		WIFEXITED(status);
	}
}

// Função para construir o path do executável
static void construct_path(char *full_path, char *dir, char *cmd)
{
    char    *to_path;

    to_path = ft_strjoin(dir, "/");
    to_path = ft_strjoin(to_path, cmd);
    ft_strlcpy(full_path, to_path, ft_strlen(to_path) + 1);
}

// Função para encontrar o caminho do comando (dividida)
static char *allocate_full_path(char *dir, char *cmd)
{
	char *full_path;

	full_path = malloc(strlen(dir) + strlen(cmd) + 2);
	if (!full_path)
	{
		perror("Error allocating memory for full_path");
		return NULL;
	}
	construct_path(full_path, dir, cmd);
	return full_path;
}

// Função para verificar se um caminho é executável
static int check_executable_path(char *full_path, t_types *type)
{
	if (access(full_path, X_OK) == 0)
	{
		free(type->cmd);
		type->cmd = full_path;
		return (1);
	}
	//free(full_path);
	return (0);
}

static char *ft_strtok_r(char *str, char *delim, char **save_ptr)
{
    char *start;
    char *end;

    if (str != NULL) // Se `str` não for NULL, comece uma nova string
        *save_ptr = str;
    if (*save_ptr == NULL) // Se o ponteiro salvo for NULL, não há mais tokens
        return NULL;
    start = *save_ptr; // Ignora os delimitadores iniciais
    while (*start && ft_strchr(delim, *start))
        start++;
    if (*start == '\0') // Se o final da string foi alcançado, não há mais tokens
    {	
        *save_ptr = NULL;
        return NULL;
    }
    end = start; // Encontra o próximo delimitador
    while (*end && !ft_strchr(delim, *end)) 
        end++;
    if (*end) // Se um delimitador foi encontrado, termine o token e atualize o ponteiro salvo
    {
        *end = '\0';
        *save_ptr = end + 1;
    } 
    else
        *save_ptr = NULL;
    return start;
}

// Função auxiliar para percorrer diretórios no PATH
static int search_in_path(char *path_dup, t_types *type)
{
	char	*dir;
	char	*full_path;
	char	*save_ptr;

	dir = ft_strtok_r(path_dup, ":", &save_ptr);
	while (dir != NULL)
	{
		full_path = allocate_full_path(dir, type->cmd);
		if (!full_path)
		{
			perror("Error on malloc");
			free(path_dup);
			exit (EXIT_FAILURE);
		}
		if (check_executable_path(full_path, type))
			return (1);
		dir = ft_strtok_r(NULL, ":", &save_ptr); 
	}
	return (0);
}

// Função para encontrar o caminho de um comando
void find_command_path(t_types *type, t_envp *env_list)
{
	char *path;
	char *path_dup;

	(void)env_list;
	path = get_value("PATH", env_list);
	if (!path)
	{
		printf("minishell: %s: %s\n", strerror(errno), type->cmd);
		return ;
	}
	path_dup = ft_strdup(path);
	if (!path_dup)
	{
		perror("Error duplicating PATH");
		exit(EXIT_FAILURE);
	}
	if (!search_in_path(path_dup, type))
		fprintf(stderr, "minishell: command not found: %s\n", type->cmd);
	free(path_dup);
}*/


static char *ft_strtok_r(char *str, char *delim, char **save_ptr)
{
    char *start;
    char *end;

    if (str != NULL) // Se `str` não for NULL, comece uma nova string
        *save_ptr = str;
    if (*save_ptr == NULL) // Se o ponteiro salvo for NULL, não há mais tokens
        return NULL;
    start = *save_ptr; // Ignora os delimitadores iniciais
    while (*start && ft_strchr(delim, *start))
        start++;
    if (*start == '\0') // Se o final da string foi alcançado, não há mais tokens
    {	
        *save_ptr = NULL;
        return NULL;
    }
    end = start; // Encontra o próximo delimitador
    while (*end && !ft_strchr(delim, *end)) 
        end++;
    if (*end) // Se um delimitador foi encontrado, termine o token e atualize o ponteiro salvo
    {
        *end = '\0';
        *save_ptr = end + 1;
    } 
    else
        *save_ptr = NULL;
    return start;
}

static void construct_path(char *full_path, char *dir, char *cmd)
{
    char    *to_path;

    to_path = ft_strjoin(dir, "/");
    to_path = ft_strjoin(to_path, cmd);
    ft_strlcpy(full_path, to_path, ft_strlen(to_path) + 1);
}

void find_command_path(t_types *type, t_envp *env_list) 
{
    char    *path;
    char    *path_dup;
    char    *dir;
    char    *save_ptr;
    char    *full_path;

    path = get_value("PATH", env_list); // get the PATH of minishell
    if (!path)
    {
        printf("minishell: %s: %s\n", strerror(errno), type->cmd);
        return ;
    }
    path_dup = ft_strdup(path); //duplocating the PATH
    if (!path_dup)
    {
        perror("Error on dup");
        exit(EXIT_FAILURE);
    }
    dir = ft_strtok_r(path_dup, ":", &save_ptr); //split the PATH
    while (dir != NULL) 
    {
        full_path = malloc(ft_strlen(dir) + ft_strlen(type->cmd) + 2); // +2 para '/' e '\0'
        if (!full_path) 
        {
            perror("Error on malloc");
            free(path_dup);
            exit(EXIT_FAILURE);
        }
        construct_path(full_path, dir, type->cmd);
        if (access(full_path, X_OK) == 0) // Verifica se o caminho é executável
        { 
            free(type->cmd);        // Libera o conteúdo anterior de `type->cmd`
            type->cmd = full_path;  // Atualiza `type->cmd` com o novo caminho completo
            free(path_dup);         // Libera a cópia do PATH
            return;                 // Sai da função após encontrar o comando
        }
        free(full_path); // Libera full_path se não for válido
        dir = ft_strtok_r(NULL, ":", &save_ptr); //split the PATH
    }
    printf("minishell: %s: %s\n", strerror(errno), type->cmd);
    free(path_dup); // Libera a cópia do PATH
}

void	exec_cmd(t_init_input *cmd, t_types *type, char **env)
{
    (void) cmd;
    char    **args;
    pid_t	pid;
    int		status;

    args = types_to_char(type);
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
                //perror("dup2 fd_out has failed in exec function");
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
        WIFEXITED(status);
    }
}

// Função auxiliar para execução de comandos
void execute_command(t_types *type, t_envp *env_list, t_init_input *input_list, char **env)
{
	if (!type)
        return ;
	find_command_path(type, env_list); //procura o path do comando na env_list
	exec_cmd(input_list, type, env); //executa execve
	clear_heredoc_files(); //verificar se tem algum temporário heredoc_*.tmp e deleta
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
	if (is_btin(type))
		execute_builtin(env_list, input_list, type);
	if (is_exec(type)) //execve
		execute_command(type, env_list, input_list, env);
    //função para verificar fds abertos e fechar, nó por nó. 
    free_list(input_list);
    free_types(type);
	env = NULL;
    return (0);
}
