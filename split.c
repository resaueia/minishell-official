/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 18:22:34 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/06/13 17:40:26 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Thinking of this split res, gotta test to see if it makes sense!*/

char	**ft_split(char *str, char delim)
{
	char	**result;
	int		count;
	int		i;
	char	*token;

	count = 1; // if no delimeters, there's still one substring
	for (i = 0; str[i]; i++) //count the delimiters
		if (str[i] == delim)
			count++;

	result = (char **)malloc((count + 1) * sizeof(char *));
	if (!result)
		return (NULL);

	i = 0;
	token = strtok(str, &delim); // this is where we have to find a way to tokenize our input
	while (token)
	{
		result[i] = ft_strdup(token); //Here, each token is duplicated and stored in the result array
		token = strtok(NULL, &delim); //Continue tokenization
		i++;
	}
	result[i] = NULL;
	return (result);
}

void    free_split_res(char **result)
{
    int i;

    i = 0;
    while (result[i])
    {
        free(result[i]);
        i++;
    }
    free(result);
}

