/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:02:07 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/19 16:35:54 by rsaueia          ###   ########.fr       */
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
    else if (ft_strcmp(wrd, "env") == 0)
        return (1);
    else if (ft_strcmp(wrd, "exit") == 0)
        return (1);
    else
        return (0);
}
/*
static int	what_type(char *wrd)
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
}*/
/*
static void	insert_types(t_types **head, char *wrd)
{
    //printf("\n----\non insert_types\n");
	t_types	*new;
	t_types	*temp;
    
    //printf("head: [%p]\n", *head);

	new = (t_types *)malloc(sizeof(t_types));
	new->cmd = ft_strdup(wrd);
    new->type = what_type(wrd);
	new->prev = NULL;
	new->next = NULL;
	if (!*head)
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

static char	**args_split(char *input)
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
}*/


void process_input(t_init_input *input_list, t_types *types, char *prompt, t_envp *env_list)
{
    t_init_input *args_list = NULL;
    t_init_input *args_tail = NULL;
    t_init_input *pipe_cmds[256];
    char **env;
    char **cmds;

    (void)prompt;
    env = env_to_char(env_list); // Converte env_list para char **
    cmds = list_to_char(input_list);

    // Divide os comandos
    split_commands(cmds, &args_list, &args_tail);

    // Verifica Heredoc
    if (is_heredoc(args_list) == -1) {
        perror("Error setting up heredoc");
        free_list(args_list);
        free_list(input_list);
        return;
    }

    // Configura redirecionamentos
    if (setup_redirection(args_list) == -1) {
        perror("Error while setting up redirection");
        free_list(args_list);
        free_list(input_list);
        return;
    }

    // Verifica Pipes
    if (has_pipe(args_list)) {
        split_by_pipes(args_list, pipe_cmds);
        execute_pipeline(pipe_cmds, env);
        free_list(args_list);
        free_list(input_list);
        return;
    }

    // Verifica e executa Built-in
    if (is_btin(types)) {
        execute_builtin(types->cmd, env_list, args_list);
    } else {
        // Executa com execve
        find_command_path(types, env_list);
        exec_cmd(args_list, types, env);
    }

    // Libera memória
    free_list(args_list);
    free_list(input_list);
    free(cmds);
    free(env);
}

