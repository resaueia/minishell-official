/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_validations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:08:04 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/22 20:47:17 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_dol(char *cmd)
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

int	validate_before_dollar(char *str, int i)
{
	if (i == 0)
		return (1);	
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
