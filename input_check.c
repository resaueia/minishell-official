/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:00:25 by rsaueia-          #+#    #+#             */
/*   Updated: 2025/01/06 18:15:12 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
