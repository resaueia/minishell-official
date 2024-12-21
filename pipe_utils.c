/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:38:45 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/21 19:18:28 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd_pipe(t_init_input *cmd, t_types *type, char **env)
{
	t_types	*temp;
	char	**args;

	printf("\n---\nexec_cmd_pipe\n");
	temp = type;
	printf("temp: [%p]\n", temp);
	while (temp)
	{
		printf("cms: [%s]_[%u]_[%i]_[%i]\n", temp->cmd, temp->type, temp->fd[0],
			temp->fd[1]);
		temp = temp->next;
	}
	(void)cmd;
	args = types_to_char(type);
	(void)args;
	if (execve(type->cmd, args, env) == -1)
	{
		perror("Execution has failed");
		exit(EXIT_FAILURE);
	}
}

static void	split_and_insert(t_types **types, char **cmds)
{
	char	**args;
	int		i;
	int		j;

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
	free(prompt);
	return (last_exit_status);
}
