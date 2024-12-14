/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:38:45 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/14 18:05:22 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd_pipe(t_init_input *cmd, t_types *type, char **env)
{
    char    **args;
    (void)cmd;

    args = types_to_char(type);
    if (execve(type->cmd, args, env) == -1)
    {
        perror("Execution has failed");
        exit(EXIT_FAILURE);
    }
}

int    to_exec_pipe(t_init_input *input_list, t_types *type, t_envp *env_list)
{
    (void) input_list;
    char  **env;
    t_types     *tmp;
    
    env = env_to_char(env_list);
    tmp = type;
    (void) env;
    (void) type;
    (void) tmp;

    if (is_hdoc(type)) //heredoc
    {
        include_fds(input_list);
        if (is_heredoc(input_list, type) == -1)
        {   
            perror ("Error setting up heredoc");
            exit(EXIT_FAILURE);
        }
        free_list(input_list);
        free_types(type);
        return (0);
    }
    if (is_rdrct(type)) //redirects
    {
        if (setup_redirection(input_list, type) == -1)
        {
            perror("Error whule setting up redirection\n");
            exit(EXIT_FAILURE);
        }
        remove_node(&type);
    }
    if (is_btin(type)) //builtin
        execute_builtin(type->cmd, env_list, input_list, type); //executa o comando
    else //if (is_exec(type)) //execve
    {
        find_command_path(type, env_list); //procura o path do comando na env_list
        exec_cmd_pipe(input_list, type, env); //executa execve
    }
    free_list(input_list);
    free_types(type);
    return (0);
}