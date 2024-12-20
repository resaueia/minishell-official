/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   what_type.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:22:35 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/20 18:17:10 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_hdoc(t_types *type)
{
	t_types	*tmp;

	tmp = type;
	while (tmp)
	{
		if (tmp->type == 44)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	is_pp(t_types *type)
{
	t_types	*tmp;

	tmp = type;
	while (tmp)
	{
		if (tmp->type == 11)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	is_rdrct(t_types *type)
{
	t_types	*tmp;

	tmp = type;
	while (tmp)
	{
		if (tmp->type == 04 || tmp->type == 07 || tmp->type == 77)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	is_btin(t_types *type)
{
	t_types	*tmp;

	tmp = type;
	while (tmp)
	{
		if (tmp->type == 03)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	is_exec(t_types *type)
{
	t_types *tmp;

	tmp = type;
	while (tmp)
	{
		if (tmp->type == 05)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}