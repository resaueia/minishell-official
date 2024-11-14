/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:47:41 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/13 18:07:22 by rsaueia          ###   ########.fr       */
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

int	ft_is_whitspace(char c)
{
	return (c == '\t' || c == '\v' || c == '\r' || c == ' ');
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

/*char	*ft_strdup(char *s)
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
}*/

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

int	ft_islower(char *args)
{
	int	len;
	int	i;

	len = ft_strlen(args);
	i = 0;
	while (args[i++])
	{
		if (args[i] >= 'a' || args[i] <= 'z')
			len--;
	}
	if (len == 0)
		return (0);
	else if (len > 0)
		return (1);
	else
		return (-1);
}

void remove_quotes(char **str)
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
			if(*src != '\'')
				*dst++ = *src;
		}
		*dst = '\0';
	}
}

char	*ft_joinpath(char *path, char *key, t_envp **env_list)
{
	char	*new_path;
	char	*value;
	char	*tmp;

	value = get_value(key, *env_list); //armaena o valor de HOME
	new_path = (char *)malloc(sizeof(char) * (ft_strlen(value) + ft_strlen(path) + 2)); //maloca o tamanho de value + path + 2
	if (!new_path)
		return (NULL);
	tmp = new_path; //recebe o endereço de new_path
	while (*value)
		*tmp++ = *value++; //tmp recebe o valor de value (HOME)
	*tmp++ = '/'; //home termina sem '/', atribuir para continuar com o new_path
	while (*path)
		*tmp++ = *path++; //tmp recebe o valor de path descrito no prompt e passado pelo argumento
	*tmp = '\0'; //finaliza a string
	return (new_path); //retorna o novo path
}

int	is_space(char *args)
{
	if (*args == ' ' && (args + 1 == NULL))
		return (1);
	else if (*args == ' ' && (args + 1 != NULL))
		return (2);
	return (0);
}
void	ft_bzero(void *str, size_t n)
{
	ft_memset(str, '\0', n);
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
void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (fd < 0 || !s)
		return ;
	while (s[i])
		ft_putchar_fd(s[i++], fd);
}

void	ft_putchar_fd(char c, int fd)
{
	if (fd < 0)
		return ;
	write(fd, &c, sizeof(c));
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