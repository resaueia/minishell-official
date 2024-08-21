/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 18:06:59 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/08/21 18:31:55 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_delimiter(char c)
{
	// Checks if the character is in fact a delimiter character
	return (c == ' ' || c == '|' || c == '<' || c == '>' || c == ';');
}

static int	ft_wordcount(char *str)
{
	int	i;
	int	word_nb;

	i = 0;
	word_nb = 0;
	while (str[i])
	{
		// skips all delimiter characters
		while (is_delimiter(str[i]))
			i++;
		// counts a word and/or a delimiter character
		if (!is_delimiter(str[i]) && str[i] != '\0')
			word_nb++;
		// Checks if the delimiter is a special character, so it can be properly dealt with
		if ((str[i] == '>' || str[i] == '<') && str[i + 1] == str[i])
			i += 2;
		else
			while (!is_delimiter(str[i]) && str[i] != '\0')
				i++;
	}
	return (word_nb);
}

static char	*custom_dup(char *str, int start, int finish)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc((finish - start + 1) * sizeof(char));
	while (start < finish)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

t_init_input	*add_node(char **input)
{
	t_init_input	*new_node;

	new_node = (t_init_input *)malloc(sizeof(t_init_input));
	if (!new_node)
		return (NULL);
	new_node->string = ft_strdup(input);
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}

char	**ft_split(char *s)
{
	t_init_input	*head;
	t_init_input	*tail;
	t_init_input	*new_node;
	char			*substr;
	size_t	i;
	int		start_index;

	i = 0;
	start_index = -1;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (!is_delimiter(s[i]) && start_index < 0)
			start_index = i;
		else if ((is_delimiter(s[i]) || s[i + 1] == '\0') && start_index >= 0)
		{
			// In case it finds a double opperand such as '>>' or '<<'
			if ((s[i] == '>' || s[i] == '<') && s[i] == s[i + 1])
				i++;
			substr = custom_dup(s, start_index, i + (s[i + 1] == '\0'));
			new_node = add_node(substr);
			free(substr);
			if (!head)
				head = new_node;
			else
			{
				tail->next = new_node;
				new_node->prev = tail;
			}
			tail = new_node;
			start_index = -1;
		}
		i++;
	}
	return (head);
}
int main(void)
{
    const char *input = "echo hello > file | cat < input.txt; ls -l >> output.txt";
    t_init_input *head = ft_split(input);
    t_init_input *current = head;

    // Traverse and print the linked list
    while (current != NULL)
    {
        printf("%s\n", current->string);
        current = current->next;
    }

    // Free the linked list
    current = head;
    t_init_input *tmp;
    while (current != NULL)
    {
        tmp = current;
        current = current->next;
        free(tmp->string);
        free(tmp);
    }
    return 0;
}