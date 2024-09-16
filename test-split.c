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
	//return (c == ' ' | c == '|' || c == '<' || c == '>');
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

void	print_stack(t_init_input *stack)
{
	int i;
	t_init_input	*current;

	i = 0;
	current = stack;
	while (current)
	{
		printf("Node: %i, Valor:%s\n", i, current->string);
		current = current->next;
		i++;
	}
	//free(stack);
}
/*void	print_stack(t_init_input *stack)
{
    t_init_input *head = stack;
    t_init_input *current = head;
	while (current != NULL)
    {
        printf("%s\n", current->string);
        current = current->next;
    }
    current = head;
    t_init_input *tmp;
    while (current != NULL)
    {
        tmp = current;
        current = current->next;
        free(tmp->string);
        free(tmp);
    }
}*/

int main(void)
{
    char *input = "echo hello > file | cat < input.txt; ls -l >> output.txt";
    t_init_input *head = ft_split(input);
    t_init_input *current = head;

    print_stack(current);
	
	/*while (current != NULL)
    {
        printf("%s\n", current->string);
        current = current->next;
    }
    current = head;
    t_init_input *tmp;
    while (current != NULL)
    {
        tmp = current;
        current = current->next;
        free(tmp->string);
        free(tmp);
    }*/
	return 0;
}