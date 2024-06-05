/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 18:22:34 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/06/05 18:27:01 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Check if a character is one of the delimiters
static int is_delimiter(char c, const char *delimiters) {
    while (*delimiters) {
        if (c == *delimiters)
            return 1;
        delimiters++;
    }
    return 0;
}

// Count the number of words in the string separated by delimiters
static int ft_token_count(const char *str, const char *delimiters) {
    int i = 0;
    int word_nb = 0;

    while (str[i]) {
        while (is_delimiter(str[i], delimiters))
            i++;
        if (!is_delimiter(str[i], delimiters) && str[i] != '\0')
            word_nb++;
        while (!is_delimiter(str[i], delimiters) && str[i] != '\0')
            i++;
    }
    return word_nb;
}

// Duplicate a substring
static char *custom_dup(const char *str, int start, int finish) {
    char *word = malloc((finish - start + 1) * sizeof(char));
    int i = 0;

    while (start < finish)
        word[i++] = str[start++];
    word[i] = '\0';
    return word;
}

// Split the string into an array of words using multiple delimiters
char **ft_split(char const *s, const char *delimiters) {
    char **megazord;
    size_t i = -1;
    int start_index = -1;
    size_t words = 0;

    if (!s)
        return NULL;

    megazord = (char **)malloc(sizeof(char *) * (ft_token_count(s, delimiters) + 1));
    if (!megazord)
        return NULL;

    while (++i <= strlen(s)) {
        if (!is_delimiter(s[i], delimiters) && start_index < 0)
            start_index = i;
        else if ((is_delimiter(s[i], delimiters) || i == strlen(s)) && start_index >= 0) {
            megazord[words++] = custom_dup(s, start_index, i);
            start_index = -1;
        }
    }
    megazord[words] = 0;
    return megazord;
}

