/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:03:38 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/21 20:04:16 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_empty_string(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' || is_whitspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	is_double_delim(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_strchr("|<>", str[i]))
		{
			if ((str[i] == '<' && str[i + 1] == '<') || (str[i] == '>' && str[i
						+ 1] == '>'))
				i++;
			if (ft_strchr("|<>", str[i + 1]))
				return (1);
		}
		i++;
	}
	return (0);
}

int	has_end_delim(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (ft_strchr("|<>", str[len - 1]))
		return (1);
	return (0);
}

/* I'm already ignoring white spaces in one of my split functions and
because they're properly dealt with, they're not stored into nodes, 
hence why I believe this function is redundant (for now!).*/