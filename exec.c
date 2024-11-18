/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:50:29 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/18 16:21:19 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_builtin(char *cmd, t_envp *env_list, t_init_input *list, t_types *types)
{
    (void)cmd;
	t_envp	*tmp;
    int     saved_stdout;
    //int   saved_stdin;

    printf("exec_bi >>> file descriptor in: [%d]\n", list->fd_in);
    printf("exec_bi >>> file descriptor out: [%d]\n", list->fd_out);

	tmp = env_list;
	saved_stdout = dup(STDOUT_FILENO);
	//saved_stdin = dup(STDIN_FILENO);
    if (list->fd_out != STDOUT_FILENO)
    {
		if (dup2(list->fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2 error encountered in builtin");
			close(saved_stdout);
			return ;
		}
        close(list->fd_out);
    }
    /*if (list->fd_in != STDIN_FILENO)
    {
        dup2(list->fd_in, STDIN_FILENO);
        close(list->fd_in);
    }*/
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
	//dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	//close(saved_stdin);
}

static void find_command_path(t_types *type, t_envp *env_list) 
{
    (void)env_list;
    char *path = getenv("PATH"); // Obtém o PATH do sistema
    char *path_dup;
    char *dir;
    char *full_path;

    if (!path)
    {
        fprintf(stderr, "minishell: PATH not found\n");
        return ;
    }

    path_dup = strdup(path); //faz uma cópia do path para manipulação
    if (!path_dup)
    {
        perror("Erro ao duplicar PATH");
        exit(EXIT_FAILURE);
    }

    dir = strtok(path_dup, ":");
    while (dir != NULL) 
    {
        full_path = malloc(strlen(dir) + strlen(type->cmd) + 2); // +2 para '/' e '\0'
        if (!full_path) 
        {
            perror("Erro ao alocar memória para full_path");
            free(path_dup);
            exit(EXIT_FAILURE);
        }
        sprintf(full_path, "%s/%s", dir, type->cmd);
        if (access(full_path, X_OK) == 0) // Verifica se o caminho é executável
        { 
            free(type->cmd);        // Libera o conteúdo anterior de `type->cmd`
            type->cmd = full_path;  // Atualiza `type->cmd` com o novo caminho completo
            free(path_dup);         // Libera a cópia do PATH
            return;                 // Sai da função após encontrar o comando
        }
        free(full_path); // Libera full_path se não for válido
        dir = strtok(NULL, ":");
    }
    // Caso o comando não seja encontrado, imprime uma mensagem de erro
    fprintf(stderr, "minishell: comando não encontrado: %s\n", type->cmd);
    free(path_dup); // Libera a cópia do PATH
}

void    exec_command(t_init_input *cmd, char **envp)
{
    pid_t   pid;
    int     status;

    pid = fork();
    if (pid == -1)
    {
        perror("Fork in exec function has failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        if (cmd->fd_in != STDIN_FILENO)
        {
            if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
            {
                perror("dup2 fd_in has failed in exec function");
                exit(EXIT_FAILURE);
            }
            close(cmd->fd_in);
        }
        if (cmd->fd_out != STDOUT_FILENO)
        {
            if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
            {
                perror("dup2 fd_out has failed in exec function");
                exit(EXIT_FAILURE);
            }
            close(cmd->fd_out);
        }
        if (execve(cmd->args[0], cmd->args, envp) == -1)
        {
            perror("Execution has failed");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            printf("Child has exited with status: %d\n", WEXITSTATUS(status));
    }
}

void	exec_cmd(t_init_input *cmd, t_types *type, char **env)
{
    char    **args;
    pid_t	pid;
    int		status;

    args = types_to_char(type);
    (void)args;
    cmd->fd_in = 0;
    cmd->fd_out = 1;
    printf("exec_cmd >>> file descriptor in: [%d]\n", cmd->fd_in);
    printf("exec_cmd >>> file descriptor out: [%d]\n", cmd->fd_out);
    pid = fork();
    if (pid == -1)
    {
        perror("Fork in exec function has failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        if (cmd->fd_in != STDIN_FILENO)
        {
            if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
            {
                perror("dup2 fd_in has failed in exec function");
                exit(EXIT_FAILURE);
            }
            close(cmd->fd_in);
        }
        if (cmd->fd_out != STDOUT_FILENO)
        {
            if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
            {
                perror("dup2 fd_out has failed in exec function");
                exit(EXIT_FAILURE);
            }
            close(cmd->fd_out);
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
            printf("Child has exited with status: %d\n", WEXITSTATUS(status));
    }
}

int    to_exec(char **cmds, t_init_input *input_list, t_types *type, t_envp *env_list)
{
    //printf("\n----\nto_exec\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("env_list: [%p]\n", env_list);
    //printf("type: [%p]\n", type);
    //printf("input_list->types: [%p]\n", input_list->types);
    (void) input_list;
    t_init_input    *args_list;
    t_init_input    *args_tail;
    t_init_input    *pipe_cmds[256];
    t_types         *tmp;
    char            **env;
    //char            **args;
    
    args_list = NULL;
    args_tail = NULL;
    env = env_to_char(env_list); //write a env_list_to_char
    //args = cmds;
    split_commands(cmds, &args_list, &args_tail); 
    //print_the_stack(args_list);    
    //printf("\n----\nafter declarations\n");
    input_list->fd_in = 0;
    input_list->fd_out = 1;
    tmp = type;
    (void) env;
    (void) type;
    (void) tmp;

    //printf("\n----\nafter insert values on vars\n");
    //printf("env: [%p]\n", env);
    //printf("tmp: [%p]\n", tmp);
    /*if (!tmp)
        printf("type is NULL\n");
    else
        printf("type is not NULL\n");
    while (tmp)
    {
        printf("cms: [%s] - types: [%u]\n", tmp->cmd, tmp->type);
        tmp = tmp->next;
    }*/
    if (is_heredoc(args_list) == -1)
    {
        //executa heredoc
        //printf("has heredoc\n");
        //tackle_heredoc(cmd_list);
        perror ("Error setting up heredoc");
        free_list(args_list);
        //free_list(input_list);
        return (1);
    }
    if (has_pipe(args_list))
    {
        //executa em cenário de pipe
        //printf("has pipe\n");
        //args_list = split_commands(cmds, &head, &tail);
        //printf("\n----\nprint the args_list:\n");
        //print_the_stack(args_list);
        split_by_pipes(args_list, pipe_cmds);
        execute_pipeline(pipe_cmds, env);
    }
    if (setup_redirection(args_list) == -1)
    {
        //executa redirect
        printf("has redirect\n");
        perror("Error whule setting up redirection\n");
        free_list(args_list);
        //free_list(input_list);
        return (1);
    }
    else if (is_btin(type)) //builtin
    {
        execute_builtin(type->cmd, env_list, input_list, type);
        //executa o comando
    }
    else //if (is_exec(type)) //execve
    {
        printf("has execve\n");
        //procura o path do comando na env_list
        find_command_path(type, env_list);
        printf("cmd_path: [%s]\n", type->cmd);
        //executa execve
        exec_cmd(input_list, type, env);
    }
    //free(env);
    return (0);
}