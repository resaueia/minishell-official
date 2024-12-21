/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:00:25 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/21 16:47:18 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_empty_string(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' || is_whitspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	is_double_delim(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_strchr("|<>", str[i]))
		{
			if ((str[i] == '<' && str[i + 1] == '<') || (str[i] == '>' && str[i
						+ 1] == '>'))
				i++;
			if (ft_strchr("|<>", str[i + 1]))
				return (1);
		}
		i++;
	}
	return (0);
}

int	has_end_delim(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (ft_strchr("|<>", str[len - 1]))
		return (1);
	return (0);
}

/* I'm already ignoring white spaces in one of my split functions and
because they're properly dealt with, they're not stored into nodes, 
hence why I believe this function is redundant (for now!).*/

static void	toggle_quote(char c, int *single_quote, int *double_quote)
{
	if (c == '\'' && *double_quote == 0)
	{
		if (*single_quote == 0)
			*single_quote = 1;
		else
			*single_quote = 0;
	}
	else if (c == '\"' && *single_quote == 0)
	{
		if (*double_quote == 0)
			*double_quote = 1;
		else
			*double_quote = 0;
	}
}

/* Function: toggle_quote
 * Alternates the state of `single_quote` and `double_quote` based on the
 * character `c`, ensuring that quotes are toggled correctly without conflicts.
 */

int	quotes_check(char *str)
{
	int	single_quote;
	int	double_quote;
	int	i;

	single_quote = 0;
	double_quote = 0;
	i = 0;
	while (str[i])
	{
		toggle_quote(str[i], &single_quote, &double_quote);
		i++;
	}
	if (single_quote == 1 || double_quote == 1)
		return (0);
	return (1);
}

/* Function: quotes_check
 * Checks if all quotes in the input string are properly closed.
 * Returns 1 if the quotes are balanced, or 0 if there are unmatched quotes.
 */

int	input_check(char *input)
{
	if (is_double_delim(input))
	{
		ft_putstr_fd("Error: Double delimiters were found.\n", 2);
		return (1);
	}
	if (has_end_delim(input))
	{
		ft_putstr_fd("Error: Delimiter at the end of input.\n", 2);
		return (1);
	}
	if (!quotes_check(input))
	{
		ft_putstr_fd("Error: Quotes haven't been properly closed.\n", 2);
		return (1);
	}
	return (0);
}
