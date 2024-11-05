/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:47:41 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/04 22:45:04 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_the_stack(t_init_input *list)
{
	t_init_input *current = list;

	// Traverse and print the linked list
	while (current != NULL)
	{
		printf("string: [%s] - token [%u]\n", current->string, current->token);
		current = current->next;
	}
}

int	is_whitspace(char c)
{
	return (c == '\t' || c == '\v' || c == '\r' || c == ' ');
}

int	is_space(char *args)
{
	if (*args == ' ' && (args + 1 == NULL))
		return (1);
	else if (*args == ' ' && (args + 1 != NULL))
		return (2);
	return (0);
}

int	is_number(char c)
{
	return (c >= '0' && c <= '9');
}

int	is_lower(char *args)
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

char	*joinpath(char *path, char *key, t_envp **env_list)
{
	char	*new_path;
	char	*value;
	char	*tmp;

	value = get_value(key, *env_list); //armazena o valor de HOME
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

