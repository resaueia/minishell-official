/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:57:55 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/22 20:39:24 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_bzero(void *str, size_t n)
{
	ft_memset(str, '\0', n);
}

char	*ft_strdup(char *str)
{
	int		i;
	char	*dup;

	dup = NULL;
	i = 0;
	if (!str)
		return (NULL);
	dup = (char *)calloc(sizeof(char), (ft_strlen(str) + 1));
	if (!dup)
		return (NULL);
	while (str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_strchr(char *s, int c)
{
	while (*s)
	{
		if (*s == c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}

char	*ft_substr(char *str, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;

	i = 0;
	substr = (char *)malloc(sizeof(char) * (len + 1));
	if (!substr)
		return (NULL);
	while (i < len && str[start])
	{
		substr[i] = str[start];
		i++;
		start++;
	}
	substr[i] = '\0';
	return (substr);
}

void	*ft_memset(void *dest, int c, size_t n)
{
	unsigned long int	i;
	unsigned char		*_dest;
	unsigned char		_c;

	i = 0;
	_dest = (unsigned char *)dest;
	_c = (unsigned char)c;
	while (i < n)
	{
		_dest[i++] = _c;
	}
	return (dest);
}
