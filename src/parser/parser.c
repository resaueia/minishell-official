/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:02:07 by jparnahy          #+#    #+#             */
/*   Updated: 2025/01/07 19:16:38 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	insert_types(t_types **head, char *wrd)
{
	t_types	*new;
	t_types	*temp;

	if (!wrd || wrd[0] == '\0')
		return ;
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

void	include_fds(t_init_input *input_list)
{
	t_init_input	*temp;

	temp = input_list;
	while (temp)
	{
		temp->fd_in = 0;
		temp->fd_out = 1;
		temp = temp->next;
	}
}

void	split_and_insert(t_types **types, char **cmds)
{
	char	**args;
	int		i;
	int		j;

	i = -1;
	while (cmds[++i])
	{
		args = args_split(cmds[i]);
		j = -1;
		while (args[++j])
			insert_types(types, args[j]);
		args = free_from_split(args);
	}
}

void	process_input(t_init_input *input_list, t_types *types, char *prompt,
		t_envp *env_list)
{
	char	**cmds;

	if (!prompt || *prompt == '\0')
		return ;
	cmds = lexer(prompt);
	input_list = delim_split(prompt);
	include_fds(input_list);
	free(prompt);
	prompt = NULL;
	split_and_insert(&types, cmds);
	cmds = free_from_split(cmds);
	args_of_cmds(types);
	lets_expander(types, env_list, last_status(-1));
	remove_quotes_from_types(types);
	if (to_exec(input_list, types, env_list))
		input_list = NULL;
}
