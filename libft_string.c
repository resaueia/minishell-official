/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thfranco <thfranco@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:56:28 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/24 17:33:14 by thfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_strcmp(char *str, char *value)
{
	int	i;

	i = 0;
	while (str[i] == value[i] && str[i] && value[i])
		i++;
	if (value[i] == '\0' && str[i] == '\0')
	{
		return (0);
	}
	else
	{
		return (1);
	}
}

size_t	ft_strlcpy(char *dest, const char *src, size_t sz)
{
	size_t	i;
	size_t	slen;

	slen = ft_strlen((char *)src);
	i = 0;
	if (sz != 0)
	{
		while (src[i] != '\0' && i < (sz - 1))
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (slen);
}

size_t	ft_strlcat(char *dest, const char *src, size_t sz)
{
	unsigned long int	i;
	unsigned long int	j;
	unsigned long int	len_d;
	unsigned long int	len_s;

	len_s = ft_strlen((char *)src);
	len_d = ft_strlen((char *)dest);
	if (!dest && !src && sz == 0)
		return (0);
	if (sz < len_d || !sz)
		return (len_s + sz);
	i = 0;
	j = len_d;
	while (sz > (j + 1) && src[i] != '\0')
		dest[j++] = src[i++];
	dest[j] = '\0';
	return (len_s + len_d);
}

int	ft_strncmp(char *s1, char *s2, size_t n)
{
	--n;
	while (s1[n] == s2[n] && s1[n] && s2[n])
	{
		if (n == 0)
			return (0);
		n--;
	}
	return (1);
}
