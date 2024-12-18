/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:39:20 by rsaueia           #+#    #+#             */
/*   Updated: 2024/12/18 19:40:29 by jparnahy         ###   ########.fr       */
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
    char    *prompt;
    char    **args;
    char    **cmds;
    int     i;
    int     j;
    int     last_exit_status;
    
    (void)last_exit_status;
    prompt = ft_strdup(input_list->string);
    cmds = lexer(prompt); // split the input for delim and quotes

    i = -1;
    while(cmds[++i])
    {
        j = -1;
        args = args_split(cmds[i]); // split the input for space
        while (args[++j])
            insert_types(&types, args[j]);
        args = free_from_split(args);
    }
    cmds = free_from_split(cmds);
    args_of_cmds(types);
    last_exit_status = to_exec_pipe(input_list, types, env_list); //enviar para execução
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
            current->fd_out = pipe_fd[1]; // Armazena o lado de escrita do pipe no nó atual
            current->next->next->fd_in = pipe_fd[0]; // Armazena o lado de leitura do pipe no próximo nó
        }
        pid = fork(); // Cria um processo filho para o comando atual
        if (pid == -1) 
        {
            perror("Error during fork");
            return (-1);
        }
        if (pid == 0) // Processo filho
        {
            if (current == input_list) //condição de verificação do primeiro nó
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
    while (wait(NULL) > 0); // Espera todos os processos filhos terminarem*/
    return (0);
}
