/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:47:41 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/21 15:06:55 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_the_stack(t_init_input *list)
{
	t_init_input	*current;

	current = list;
	while (current)
	{
		printf("string: [%s] || token: [%i] || ", current->string,
			current->token);
		printf("fd_in: [%d] || fd_out: [%i]\n", current->fd_in,
			current->fd_out);
		current = current->next;
	}
}

int	is_whitspace(char c)
{
	return (c == '\t' || c == '\v' || c == '\r' || c == ' ');
}

int	is_whitespace_string(const char *str)
{
	while (*str)
	{
		if (!is_whitspace(*str))
			return (0);
		str++;
	}
	return (1);
}

int	is_special(char c)
{
	return (c == '!' || c == '*' || c == '@');
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
	while (args[i])
	{
		if (args[i] >= 'a' && args[i] <= 'z')
			len--;
		i++;
	}
	if (len == 0)
		return (0);
	else if (len > 0)
		return (1);
	else
		return (-1);
}

void	remove_quotes(char **str)
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
			if (*src != '\'')
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

	value = get_value(key, *env_list);
	new_path = (char *)malloc(sizeof(char) * (ft_strlen(value) + ft_strlen(path)
				+ 2));
	if (!new_path)
		return (NULL);
	tmp = new_path;
	while (*value)
		*tmp++ = *value++;
	*tmp++ = '/';
	while (*path)
		*tmp++ = *path++;
	*tmp = '\0';
	return (new_path);
}

char	*ft_strndup(char *str, int len)
{
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
	return (new);
}

int	is_expander(t_types *types)
{
	if (types->cmd[0] == '$' && types->cmd[1] == '?')
		return (1);
	else
		return (0);
}

char	*extract_key(char *str)
{
	int	len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (ft_substr(str, 0, len));
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*new_str;

	new_str = ft_strjoin(s1, s2);
	free(s1);
	return (new_str);
}

int	last_status(int new_status)
{
	static int	status;

	if (new_status > -1)
		status = new_status;
	return (status);
}
