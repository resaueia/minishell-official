/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 23:43:15 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/20 20:00:02 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_dol(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

static void	rmv_db_qts(char **str)
{
	char	*src;
	char	*dst;

	src = *str;
	dst = *str;
	while (*src)
	{
		if (*src == '$')
		{
			if (*(src + 1) == '\"')
				src++;
		}
		if (*src != '\"')
			*dst++ = *src;
		src++;
	}
	*dst = '\0';
}

static void	rmv_sg_qts(char **str)
{
	char	*src;
	char	*dst;

	src = *str;
	dst = *str;
	while (*src)
	{
		if (*src == '$')
		{
			if (*(src + 1) == '\'')
				src++;
		}
		if (*src != '\'')
			*dst++ = *src;
		src++;
	}
	*dst = '\0';
}

static void	remove_backslashes(char *str)
{
	char	*src;
	char	*dst;

	src = str;
	dst = str;
	while (*src)
	{
		if (*src != '\\')
			*dst++ = *src;
		src++;
	}
	*dst = '\0';
}

char	*ft_strjoin_three(char *s1, char *s2, char *s3)
{
	char	*joined;
	size_t	len;

	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1;
	joined = (char *)malloc(sizeof(char) * len);
	if (!joined)
		return (NULL);
	ft_strlcpy(joined, s1, len);
	ft_strlcat(joined, s2, len);
	ft_strlcat(joined, s3, len);
	return (joined);
}

static char	*status_expander(char *str, int i, int exit_status)
{
	char	*prefix;
	char	*suffix;
	char	*status_str;
	char	*expanded;

	prefix = ft_substr(str, 0, i);
	status_str = ft_itoa(exit_status);
	suffix = ft_strdup(str + i + 2);
	expanded = ft_strjoin_three(prefix, status_str, suffix);
	free(prefix);
	free(status_str);
	free(suffix);
	free(str);
	return (expanded);
}

static int	validate_before_dollar(char *str, int i)
{
	if (str[i - 1] == '&' || str[i - 1] == '!')
		return (0);
	if (str[i - 1] == '(' || str[i - 1] == ')')
	{
		printf("minishell: syntax error near unexpected token");
		if (str[i - 1] == '(')
			printf(" `%s'\n", &str[i]);
		else if (str[i - 1] == ')')
			printf(" `%c'\n", str[i - 1]);
		return (0);
	}
	return (1);
}

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

static char	*get_expanded_value(char *str, int start, t_envp *env_list, int end)
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

static char	*rebd_str(char *prefix, char *expanded, char *suffix, char *old)
{
	char	*new_str;
	char	*temp;
	char	*pre_temp;

	pre_temp = prefix;
	if (pre_temp[0] == '*')
		pre_temp++;
	temp = ft_strjoin(pre_temp, expanded);
	if (*pre_temp == '#')
		new_str = ft_strdup("");
	else
		new_str = ft_strjoin(temp, suffix);
	free(temp);
	free(prefix);
	free(suffix);
	free(old);
	return (new_str);
}

static char	*env_var_expander(char *str, int i, t_envp *env_list)
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

static char	*to_expander(char *str, int i, t_envp *env, int exit_status)
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

static char	*expander_or_not(char *cmd, t_envp *env_list, int exit_status)
{
	int	i;

	i = 0;
	remove_backslashes(cmd);
	while (cmd[i])
	{
		if (cmd[i] == '\"')
		{
			rmv_db_qts(&cmd);
			break ;
		}
		else if (cmd[i] == '\'')
		{
			rmv_sg_qts(&cmd);
			return (cmd);
		}
		i++;
	}
	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '$')
			cmd = to_expander(cmd, i, env_list, exit_status);
	}
	return (cmd);
}

void	lets_expander(t_types *types, t_envp *env_list, int exit_status)
{
	t_envp	*env;

	env = env_list;
	while (types)
	{
		if (has_dol(types->cmd))
			types->cmd = expander_or_not(types->cmd, env, exit_status);
		types = types->next;
	}
	env = NULL;
}
