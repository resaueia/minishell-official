/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:47:41 by jparnahy          #+#    #+#             */
/*   Updated: 2024/09/03 16:45:39 by jparnahy         ###   ########.fr       */
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
		return (0);
	else
		return (1);
}

int	ft_strncmp(char *s1, char *s2, size_t n)
{
	//printf("\nn: %zu\n", n);
	//printf("s1: %s\n", s1);
	//printf("s2: %s\n", s2);
	--n;
	while (s1[n] == s2[n] && s1[n] && s2[n])
	{
		//printf("s1[%li]: %c | s2[%li]: %c\n", n, s1[n], n, s2[n]);
		if (n == 0)
			return (0);
		n--;
	}
	//printf("NOK\n--");
	return (1);
}

char	*ft_strdup(char *s)
{
	char	*test;
	char	*ptr_keeper;

	test = (char *)malloc(sizeof(char) * ft_strlen(s) + 1);
	if (!test)
		return (NULL);
	ptr_keeper = test;
	while (*s)
	{
		*test = *s;
		test++;
		s++;
	}
	*test = '\0';
	return ((char *)ptr_keeper);
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
void remove_quotes(char **str)
{
	//printf("pointer of str: [%p]\n", *str);
	//printf("string of str: [%s]\n", *str);
	//printf("char of str: [%c]\n", **str);
	char	*src;
	char	*dst;

	src = *str;
	dst = *str;

	//printf("pointer of src: [%p]\n", src);
	//printf("char of src: [%c]\n", *src);
	//printf("pointer of dst: [%p]\n", dst);

	if (*src == '\"')
	{
		src++;
		while (*src && *src != '\"')
			*dst++ = *src++;
		*dst = '\0';
	}
	else if (*src == '\'')
	{
		src++;
		while (*src && *src != '\'')
			*dst++ = *src++;
		*dst = '\0';
	}
	else
	{
		while (*src)
			*dst++ = *src++;
	}
}