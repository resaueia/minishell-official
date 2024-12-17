/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:39:20 by rsaueia           #+#    #+#             */
/*   Updated: 2024/12/17 18:49:35 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_types	*init_types(void)
{
	t_types	*types;

	types = (t_types *)malloc(sizeof(t_types));
	if (!types)
		return (NULL);
	types->cmd = NULL;
	types->type = 0;
	types->prev = NULL;
	types->next = NULL;
	return (types);
}

static void split_and_insert(t_types **types, char **cmds)
{
    char **args;
    int i;
    int j;

    i = -1;
    while (cmds[++i])
    {
        args = args_split(cmds[i]);
        j = -1;
        while (args[++j])
            insert_types(types, args[j]);
        args = free_from_split(args);
    }
}

static int process_pipe(t_init_input *input_list, t_types *types, t_envp *env_list)
{
    char *prompt;
    char **cmds;
    int last_exit_status;

    prompt = ft_strdup(input_list->string);
    cmds = lexer(prompt);
    split_and_insert(&types, cmds);
    cmds = free_from_split(cmds);

    last_exit_status = to_exec_pipe(input_list, types, env_list);
    free(prompt);

    return (last_exit_status);
}
/* Function: process_pipe
 * Splits the input string into commands and arguments, inserts them
 * into the types list, and executes the pipeline. Returns the last exit
 * status of the pipeline execution.
 */

static t_init_input *handle_parent_process(t_init_input *current)
{
    if (current->fd_in != STDIN_FILENO)
        close(current->fd_in);
    if (current->fd_out != STDOUT_FILENO)
        close(current->fd_out);

    current = current->next;
    if (current && current->token == 11)
        current = current->next; // Pula o nó do pipe

    return (current);
}

/* Function: handle_parent_process
 * Handles pipe management in the parent process and advances the command list.
 */

static void handle_child_process(t_init_input *current, t_init_input *input_list,
                                 t_types *types, t_envp *env_list)
{
    int last_exit_status;

    if (current == input_list) // Primeiro comando
    {
        dup2(current->fd_out, STDOUT_FILENO);
        close(current->fd_out);
    }
    else if (current->next) // Comando intermediário
    {
        dup2(current->fd_in, STDIN_FILENO);
        dup2(current->fd_out, STDOUT_FILENO);
        close(current->fd_in);
        close(current->fd_out);
    }
    else // Último comando
    {
        dup2(current->fd_in, STDIN_FILENO);
        close(current->fd_in);
    }

    last_exit_status = process_pipe(current, types, env_list);
    exit(last_exit_status);
}

/* Function: handle_child_process
 * Handles file descriptor duplication and calls process_pipe
 * for command execution in the child process. Exits with the last exit status.
 */


int setup_pipeline(t_init_input *input_list, t_envp *env_list)
{
    int           pipe_fd[2];
    pid_t         pid;
    t_init_input  *current;
    t_types       *types;

    types = init_types();
    current = input_list;
    while (current)
    {
        if (current->next && current->next->token == 11)
        {
            if (pipe(pipe_fd) == -1)
                return (perror("Error creating pipe"), -1);
            current->fd_out = pipe_fd[1];
            current->next->next->fd_in = pipe_fd[0];
        }
        pid = fork();
        if (pid == -1)
            return (perror("Error during fork"), -1);
        if (pid == 0)
            handle_child_process(current, input_list, types, env_list);
        current = handle_parent_process(current);
    }
    while (wait(NULL) > 0);
    return (0);
}

/* Function: setup_pipeline
 * Sets up and executes a pipeline of commands. Handles pipe creation,
 * forking processes, and managing file descriptors.
 */




/*static void    process_pipe(t_init_input *input_list, t_types *types, t_envp *env_list)
{
    char    *prompt;
    char    **args;
    char    **cmds;
    int     i;
    int     j;
    int     last_exit_status;
     
    (void)last_exit_status;
    prompt = ft_strdup(input_list->string);
    cmds = lexer(prompt);
    i = -1;
    while(cmds[++i])
    {
        j = -1;
        args = args_split(cmds[i]);
        while (args[++j])
        {
            insert_types(&types, args[j]);
        }
        args = free_from_split(args);
    }
    cmds = free_from_split(cmds);
    last_exit_status = to_exec_pipe(input_list, types, env_list);
}

int setup_pipeline(t_init_input *input_list, t_envp *env_list) 
{
    int           pipe_fd[2];
    pid_t         pid;
    t_init_input    *current;
    t_types         *types;

    types = init_types();
    current = input_list;
    while (current) 
    {
        if (current->next && current->next->token == 11) 
        {
            if (pipe(pipe_fd) == -1) 
            {
                perror("Error creating pipe");
                return (-1);
            }
            current->fd_out = pipe_fd[1];
            current->next->next->fd_in = pipe_fd[0];
        }

        pid = fork();
        if (pid == -1) 
        {
            perror("Error during fork");
            return (-1);
        }
        if (pid == 0) // Processo filho
        {
            if (current == input_list)
            {
                dup2(current->fd_out, STDOUT_FILENO);
                close(current->fd_out);
            }
            else if (current != input_list && current->next) //condição de verificação dos nós intermediários
            {
                dup2(current->fd_in, STDIN_FILENO);
                dup2(current->fd_out, STDOUT_FILENO);
                close(current->fd_in);
                close(current->fd_out);
            }
            else if (!current->next)
            {
                dup2(current->fd_in, STDIN_FILENO);
                close(current->fd_in);
            }
            process_pipe(current, types, env_list);
        }
        else 
        {
            if (current->fd_in != STDIN_FILENO) // Processo pai: gerenciar os pipes
                close(current->fd_in);
            if (current->fd_out != STDOUT_FILENO)
                close(current->fd_out);
            current = current->next; // Avança para o próximo comando na lista
            if (current && current->token == 11) 
                current = current->next; // Pula o nó do pipe
        }
    }
    while (wait(NULL) > 0);
    return (0);
}*/
