/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:38:45 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/19 00:38:06 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd_pipe(t_init_input *cmd, t_types *type, char **env)
{
    char    **args;
    (void)cmd;

    args = types_to_char(type);
    (void)args;
    if (execve(type->cmd, args, env) == -1)
    {
        perror("Execution has failed");
        exit(EXIT_FAILURE);
    }
}

/* Function: to_exec_pipe
 * Orchestrates the execution of commands in a pipeline:
 * - Handles heredoc setup.
 * - Handles input/output redirections.
 * - Executes built-in or external commands.
 * Frees the input list and type structures before returning.
 */

int to_exec_pipe(t_init_input *input_list, t_types *type, t_envp *env_list)
{
    char    **env;

    env = env_to_char(env_list);
    if (is_hdoc(type)) // Heredoc
        return (handle_heredoc(input_list, type));
    if (is_rdrct(type))
		return (handle_redirection(input_list, type));
	if (is_btin(type))
		execute_builtin(env_list, input_list, type);
	if (is_exec(type)) //execve
		execute_command(type, env_list, input_list, env);
    free_list(input_list);
    free_types(type);
    return (0);
}