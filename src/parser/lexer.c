/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:49:45 by jparnahy          #+#    #+#             */
/*   Updated: 2025/01/07 19:26:22 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	to_quotes(char c, int quotes)
{
	if (c == '\'')
	{
		if (!quotes)
			return (1);
		else if (quotes == 1)
			return (0);
	}
	else if (c == '\"')
	{
		if (!quotes)
			return (2);
		else if (quotes == 2)
			return (0);
	}
	return (quotes);
}

static int	counter(char *input)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '>' || input[i] == '<' || input[i] == '|')
		{
			count += 2;
			if (input[i + 1] == input[i])
			{
				count++;
				i++;
			}
		}
		i++;
	}
	return (count);
}

static char	*to_replace(char *input, int position)
{
	char	*ret;
	int		i;
	int		j;
	int		len;

	len = ft_strlen(input) + counter(input) + 1;
	ret = (char *)malloc(len * sizeof (char));
	if (!ret)
		return (NULL);
	i = -1;
	j = -1;
	while (++j < position)
		ret[j] = input[++i];
	i++;
	ret[j++] = 29;
	ret[j++] = input[i++];
	if (input[i] == input[i - 1] && input[i] != '|')
		ret[j++] = input[i++];
	ret[j++] = 29;
	while (input[i] && j < len)
		ret[j++] = input[i++];
	ret[j] = '\0';
	input = free_char_ptr(input);
	return (ret);
}

char	**lexer(char *input)
{
	int		i;
	int		quotes;
	char	*temp;
	char	**ret;

	i = -1;
	quotes = 0;
	temp = ft_strdup(input);
	while (temp && temp[++i])
	{
		if ((temp[i] == '|' || temp[i] == '>' || temp[i] == '<' ) && !quotes)
		{
			temp = to_replace(temp, i);
			i = i + 2;
		}
		else if (temp[i] == '\"' || temp[i] == '\'')
			quotes = to_quotes(temp[i], quotes);
	}
	if (quotes)
		temp = free_char_ptr(temp);
	ret = the_split(temp, 29);
	temp = free_char_ptr(temp);
	return (ret);
}
