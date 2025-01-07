/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:38:45 by jparnahy          #+#    #+#             */
/*   Updated: 2025/01/07 19:13:05 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Function: process_pipe
 * Splits the input string into commands and arguments, inserts them
 * into the types list, and executes the pipeline. Returns the last exit
 * status of the pipeline execution.
 */

int	process_pipe(t_init_input *input_list, t_types *types,
		t_envp *env_list)
{
	char	*prompt;
	char	**cmds;
	int		last_exit_status;

	prompt = ft_strdup(input_list->string);
	cmds = lexer(prompt);
	split_and_insert(&types, cmds);
	args_of_cmds(types);
	cmds = free_from_split(cmds);
	remove_quotes_from_types(types);
	last_exit_status = to_exec(input_list, types, env_list);
	free_env(env_list);
	free(prompt);
	return (last_exit_status);
}
