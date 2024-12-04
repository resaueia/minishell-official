/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:50:29 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/03 23:49:36 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
static int  check_node(t_types *type)
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
		ft_echo(types, &tmp, STDOUT_FILENO);
	else if (ft_strncmp(types->cmd, "cd", 2) == 0)
		ft_cd(types->next->cmd, &tmp);
	else if (ft_strncmp(types->cmd, "export", 6) == 0)
		ft_export(types->next->cmd, &tmp);
	else if (ft_strncmp(types->cmd, "unset", 5) == 0)
		ft_unset(types->next->cmd, &tmp);
    dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
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

static void construct_path(char *full_path, char *dir, char *cmd)
{
    char    *to_path;

    to_path = ft_strjoin(dir, "/");
    to_path = ft_strjoin(to_path, cmd);
    ft_strlcpy(full_path, to_path, ft_strlen(to_path) + 1);
}

void find_command_path(t_types *type, t_envp *env_list) 
{
    //transformar para salvar o statuscode em caso de erro no diretório.
    (void)env_list;
    char    *path;
    char    *path_dup;
    char    *dir;
    char    *save_ptr;
    char    *full_path;

    path = getenv("PATH"); // get the PATH of sistema
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
        //sprintf(full_path, "%s/%s", dir, type->cmd);
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
    // Caso o comando não seja encontrado, imprime uma mensagem de erro
    printf("minishell: %s: %s\n", strerror(errno), type->cmd);
    free(path_dup); // Libera a cópia do PATH
}

void	exec_cmd(t_init_input *cmd, t_types *type, char **env)
{
    //printf("\n----\non exec_cmd\n\n");
    //printf("cmd:       [%s]\n", type->cmd);
    printf("cmd:       [%p]_[%s]_[%u]_[%i]_[%i]\n", type->cmd, type->cmd, type->type, type->fd[0], type->fd[1]);
    //rintf("cmd->next: [%s]\n", type->next->cmd);
    //printf("cmd->next: [%p]_[%s]_[%u]_[%i]_[%i]\n", type->next->cmd, type->next->cmd, type->next->type, type->next->fd[0], type->next->fd[1]);
    //printf("\n----\n");
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
        /*if (WIFEXITED(status))
            printf("Child has exited with status: %d\n", WEXITSTATUS(status));*/
    }
}

int    to_exec(t_init_input *input_list, t_types *type, t_envp *env_list)
{
    //printf("\n----\non to_exec\n\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("env_list: [%p]\n", env_list);
    //printf("type: [%p]\n", type);
    //printf("input_list->types: [%p]\n", input_list->types);
    (void) input_list;
    char  **env;
    t_types     *tmp;
    
    //printf("\n----\nafter declarations\n");
    //input_list->fd_in = 0;
    //input_list->fd_out = 1;
    //printf("input_list->fd_in: [%d]\n", input_list->fd_in);
    //printf("input_list->fd_out: [%d]\n", input_list->fd_out);
    env = env_to_char(env_list);
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
    if (is_hdoc(type)) //heredoc
    {
        //executa heredoc
        //printf("has heredoc\n");
        if (is_heredoc(input_list, type) == -1)
        {   
            perror ("Error setting up heredoc");
            //free_list(args_list);
            //free_list(input_list);
            //exit(EXIT_FAILURE);
            return (1);
        }
        //printf("\n----\n");
        //printf("heredoc has been executed\n");
        //printf("input_list->fd_in: [%d]\n", input_list->fd_in);
        //printf("input_list->fd_out: [%d]\n", input_list->fd_out);
        //printf("cmd: [%s]\n", type->cmd);
        //printf("cmd next: [%s]\n", type->next->cmd);
        if (check_node(type))
        {
            free_list(input_list);
            free_types(type);
            clear_heredoc_files();
            return (0);
        }
    }
    if (is_pp(type)) //pipe
    {
        //executa em cenário de pipe
        //printf("has pipe\n");
        //print_the_stack(input_list);
        if (setup_pipeline(input_list, env_list) == -1) 
        {
            perror("Error while setting up pipeline");
            return (-1);
        }
        free_list(input_list);
        free_types(type);
        return (0);
    }
    if (is_rdrct(type)) //redirects
    {
        //executa redirect
        //printf("has redirect\n");
        if (setup_redirection(input_list, type) == -1)
        {
            //perror("Error whule setting up redirection\n");
            //free_list(args_list);
            //free_list(cmd_list);
            //exit(EXIT_FAILURE);
            return (1);
        }
        if (check_node(type))
        {
            free_list(input_list);
            free_types(type);
            return (0);
        }
        else
            remove_node(&type);
        
    }
    if (is_btin(type)) //builtin
    {
        //printf("has builtin\n");
        //printf("cmd: [%s]\n", type->cmd);
        //printf("cmd next: [%s]\n", type->next->cmd);
        //printf("input_list->fd_in: [%d]\n", input_list->fd_in);
        //printf("input_list->fd_out: [%d]\n", input_list->fd_out);
        execute_builtin(type->cmd, env_list, input_list, type); //executa o comando
    }
    else //if (is_exec(type)) //execve
    {
        printf("has execve\n");
        //procura o path do comando na env_list
        if (!type)
            return (0);
        find_command_path(type, env_list); 
        //printf("cmd_path: [%s]\n", type->cmd);
        //executa execve
        exec_cmd(input_list, type, env);
        clear_heredoc_files();
        //verificar se tem algum temporário heredoc_*.tmp e deleta
    }
    //função para verificar fds abertos e fechar, nó por nó. 
    free_list(input_list);
    free_types(type);
    return (0);
}