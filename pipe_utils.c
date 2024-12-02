/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:38:45 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/02 19:31:16 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd_pipe(t_init_input *cmd, t_types *type, char **env)
{
    //printf("\n----\non exec_cmd_pipe\n\n");
    //printf("cmd: [%s]\n", type->cmd);
    //printf("cmd next: [%s]\n", type->next->cmd);
    char    **args;
    (void)cmd;

    args = types_to_char(type);
    (void)args;
    
    //printf("exec_cmd >>> file descriptor in: [%d]\n", cmd->fd_in);
    //printf("exec_cmd >>> file descriptor out: [%d]\n", cmd->fd_out);
    //INCLUIR VERIFICAÇÃO DO ARGUMENTO APÓS O EXECUTÁVEL.
    //SE FOR UM REDIR, ENVIAR O PRÓXIMO NÓ COMO ARGUMENTO.
    if (execve(type->cmd, args, env) == -1)
    {
        perror("Execution has failed");
        exit(EXIT_FAILURE);
    }
}

int    to_exec_pipe(t_init_input *input_list, t_types *type, t_envp *env_list)
{
    //printf("\n----\non to_exec_pipe\n\n");
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

    /*printf("\n----\nafter insert values on vars\n");
    printf("env: [%p]\n", env);
    printf("tmp: [%p]\n", tmp);
    if (!tmp)
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
        include_fds(input_list);
        if (is_heredoc(input_list, type) == -1)
        {   
            perror ("Error setting up heredoc");
            //free_list(args_list);
            //free_list(input_list);
            exit(EXIT_FAILURE);
        }
        //printf("\n----\n");
        //printf("heredoc has been executed\n");
        //printf("input_list->fd_in: [%d]\n", input_list->fd_in);
        //printf("input_list->fd_out: [%d]\n", input_list->fd_out);
        //printf("cmd: [%s]\n", type->cmd);
        //printf("cmd next: [%s]\n", type->next->cmd);
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
            perror("Error whule setting up redirection\n");
            //free_list(args_list);
            //free_list(cmd_list);
            exit(EXIT_FAILURE);
        }
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
        //printf("\nhas execve\n");
        //procura o path do comando na env_list
        //printf("cmd: [%s]\n", type->cmd);
        //printf("cmd next: [%s]\n", type->next->cmd);
        //printf("input_list->fd_in: [%d]\n", input_list->fd_in);
        //printf("input_list->fd_out: [%d]\n", input_list->fd_out);
        find_command_path(type, env_list); 
        //printf("cmd_path: [%s]\n", type->cmd);
        //executa execve
        exec_cmd_pipe(input_list, type, env);
        //verificar se tem algum temporário heredoc_*.tmp e deleta
    }
    free_list(input_list);
    free_types(type);
    return (0);
}