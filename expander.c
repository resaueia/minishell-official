/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:04:31 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/21 20:27:08 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_expanded_value(char *str, int start, t_envp *env_list, int end)
{
	char	*key;
	char	*value;

	if (ft_isdigit(str[start]) || is_special(str[start]))
	{
		start++;
		if (start == end)
			return (ft_strdup(""));
		else
		{
			key = ft_substr(str, start, end - start);
			value = ft_strdup(key);
		}
	}
	else
	{
		key = ft_substr(str, start, end - start);
		if (is_key(key, env_list))
			value = get_value(key, env_list);
		else
			value = ft_strdup("");
	}
	free(key);
	return (value);
}

char	*to_expander(char *str, int i, t_envp *env, int exit_status)
{
	char	*expanded;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
			{
				expanded = status_expander(str, i, exit_status);
				last_status(0);
				return (expanded);
			}
			else if (str[i + 1] != '\0' && str[i + 1] != '?')
			{
				expanded = env_var_expander(str, i, env);
				if (*expanded == '(' || *expanded == ')')
					return (ft_strdup(""));
				return (expanded);
			}
		}
		i++;
	}
	return (str);
}
