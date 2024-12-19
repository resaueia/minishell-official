/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:02:07 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/19 19:44:43 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *wrd)
{
    if (ft_strcmp(wrd, "echo") == 0)
        return (1);
    else if (ft_strcmp(wrd, "cd") == 0)
        return (1);
    else if (ft_strcmp(wrd, "pwd") == 0)
        return (1);
    else if (ft_strcmp(wrd, "export") == 0)
        return (1);
    else if (ft_strcmp(wrd, "unset") == 0)
        return (1);
    else if (ft_strcmp(wrd, "env") == 0 || ft_strcmp(wrd, "envp") == 0)
        return (1);
    else if (ft_strcmp(wrd, "exit") == 0)
        return (1);
    else
        return (0);
}

int	what_type(char *wrd)
{
    if (ft_strcmp(wrd, "|") == 0)
        return (PIPE);
    else if (ft_strcmp(wrd, ">") == 0)
        return (OUT);
    else if (ft_strcmp(wrd, ">>") == 0)
        return (APPEND);
    else if (ft_strcmp(wrd, "<") == 0)
        return (IN);
    else if (ft_strcmp(wrd, "<<") == 0)
        return (HDOC);
    else if (is_builtin(wrd))
        return (BUILTIN);
    else
        return (WORD);
}

void	insert_types(t_types **head, char *wrd)
{
	t_types	*new;
	t_types	*temp;

	new = (t_types *)malloc(sizeof(t_types));
	new->cmd = ft_strdup(wrd);
    new->type = what_type(wrd);
    new->fd[0] = STDIN_FILENO;
    new->fd[1] = STDOUT_FILENO;
	new->prev = NULL;
	new->next = NULL;
	if (!*head || !(*head)->cmd)
	{
		*head = new;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	new->prev = temp;
	temp->next = new;
}

char	**args_split(char *input)
{
	int		i;
	int		quotes;
	char	*temp;
	char	**ret;

	i = -1;
	quotes = 0;
	temp = NULL;
	while (input[++i])
	{
		if (input[i] == ' ' && !quotes)
			input[i] = 29;
		else if (input[i] == '\"' || input[i] == '\'')
			quotes = to_quotes(input[i], quotes);
	}
	ret = the_split(input, 29);
	temp = free_char_ptr(temp);
	return (ret);
}

void include_fds(t_init_input *input_list)
{
    t_init_input *temp;

    temp = input_list;
    while (temp)
    {
        temp->fd_in = 0;
        temp->fd_out = 1;
        temp = temp->next;
    }
}

static void remove_quotes_from_str(char **str)
{
    char *src;
    char *dst;

    if (!str || !*str)
        return;
    src = *str;
    dst = *str;
   if (*src == '\"')
	{
		while (*src++)
		{
			if (*src != '\"')
				*dst++ = *src;
		}
		*dst = '\0';
	}
	else if (*src == '\'')
	{
		while (*src++)
		{
			if(*src != '\'')
				*dst++ = *src;
		}
		*dst = '\0';
	}
}

static void remove_quotes_from_types(t_types *types)
{
    t_types *current;

    current = types;
    while (current)
    {
        if (current->cmd)
            remove_quotes_from_str(&current->cmd); // Remove aspas do cmd
        current = current->next;
    }
}


void    process_input(t_init_input *input_list, t_types *types, char *prompt, t_envp *env_list)
{
    char    **args;
    char    **cmds;
    int     i;
    int     j;
    
    cmds = lexer(prompt); // split the input for delim and quotes
    input_list = delim_split(prompt); // split the input for pipe
    include_fds(input_list);
    i = -1;
    while(cmds[++i]) //include types
    {
        j = -1;
        args = args_split(cmds[i]); // split the input for space
        while (args[++j])
            insert_types(&types, args[j]);
        args = free_from_split(args);
    }
    cmds = free_from_split(cmds);
    //printf("\n----\nprint the types before args_of_cmds:\n");
    t_types *temp = types;
    /*printf("temp: [%p]\n", temp);
    while (temp)
    {
        printf("cms: [%s]_[%u]_[%i]_[%i]\n", temp->cmd, temp->type, temp->fd[0], temp->fd[1]);
        temp = temp->next;
    }*/
    args_of_cmds(types);

    printf("\n----\nprint the types afeter args_of_cmds:\n");
    temp = types;
    printf("temp: [%p]\n", temp);
    while (temp)
    {
        printf("cms: [%s]_[%u]_[%i]_[%i]\n", temp->cmd, temp->type, temp->fd[0], temp->fd[1]);
        temp = temp->next;
    }
    lets_expander(types, env_list, input_list->exit_status);
    remove_quotes_from_types(types);
    input_list->exit_status = to_exec(input_list, types, env_list);
}
