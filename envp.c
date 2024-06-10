/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 16:02:16 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/06/07 16:02:37 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Main structure (defined on main) */

typedef struct s_envp
{
	char	*key;
	char	*value;
	struct s_envp	*next;
}				t_envp;

t_envp      *create_node(char *key, char *value)
{
    t_envp  *new_node;

    new_node = (t_envp *)malloc(sizeof(t_envp));
    if (!new_node)
        return (NULL);

    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = NULL;

    return (new_node);
}

t_envp *get_envp(char **envp)
{
    t_envp  *head;
    t_envp  *current;
    t_envp  *new_node;
    char    *input;
    char    *delim;
    char    *key;
    char    *value;

    head = NULL;
    current = NULL;
    while (*envp)
    {
        input = envp;
        delim = ft_strchr(input, '=');
        if (delim)
        {
            key = ft_strdup(input - delim);
            value = ft_strdup(delim + 1);
            new_node = create_node(key, value);
            if (!head)
                head = new_node;
            else
            current->next = new_node;
            free(key);
            free(value);
        }
        envp++;
    }
    return (head);
}

void print_envp_list(t_envp *head)
{
    t_envp *current
    
    current = head;
    while (current)
    {
        printf("Key: %s, Value: %s\n", current->key, current->value);
        current = current->next;
    }
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    t_envp *env_list;

    env_list = get_envp(envp);
    print_envp_list(env_list);
    return (0);
}

