/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:44:41 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/18 19:44:00 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wordcount(const char *str, char brk)
{
	int	groups;
	int	i;

	i = 0;
	groups = 0;
	while (str[i])
	{
		if (str[i] != brk)
		{
			groups++;
			while (str[i] && str[i] != brk)
				i++;
		}
		else
			i++;
	}
	return (groups);
}

static int	wordlen(const char *str, char brk)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != brk)
		i++;
	return (i);
}

static char	**free_split(char **str, int i)
{
	while (i >= 0)
		free(str[i--]);
	free(str);
	return (NULL);
}

char	**the_split(char const *s, char c)
{
	char	**splt;
	int		i;
	int		j;

	splt = (char **)malloc((wordcount((char *)s, c) + 1) * sizeof(*splt));
	if (!splt || !s)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			splt[j] = ft_substr((char *)s, i, wordlen(&s[i], c));
			if (!splt[j])
				return (free_split(splt, j));
			j++;
			i += wordlen(&s[i], c);
		}
		else
			i++;
	}
	splt[j] = 0;
	return (splt);
}
