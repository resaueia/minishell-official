/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:00:25 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/18 19:36:33 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_empty_string(char *str)
{
	int i;
    
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
	int i;
    
    i = 0;
	while (str[i])
	{
		if (ft_strchr("|<>", str[i])) // is it a delim char?
		{
			if ((str[i] == '<' && str[i + 1] == '<') || (str[i] == '>' && str[i + 1] == '>')) // here we check for heredoc and redirect
				i++;
			if (ft_strchr("|<>", str[i + 1])) // is the next one also a delim?
				return (1);
		}
		i++;
	}
	return (0);
}

int	has_end_delim(char *str)
{
	int len;
    
    len = ft_strlen(str);
	if (ft_strchr("|<>", str[len - 1])) // is the last character a delim?
		return (1);
	return (0);
}

int	quotes_check(char *str)
{
	int single_quote;
	int double_quote;
	int i;

	single_quote = 0;
	double_quote = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && double_quote == 0) // Initial condition, finds single quotes (as long as double ones are off)
		{
			if (single_quote == 0)  // If off then turn it on
				single_quote = 1;   
			else
				single_quote = 0;   // else turns it off
		}
		else if (str[i] == '\"' && single_quote == 0) // Same here, needs to check if it's inside single quotes
		{
			if (double_quote == 0)  // If off then turn it on
				double_quote = 1;   
			else
				double_quote = 0;   // else they're off
		}
		i++;
	}
	// If by the end of the check either of them are still on, they're not balanced
	if (single_quote == 1 || double_quote == 1)
		return (0);  
	return (1);
}

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