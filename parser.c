/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:02:07 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/18 19:39:05 by jparnahy         ###   ########.fr       */
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
    //printf("\n----\non insert_types\n");
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

static int is_delim(int type)
{
    if (type == PIPE || type == IN || type == OUT || type == APPEND || type == HDOC)
        return (1);
    return (0);
}

static int  is_redirects(int type)
{
    if (type == IN || type == OUT || type == APPEND || type == HDOC)
        return (1);
    return (0);
}

void args_of_cmds(t_types *cmd)
{
    t_types *head;
    int     node_ref;

    head = cmd;
    node_ref = 1;
    while (cmd)
    {
        if (node_ref == 1)
        {
            if (is_redirects(cmd->type))
            {
                if ((ft_strcmp(cmd->cmd, ">") == 0 || ft_strcmp(cmd->cmd, "<") == 0) && cmd->next->cmd)
                    cmd->next->type = FLE;
                else if (ft_strcmp(cmd->cmd, "<<") == 0 && cmd->next->cmd)
                    cmd->next->type = ENDOF;
            }
            else if (is_builtin(cmd->cmd) == 1)
            {
                if (ft_strcmp(cmd->cmd, "echo") == 0 && !cmd->next)
                    break;
                if (ft_strcmp(cmd->cmd, "echo") == 0 && cmd->next->cmd)
                {
                    cmd = cmd->next;
                    while (cmd && is_delim(cmd->type) == 0)
                    {
                        cmd->type = ARGS;
                        cmd = cmd->next;
                    }
                    if (!cmd)
                        break;
                }
            }
            else
                cmd->type = EXEC;
            node_ref = 0;
        }
        if (cmd->type == PIPE)
            node_ref = 1;
        if (node_ref == 0)
        {
            if (is_redirects(cmd->type))
            {
                if ((ft_strcmp(cmd->cmd, ">") == 0 || ft_strcmp(cmd->cmd, "<") == 0) && cmd->next->cmd)
                    cmd->next->type = FLE;
                else if (ft_strcmp(cmd->cmd, "<<") == 0 && cmd->next->cmd)
                    cmd->next->type = ENDOF;
            }
            else if (is_builtin(cmd->cmd) == 1)
            {
                if (ft_strcmp(cmd->cmd, "echo") == 0 && cmd->next->cmd)
                {
                    cmd = cmd->next;
                    while (cmd && is_delim(cmd->type) == 0)
                    {
                        cmd->type = ARGS;
                        cmd = cmd->next;
                    }
                    if (!cmd)
                        break;
                }
            }
            else if (ft_strcmp(cmd->cmd, "Makefile") == 0)
                cmd->type = FLE;
        }
        cmd = cmd->next;
    }
    cmd = head;
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

void    process_input(t_init_input *input_list, t_types *types, char *prompt, t_envp *env_list)
{
    char    **args;
    char    **cmds;
    int     i;
    int     j;
    int     last_exit_status;

    last_exit_status = 0;
    cmds = lexer(prompt); // split the input for delim and quotes
    input_list = delim_split(prompt); // split the input for pipe
    include_fds(input_list);
    i = -1;
    while(cmds[++i])
    {
        j = -1;
        args = args_split(cmds[i]); // split the input for space
        while (args[++j])
            insert_types(&types, args[j]);
        args = free_from_split(args);
    }
    cmds = free_from_split(cmds);
    args_of_cmds(types);
    lets_expander(types, env_list, last_exit_status); //send to expander, rever $? e $ENV~xpto
    last_exit_status = to_exec(input_list, types, env_list); //enviar para execução
}
