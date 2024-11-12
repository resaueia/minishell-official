/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:47:41 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/12 12:19:37 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_the_stack(t_init_input *list)
{
	t_init_input *current = list;

	// Traverse and print the linked list
	printf("string: [%s]\n", current->string);
	//printf("args: [%s]\n", *current->args);
	printf("fd_in: [%d]\n", current->fd_in);
	printf("fd_out: [%d]\n", current->fd_out);
	printf("token: [%i]\n", current->token);
	//printf("types: [%s]\n", current->types->cmd);
	//printf("types: [%i]\n", current->types->type);	
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

char	*ft_strndup(char *str, int	len)
{
	printf("\n----\nft_strndup\n");
	printf("str: [%s]\n", str);
	char	*new;
	int		i;

	i = 0;
	new = (char *)malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);
	while (str[i] && i < len)
	{
		new[i] = str[i];
		i++;
	}
	new[i] = '\0';
	printf("new: [%s]\n", new);
	return (new);
}

int	is_expander(t_types *types)
{
	printf("\n----\nis_expander\n");
	printf("types->cmd: [%s]\n", types->cmd);
	if (types->cmd[0] == '$' && types->cmd[1] == '?')
		return (1);
	else
		return (0);
}
