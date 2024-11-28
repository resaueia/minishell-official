/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:39:20 by rsaueia           #+#    #+#             */
/*   Updated: 2024/11/28 17:38:13 by jparnahy         ###   ########.fr       */
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

static void    process_pipe(t_init_input *input_list, t_types *types, t_envp *env_list)
{
    //printf("\n----\non process_pipe\n\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("types: [%p]\n", types);
    //printf("env_list: [%p]\n", env_list);

    char    *prompt;
    char    **args;
    char    **cmds;
    int     i;
    int     j;
    int     last_exit_status;

    //printf("input_list: [%s]\n", input_list->string);
    
    prompt = ft_strdup(input_list->string);
    //printf("prompt: [%s]\n", prompt);
    cmds = lexer(prompt); // split the input for delim and quotes

    //print_the_stack(input_list);

    //printf("\n----\nfds\n");
    //printf("input_list->fd_in: [%d]\n", input_list->fd_in);
    //printf("input_list->fd_out: [%d]\n", input_list->fd_out);

    i = -1;
    //int k = 1;
    //printf("\n----\nafter lexer:\n");
    while(cmds[++i])
    {
        //printf("\n---\n[%iº command]\n", k++);
        //printf("cmds[%i]: [%s]\n", i, cmds[i]);
        j = -1;
        args = args_split(cmds[i]); // split the input for space
        //printf("\n----\nafter args_split:\n");
        while (args[++j])
            insert_types(&types, args[j]);
        args = free_from_split(args);
    }
    cmds = free_from_split(cmds);
    
    //printf("\n----\nprint the ptr of list:\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("input_list->types: [%p]\n", input_list->types);
    //printf("types: [%p]\n", types);
    
    //printf("\n----\nprint the types list:\n");
    /*t_types *temp = types;
    while (temp)
    {
        printf("cms: [%p]_[%s]_[%u]\n", temp->cmd, temp->cmd, temp->type);
        temp = temp->next;
    }*/
    //printf("\n----\npre send to exec\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("types: [%p]\n", types);
    //printf("env_list: [%p]\n", env_list);
    //enviar para execução
    last_exit_status = to_exec_pipe(input_list, types, env_list);
}

int setup_pipeline(t_init_input *input_list, t_envp *env_list) 
{
    //printf("\n----\non setup_pipeline\n\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("env_list: [%p]\n", env_list);
    int           pipe_fd[2];
    int           prev_fd;
    pid_t         pid;
    t_init_input    *current;
    t_types         *types;

    types = init_types();
    prev_fd = STDIN_FILENO; // Inicialmente, leitura vem do stdin
    current = input_list;
    printf("\n--\n");
    print_the_stack(input_list);

    printf("\n--\n");

    while (current) 
    {
        //printf("current->str: [%s]\n", current->string);
        //if (current->next)
        //    printf("current->next->str: [%s] | [%i]\n", current->next->string, current->next->token);
        //printf("\n--\ncurrent->str: [%s]\n", current->string);
        // Cria um pipe, exceto para o último comando
        if (current->next && current->next->token == 11) 
        {
            //printf("\n[has node with pipe]\n\n");
            if (pipe(pipe_fd) == -1) 
            {
                perror("Error creating pipe");
                return (-1);
            }
            //printf("current fd[in]: [%d]\n", current->fd_in);
            //printf("current fd[out]: [%d]\n", current->fd_out);
            //printf("to write >> pipe_fd[0]: [%d]\n", pipe_fd[0]);
            //printf("to read  >> pipe_fd[1]: [%d]\n", pipe_fd[1]);
            //printf("\n--\n");
        }
        pid = fork(); // Cria um processo filho para o comando atual
        if (pid == -1) 
        {
            perror("Error during fork");
            return (-1);
        }
        if (pid == 0) // Processo filho
        {
            if (prev_fd != STDIN_FILENO) // Configurar a entrada do comando
            {
                if (dup2(prev_fd, STDIN_FILENO) == -1) 
                {
                    perror("dup2 failed for input redirection");
                    exit(EXIT_FAILURE);
                }
                close(prev_fd);
            }
            if (current->next && current->next->token == 11) // Configurar a saída do comando
            {
                if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) 
                {
                    perror("dup2 failed for output redirection");
                    exit(EXIT_FAILURE);
                }
                close(pipe_fd[1]);
                close(pipe_fd[0]);
            }
            // Criar uma processe_pipe, para processar o current cmd e enviar para to_exec
            //printf("\n--\n");
            //printf("current->str: [%s]\n", current->string);
            //printf("current->fd_in: [%d]\n", currtypes = init_types();
            process_pipe(input_list, types, env_list);
            exit(EXIT_SUCCESS); // Encerrar o processo filho
            //printf("child process ended");
        }
        if (prev_fd != STDIN_FILENO) // Processo pai: gerenciar os pipes
            close(prev_fd);
        if (current->next && current->next->token == 11) 
        {
            close(pipe_fd[1]); // Fechar o lado de escrita do pipe
            prev_fd = pipe_fd[0]; // O próximo comando lê do lado de leitura do pipe
        }
        current = current->next; // Avança para o próximo comando na lista
        if (current && current->token == 11) 
            current = current->next; // Pula o nó do pipe
        
    }
    //printf("\n--\n");
    while (wait(NULL) > 0); // Espera todos os processos filhos terminarem*/
    //printf("setup_pipeline has been executed\n");
    return (0);
}
