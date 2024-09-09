/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delim_splitter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 19:33:43 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/09/09 19:33:48 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     is_delimiter(char c)
{
    return (c == '|' || c == '>' || c == '<');
}

char    custom_dup(char *str, int start, int finish)
{
    char    *dup;
    int     i;
    
    i = 0;
    dup = (char *)malloc(sizeof(char) * (finish - start) + 1);
    while (start < finish)
        dup[i++] = str[start++];
    dup[i] = '\0';
    return (dup);
}