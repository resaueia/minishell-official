/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:00:25 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/20 15:13:28 by rsaueia-         ###   ########.fr       */
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
		if (ft_strchr("|<>", str[i]))
		{
			if ((str[i] == '<' && str[i + 1] == '<') || (str[i] == '>' && str[i + 1] == '>'))
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
	int len;
    
    len = ft_strlen(str);
	if (ft_strchr("|<>", str[len - 1]))
		return (1);
	return (0);
}

/*

I'm already ignoring white spaces in one of my split functions and because they're properly dealt with,
they're not stored into nodes, hence why I believe this function is redundant (for now!).

int	has_delim_spaces(char *str)
{
	int i;
    
    i = 0;
	while (str[i])
	{
		if (ft_strchr("|<>", str[i]))
		{
			if (i > 0 && str[i - 1] == ' ') // is there a space pre-delim?
				return (1);
			if (str[i + 1] == ' ') // is there a space post-delim?
				return (1);
		}
		i++;
	}
	return (0);
}*/

/*int	quotes_check(char *str)
{
	int i;
    int single_quote;
	int double_quote;

    i = 0;
    single_quote = 0;
    double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'') // Here you have to escape the character (didn't know that haha)
			single_quote++;
		else if (str[i] == '\"') // Not sure if I have to escape double quotes too, have to test and check
			double_quote++;
		i++;
	}
	if (single_quote % 2 == 0 && double_quote % 2 == 0)
        return (1);
	return (0);
}*/

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
		if (str[i] == '\'' && double_quote == 0)
		{
			if (single_quote == 0)
				single_quote = 1;   
			else
				single_quote = 0;
		}
		else if (str[i] == '\"' && single_quote == 0)
		{
			if (double_quote == 0)
				double_quote = 1;   
			else
				double_quote = 0;
		}
		i++;
	}
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