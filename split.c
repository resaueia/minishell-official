/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 16:23:52 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/06/06 16:24:25 by rsaueia-         ###   ########.fr       */
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
static int ft_wordcount(const char *str, const char *delimiters) {
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

// Tokenize a command into arguments
char **tokenize_command(const char *command) {
    char **tokens = malloc(1024 * sizeof(char *));
    char *token;
    int i = 0;

    char *cmd_copy = strdup(command);  // Duplicate command to use with strtok
    token = strtok(cmd_copy, " \t\r\n");
    while (token != NULL) {
        tokens[i++] = strdup(token);  // Duplicate token to store in tokens array
        token = strtok(NULL, " \t\r\n");
    }
    tokens[i] = NULL;
    free(cmd_copy);  // Free the duplicated command string
    return tokens;
}

// Split the string into an array of commands using multiple delimiters
char ***ft_split(const char *s, const char *delimiters, int *num_commands) {
    char ***result;
    size_t i = -1;
    int start_index = -1;
    size_t words = 0;

    if (!s)
        return NULL;

    *num_commands = ft_wordcount(s, delimiters);
    result = (char ***)malloc(sizeof(char **) * (*num_commands + 1));
    if (!result)
        return NULL;

    while (++i <= strlen(s)) {
        if (!is_delimiter(s[i], delimiters) && start_index < 0)
            start_index = i;
        else if ((is_delimiter(s[i], delimiters) || i == strlen(s)) && start_index >= 0) {
            char *command = custom_dup(s, start_index, i);
            result[words++] = tokenize_command(command);
            free(command);  // Free the duplicated command string
            start_index = -1;
        }
    }
    result[words] = NULL;
    return result;
}

