/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:37:09 by rsaueia           #+#    #+#             */
/*   Updated: 2024/12/22 16:08:43 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_in(t_types *type, t_types *type_head)
{
	int	temp_fd;

	if (access(type->next->cmd, R_OK) == -1)
	{
		ft_putstr_fd("minishell: no such file or directory: ", 2);
		last_status(1);
		return (-1);
	}
	if (type->fd[0] != STDIN_FILENO)
		close(type->fd[0]);
	temp_fd = open(type->next->cmd, O_RDONLY);
	if (temp_fd == -1)
	{
		perror("Error opening fd for input redirect");
		last_status(1);
		return (-1);
	}
	type->fd[0] = temp_fd;
	type_head->fd[0] = temp_fd;
	if (type->fd[0] != STDIN_FILENO)
		type->next->fd[1] = type->fd[0];
	return (0);
}
