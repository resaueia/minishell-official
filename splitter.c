/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 18:06:59 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/08/29 15:50:51 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_delimiter(char c)
{
	// Checks if the character is in fact a delimiter character
	//return (c == ' ' || c == '|' || c == '<' || c == '>');
	return (c == ' ');
}

char	*custom_dup(char *str, int start, int finish)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc(sizeof(char) * (finish - start + 1));
	while (start < finish)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

t_init_input	*add_node(char *input)
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

t_init_input	*ft_split(char *s)
{
	t_init_input	*head = NULL;
	t_init_input	*tail = NULL;
	t_init_input	*new_node;
	char			*substr;
	size_t			i;
	int				start_index;

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
			if (!substr)
				return (NULL);
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
/*int main(void)
{
    char *input = "echo hello > file | cat < input.txt; ls -l >> output.txt";
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
}*/