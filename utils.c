/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:47:41 by jparnahy          #+#    #+#             */
/*   Updated: 2024/09/23 17:06:33 by jparnahy         ###   ########.fr       */
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