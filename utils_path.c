/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:47:41 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/21 19:04:06 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
