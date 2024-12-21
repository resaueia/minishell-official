/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:38:45 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/20 17:58:56 by rsaueia          ###   ########.fr       */
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
