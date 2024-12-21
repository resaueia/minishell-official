/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:28:09 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/21 19:37:26 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_quotes(char **str)
{
	char	*src;
	char	*dst;

	src = *str;
	dst = *str;
	if (*src == '\"')
	{
		while (*src++)
		{
			if (*src != '\"')
				*dst++ = *src;
		}
		*dst = '\0';
	}
	else if (*src == '\'')
	{
		while (*src++)
		{
			if (*src != '\'')
				*dst++ = *src;
		}
		*dst = '\0';
	}
}

void	remove_quotes_from_str(char **str)
{
	char	*src;
	char	*dst;

	if (!str || !*str)
		return ;
	src = *str;
	dst = *str;
	if (*src == '\"')
	{
		while (*src++)
		{
			if (*src != '\"')
				*dst++ = *src;
		}
		*dst = '\0';
	}
	else if (*src == '\'')
	{
		while (*src++)
		{
			if (*src != '\'')
				*dst++ = *src;
		}
		*dst = '\0';
	}
}

void	remove_quotes_from_types(t_types *types)
{
	t_types	*current;

	current = types;
	while (current)
	{
		if (current->cmd)
			remove_quotes_from_str(&current->cmd);
		current = current->next;
	}
}
