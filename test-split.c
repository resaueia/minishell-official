#include <stdlib.h>
#include <stdio.h>

typedef struct s_init_input
{
	char					*string;
	struct s_init_input		*prev;
	struct s_init_input		*next;
}							t_init_input;

int		ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(char *str)
{
	int		i;
	char	*dup;

	if (!str)
        return (NULL);
    i = 0;
	dup = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!dup)
		return (NULL);
	while(str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int	is_delimiter(char c)
{
	// Checks if the character is in fact a delimiter character
	return (c == '|' || c == '<' || c == '>');
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

/*t_init_input	*ft_split(char *s)
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
}*/
t_init_input	*ft_split(char *s)
{
	t_init_input	*head = NULL;
	t_init_input	*tail = NULL;
	t_init_input	*new_node;
	char			*substr;
	size_t			i = 0;
	int				start_index = -1;

	if (!s)
		return (NULL);

	while (s[i])
	{
		// Skip spaces
		if (s[i] == ' ')
		{
			i++;
			continue;
		}

		// Start of a word
		if (!is_delimiter(s[i]) && start_index < 0)
			start_index = i;
		// End of a word or delimiter encountered
		else if ((is_delimiter(s[i]) || s[i + 1] == '\0') && start_index >= 0)
		{
			substr = custom_dup(s, start_index, i + (s[i + 1] == '\0' && !is_delimiter(s[i])));
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

		// Handle delimiter characters
		if (is_delimiter(s[i]))
		{
			// Check for double delimiters (like '>>' or '||')
			if ((s[i] == '>' || s[i] == '<' || s[i] == '|') && s[i + 1] == s[i])
				substr = custom_dup(s, i, i + 2), i++;
			else
				substr = custom_dup(s, i, i + 1);
			
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
		}
		i++;
	}
	return (head);
}

int main(void)
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
}