/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:39:20 by rsaueia           #+#    #+#             */
/*   Updated: 2024/12/05 12:36:29 by jparnahy         ###   ########.fr       */
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
    
    (void)last_exit_status;
    prompt = ft_strdup(input_list->string);
    //printf("prompt: [%s]\n", prompt);
    cmds = lexer(prompt); // split the input for delim and quotes

    //print_the_stack(input_list);

    /*printf("\n----\nprint the cmds list:\n");
    i = -1;
    while (cmds[++i])
        printf("cmds[%i]: [%s]\n", i, cmds[i]);
    */
    /*printf("\n----\nfds\n");
    printf("input_list->fd_in: [%d]\n", input_list->fd_in);
    printf("input_list->fd_out: [%d]\n", input_list->fd_out);*/

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
        {
            //printf("args[%i]: [%s]\n", j, args[j]);
            //printf("types [%p][%s]\n", types, types->cmd);
            insert_types(&types, args[j]);
        }
        args = free_from_split(args);
    }
    cmds = free_from_split(cmds);
    
    //printf("\n----\nprint the ptr of list:\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("input_list->types: [%p]\n", input_list->types); //printf("\n----\nafter declarations\n");
    //input_list->fd_in = 0;current->string: [ wc]

    
    /*printf("\n----\nprint the types list:\n");
    t_types *temp = types;
    printf("%s\n", types->cmd);
    printf("temp: [%p]\n", temp);
    while (temp)
    {
        printf("cms: [%p]_[%s]_[%u]\n", temp->cmd, temp->cmd, temp->type);
        temp = temp->next;
    }*/
    //printf("input_list->fd_in: [%d]\n", input_list->fd_in);
    //printf("input_list->fd_out: [%d]\n", input_list->fd_out);
    //printf("input_list: [%p]\n", input_list);
    //printf("types: [%p]\n", types);
    //printf("env_list: [%p]\n", env_list);
    //enviar para execução
    last_exit_status = to_exec_pipe(input_list, types, env_list);
}

/*int setup_pipeline(t_init_input *input_list, t_envp *env_list) 
{
    printf("\n----\non setup_pipeline\n\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("env_list: [%p]\n", env_list);
    int           pipe_fd[2];
    //int           prev_fd;
    pid_t         pid;
    t_init_input    *current;
    t_types         *types;

    types = init_types();
    //prev_fd = STDIN_FILENO; // Inicialmente, leitura vem do stdin
    current = input_list;
    //printf("\n--\n");

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
            current->fd_out = pipe_fd[1]; // Armazena o lado de escrita do pipe no nó atual
            current->next->next->fd_in = pipe_fd[0]; // Armazena o lado de leitura do pipe no próximo nó
            printf("\n-- POS --\n");
            printf("current fd[in]: [%d]\n", current->fd_in);
            printf("current fd[out]: [%d]\n", current->fd_out);
            printf("next fd[in]: [%d]\n", current->next->next->fd_in);
            print_the_stack(current);
            printf("to write >> pipe_fd[0]: [%d]\n", pipe_fd[0]);
            printf("to read  >> pipe_fd[1]: [%d]\n", pipe_fd[1]);
            //printf("\n--\n");
        }
        //else
        //    current->fd_out = STDOUT_FILENO; // Último comando não precisa de pipe para saída
        pid = fork(); // Cria um processo filho para o comando atual
        if (pid == -1) 
        {
            perror("Error during fork");
            return (-1);
        }
        if (pid == 0) // Processo filho
        {
            printf("\n-- no filho [%i] --\n", getpid());
            printf("current->string: [%s]\n", current->string);
            if (current->fd_in != STDIN_FILENO)
            {
                printf("prev_fd != STDIN_FILENO\n");
                if (dup2(current->fd_in, STDIN_FILENO) == -1) 
                {
                    perror("dup2 failed for input redirection");
                    exit(EXIT_FAILURE);
                }
                printf("prev_fd | current->fd_in: [%d]\n", current->fd_in);
                printf("prev_fd | current->fd_out: [%d]\n", current->fd_out);
                close(current->fd_in);    
            }
            printf("oi\n");
            if (current->fd_out != STDOUT_FILENO) 
            {
                printf("fd out == %i\n", current->fd_out);
                //dup2(current->fd_out, STDOUT_FILENO);
                if (dup2(current->fd_out, STDOUT_FILENO) == -1) 
                {
                    printf("tô errado\n");
                    perror("dup2 failed for output redirection");
                    exit(EXIT_FAILURE);
                }
                printf("cluse\n");
                close(current->fd_out);
            }
            printf("foda-se\n");
            process_pipe(current, types, env_list);
            printf("\n-- fim do filho --\n");
        }
        else 
        {
            if (current->fd_in != STDIN_FILENO) // Processo pai: gerenciar os pipes
                close(current->fd_in); // Fecha o lado de leitura do pipe
            if (current->next && current->next->token == 11) 
            {
                close(pipe_fd[1]); // Fechar o lado de escrita do pipe
                current->fd_in = pipe_fd[0]; // Atualiza prev_fd para a próxima iteração
            }
            current = current->next; // Avança para o próximo comando na lista
            if (current && current->token == 11) 
            current = current->next; // Pula o nó do pipe
        }
    }
    //printf("\n--\n");
    while (wait(NULL) > 0); // Espera todos os processos filhos terminarem*/
    //printf("setup_pipeline has been executed\n");
    //return (0);
