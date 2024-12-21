/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:40:39 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/21 19:46:20 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *wrd)
{
	if (ft_strcmp(wrd, "echo") == 0)
		return (1);
	else if (ft_strcmp(wrd, "cd") == 0)
		return (1);
	else if (ft_strcmp(wrd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(wrd, "export") == 0)
		return (1);
	else if (ft_strcmp(wrd, "unset") == 0)
		return (1);
	else if (ft_strcmp(wrd, "env") == 0 || ft_strcmp(wrd, "envp") == 0)
		return (1);
	else if (ft_strcmp(wrd, "exit") == 0)
		return (1);
	else
		return (0);
}

int	what_type(char *wrd)
{
	if (ft_strcmp(wrd, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(wrd, ">") == 0)
		return (OUT);
	else if (ft_strcmp(wrd, ">>") == 0)
		return (APPEND);
	else if (ft_strcmp(wrd, "<") == 0)
		return (IN);
	else if (ft_strcmp(wrd, "<<") == 0)
		return (HDOC);
	else if (is_builtin(wrd))
		return (BUILTIN);
	else
		return (WORD);
}

int	is_delim(int type)
{
	if (type == PIPE || type == IN || type == OUT || type == APPEND
		|| type == HDOC)
		return (1);
	return (0);
}

int	is_redirects(int type)
{
	if (type == IN || type == OUT || type == APPEND || type == HDOC)
		return (1);
	return (0);
}
