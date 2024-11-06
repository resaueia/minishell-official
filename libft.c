/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:40:50 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/05 23:30:01 by jparnahy         ###   ########.fr       */
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
	//printf("entrou na strcmp\n");
	int	i;

	i = 0;
	while (str[i] == value[i] && str[i] && value[i])
		i++;
	if (value[i] == '\0' && str[i] == '\0')
	{
		//printf("ok\n");
		return (0);
	}
	else
	{
		//printf("not ok\n");
		return (1);
	}
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

char	*ft_strdup(char *str)
{
	int		i;
	char	*dup;

	i = 0;
	if (!str)
		return (NULL);
	dup = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!dup)
		return (NULL);
	while(str[i])
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

void	ft_putchar_fd(char c, int fd)
{
	if (fd < 0)
		return ;
	write(fd, &c, sizeof(c));
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (fd < 0 || !s)
		return ;
	while (s[i])
		ft_putchar_fd(s[i++], fd);
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

void	ft_bzero(void *str, size_t n)
{
	ft_memset(str, '\0', n);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*new;

	if (!s1 || !s2)
		return (NULL);
	new = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!new)
		return (0);
	i = 0;
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		new[i] = s2[j];
		i++;
		j++;
	}
	new[i] = '\0';
	return (new);
}

int	ft_isprint(int c)
{
	if (!(c >= 32 && c <= 126))
		return (0);
	else
		return (1);
}