//}

int setup_pipeline(t_init_input *input_list, t_envp *env_list) 
{
    //printf("\n----\non setup_pipeline\n\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("env_list: [%p]\n", env_list);
    int           pipe_fd[2];
    pid_t         pid;
    t_init_input    *current;
    t_types         *types;

    types = init_types();
    current = input_list;
    //printf("\n--\n");
    //print_the_stack(input_list);
    //printf("\n--\n");

    while (current) 
    {
        if (current->next && current->next->token == 11) 
        {
            //printf("\n[has node with pipe]\n\n");
            if (pipe(pipe_fd) == -1) 
            {
                perror("Error creating pipe");
                return (-1);
            }
            current->fd_out = pipe_fd[1]; // Armazena o lado de escrita do pipe no nó atual
            current->next->next->fd_in = pipe_fd[0]; // Armazena o lado de leitura do pipe no próximo nó
            /*printf("\n-- POS --\n");
            printf("current fd[in]: [%d]\n", current->fd_in);
            printf("current fd[out]: [%d]\n", current->fd_out);
            printf("next fd[in]: [%d]\n", current->next->next->fd_in);
            print_the_stack(current);
            printf("to write >> pipe_fd[0]: [%d]\n", pipe_fd[0]);
            printf("to read  >> pipe_fd[1]: [%d]\n", pipe_fd[1]);*/
            //printf("\n--\n");
        }
        //printf("pre fork main -> %i\n", pid);
        pid = fork(); // Cria um processo filho para o comando atual
        //printf("pos fork main -> %i\n", pid);
        if (pid == -1) 
        {
            perror("Error during fork");
            return (-1);
        }
        if (pid == 0) // Processo filho
        {
            //printf("\n-- no filho -- [%i]\n", getpid());
            //printf("current->string: [%s]\n", current->string);
            if (current == input_list) //condição de verificação do primeiro nó
            {
                //printf("\ncurrent == input_list\n");
                //printf("current: cmd[%s] | fd_in[%d] | fd_out[%d]\n", current->string, current->fd_in, current->fd_out);
                dup2(current->fd_out, STDOUT_FILENO);
                close(current->fd_out);
                //close(current->fd_in);
            }
            else if (current != input_list && current->next) //condição de verificação dos nós intermediários
            {
                //printf("\ncurrent->next != NULL\n");
                //printf("current: cmd[%s] | fd_in[%d] | fd_out[%d]\n", current->string, current->fd_in, current->fd_out);
                dup2(current->fd_in, STDIN_FILENO);
                dup2(current->fd_out, STDOUT_FILENO);
                close(current->fd_in);
                close(current->fd_out);
            }
            else if (!current->next)
            {
                //printf("current->next == NULL\n");
                //printf("current: cmd[%s] | fd_in[%d] | fd_out[%d]\n", current->string, current->fd_in, current->fd_out);
                //close(current->fd_out);
                dup2(current->fd_in, STDIN_FILENO);
                close(current->fd_in);
            }
            process_pipe(current, types, env_list);
            //printf("\n-- fim do filho --\n");
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
    while (wait(NULL) > 0); // Espera todos os processos filhos terminarem*/
    //printf("setup_pipeline has been executed\n");
    return (0);
}