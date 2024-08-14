/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 18:06:59 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/08/14 15:54:35 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_delimiter(char c)
{
	// Checks if the character is in fact a delimiter character
	return (c == ' ' || c == '|' || c == '<' || c == '>' || c == ';');
}

static int	ft_wordcount(const char *str)
{
	int	i;
	int	word_nb;

	i = 0;
	word_nb = 0;
	while (str[i])
	{
		// skips all delimiter characters
		while (is_delimiter(str[i]))
			i++;
		// counts a word and/or a delimiter character
		if (!is_delimiter(str[i]) && str[i] != '\0')
			word_nb++;
		// Checks if the delimiter is a special character, so it can be properly dealt with
		if ((str[i] == '>' || str[i] == '<') && str[i + 1] == str[i])
			i += 2;
		else
			while (!is_delimiter(str[i]) && str[i] != '\0')
				i++;
	}
	return (word_nb);
}

static char	*custom_dup(const char *str, int start, int finish)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc((finish - start + 1) * sizeof(char));
	while (start < finish)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s)
{
	char	**megazord;
	size_t	i;
	int		start_index;
	size_t	words;

	i = 0;
	start_index = -1;
	words = 0;
	if (!s)
		return (NULL);
	megazord = (char **)malloc(sizeof(char *) * (ft_wordcount(s) + 1));
	if (!megazord)
		return (NULL);
	while (s[i])
	{
		if (!is_delimiter(s[i]) && start_index < 0)
			start_index = i;
		else if ((is_delimiter(s[i]) || s[i + 1] == '\0') && start_index >= 0)
		{
			// In case it finds a double opperand such as '>>' or '<<'
			if ((s[i] == '>' || s[i] == '<') && s[i] == s[i + 1])
				i++;
			megazord[words++] = custom_dup(s, start_index, i + (s[i + 1] == '\0'));
			start_index = -1;
		}
		i++;
	}
	megazord[words] = 0;
	return (megazord);
}
