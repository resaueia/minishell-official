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

t_envp *get_envp(char **envp)
{
    t_envp  *head;
    t_envp  *current;
    char    *input;
    char    *delim;
    char    *key;
    char    *value;

    head = NULL;
    current = NULL;
    while (*envp)
    {
        input = *envp;
        delim = ft_strchr(input, '=');
        if (delim)
            key = ft_strdup(input - delim);
            value = ft_strdup(delim + 1);
        free(key);
        free(value);
        envp++;
    }
    return (head);
}

t_envp      *create_node(char *key, char *value)
{
    t_envp  *new_node;

    new_node = (t_envp *)ft_calloc(sizeof(t_envp));
    if (!new_node)
        return (NULL);

    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = NULL;

    return (new_node);
}
