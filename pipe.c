/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:39:20 by rsaueia           #+#    #+#             */
/*   Updated: 2024/11/26 18:35:52 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*void    execute_pipeline(t_init_input **cmds, char **envp)
{
    int     pipe_fd[2];
    int     input_fd;
    pid_t   pid;
    int     i;
    
    input_fd = STDIN_FILENO;
    i = 0;
    while (cmds[i])
    {
        if (cmds[i + 1] != NULL)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("Pipe has failed!");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            pipe_fd[0] = STDIN_FILENO;
            pipe_fd[1] = STDOUT_FILENO;
        }
        pid = fork();
        if (pid == -1)
        {
            perror("Fork has failed!");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            if (input_fd != STDIN_FILENO)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            if (cmds[i + 1] != NULL)
            {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
            exec_command(cmds[i], envp);
            perror("Execution has failed.");
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(NULL);
            close(pipe_fd[1]);
            input_fd = pipe_fd[0];
        }
        i++;
    }
}*/
/*void    process_pipe(t_init_input *input_list, t_envp *env_list)
{
    printf("\n----\non process_pipe\n\n");
    printf("input_list: [%p]\n", input_list);
    
    //transformar input_list em char** para alimentar a t_types do pipeline
    //com a transformação incluir a estrutura do process_input até envio para to_exec.
}*/

int setup_pipeline(t_init_input *input_list, t_envp *env_list) 
{
    printf("\n----\non setup_pipeline\n\n");
    printf("input_list: [%p]\n", input_list);
    printf("env_list: [%p]\n", env_list);
    //int           pipe_fd[2];
    //int           prev_fd;
    //pid_t         pid;
    t_init_input    *current;

    //prev_fd = STDIN_FILENO; // Inicialmente, leitura vem do stdin
    current = input_list;

    print_the_stack(input_list);

    /*while (current) 
    {
        // Cria um pipe, exceto para o último comando
        if (current->next && current->next->token == 11) 
        {
            if (pipe(pipe_fd) == -1) {
                perror("Error creating pipe");
                return (-1);
            }
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
            if (current->next && current->next->type == PIPE) // Configurar a saída do comando
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
            process_pipe(current, env_list);
            exit(EXIT_SUCCESS); // Encerrar o processo filho
        }
        if (prev_fd != STDIN_FILENO) // Processo pai: gerenciar os pipes
            close(prev_fd);
        if (current->next && current->next->type == 11) 
        {
            close(pipe_fd[1]); // Fechar o lado de escrita do pipe
            prev_fd = pipe_fd[0]; // O próximo comando lê do lado de leitura do pipe
        }
        current = current->next; // Avança para o próximo comando na lista
        if (current && current->type == 11) 
            current = current->next; // Pula o nó do pipe
    }
    while (wait(NULL) > 0); // Espera todos os processos filhos terminarem*/
    printf("setup_pipeline has been executed\n");
    return (0);
}
