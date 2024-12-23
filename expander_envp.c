/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_envp.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:20:21 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/22 21:31:01 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_invalid_prefix(char *str, int i)
{
	char	*prefix;

	prefix = ft_substr(str, 0, i);
	free(str);
	return (prefix);
}

static int	handle_special_cases(char *str, int i)
{
	int	pid;

	if (str[i + 1] == '$')
	{
		pid = getpid();
		return (pid);
	}
	return (-1);
}

static char	*extract_suffix(char *str, int *j)
{
	if (str[*j] == '-' || (str[*j] == '_' && str[*j + 1] == '\0'))
		return (ft_strdup("himBHs"));
	else if (str[*j] == '0')
		return (ft_strdup("bash"));
	while (ft_isalnum(str[*j]) || str[*j] == '_')
		(*j)++;
	return (ft_strdup(str + *j));
}

static char	*rebd_str(char *prefix, char *expanded, char *suffix, char *old)
{
	char	*new_str;
	char	*temp;
	char	*pre_temp;

	temp = NULL;
	pre_temp = prefix;
	if (pre_temp[0] == '*')
		pre_temp++;
	temp = ft_strjoin(pre_temp, expanded);
	if (*pre_temp == '#')
		new_str = ft_strdup("");
	else
		new_str = ft_strjoin(temp, suffix);
	if (temp)
		free(temp);
	if (prefix)
		free(prefix);
	if (expanded)
		free(expanded);
	if (suffix)
		free(suffix);
	if (old)
		free(old);
	return (new_str);
}

char	*env_var_expander(char *str, int i, t_envp *env_list)
{
	char	*prefix;
	char	*suffix;
	char	*expanded;
	int		pid;
	int		j;

	if (!validate_before_dollar(str, i))
		return (handle_invalid_prefix(str, i));
	prefix = ft_substr(str, 0, i);
	pid = handle_special_cases(str, i);
	if (pid == -1)
	{
		j = i + 1;
		suffix = extract_suffix(str, &j);
		expanded = get_expanded_value(str, i + 1, env_list, j);
	}
	else
	{
		suffix = ft_strdup(str + i + 2);
		expanded = ft_itoa(pid);
	}
	return (rebd_str(prefix, expanded, suffix, str));
}
