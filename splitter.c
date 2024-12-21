/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 18:06:59 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/21 18:53:37 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_delimiter(char c)
{
	return (c == ' ' || (c > 8 && c < 14));
}

/* Function: ft_split
 * Splits the input string into nodes based on delimiters (spaces or control
 * characters). Each substring is stored as a node in a linked list.
 * The function uses helper functions to handle substring extraction and node
 * creation, ensuring clarity and modularity.
 */

t_init_input	*ft_split(char *s)
{
	t_init_input	*head;
	size_t			i;
	int				start_index;

	head = NULL;
	i = 0;
	start_index = -1;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (!is_delimiter(s[i]) && start_index < 0)
			start_index = i;
		else if ((is_delimiter(s[i]) || s[i + 1] == '\0') && start_index >= 0)
			handle_substring(s, &i, &start_index, &head);
		i++;
	}
	return (head);
}
