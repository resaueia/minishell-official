/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_status.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:17:47 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/21 20:19:22 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strjoin_three(char *s1, char *s2, char *s3)
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

char	*status_expander(char *str, int i, int exit_status)
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
