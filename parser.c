/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:02:07 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/02 20:40:31 by jparnahy         ###   ########.fr       */
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
    
    //printf("head: [%p]\n", *head);
    //printf("wrd: [%s]\n", wrd);

	new = (t_types *)malloc(sizeof(t_types));
	new->cmd = ft_strdup(wrd);
    new->type = what_type(wrd);
    new->fd[0] = STDIN_FILENO;
    new->fd[1] = STDOUT_FILENO;
	new->prev = NULL;
	new->next = NULL;
	if (!*head || !(*head)->cmd)
	{
        //printf("head is NULL\n");
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


void    process_input(t_init_input *input_list, t_types *types, char *prompt, t_envp *env_list)
{
    //printf("\n----\nprocess_input\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("input_list->types: [%p]\n", input_list->types);
    //printf("types: [%p]\n", types);
    //printf("env_list: [%p]\n", env_list);
    char    **args;
    char    **cmds;
    int     i;
    int     j;
    int     last_exit_status;

    (void) env_list;
    (void) input_list;

    last_exit_status = 0;
    //printf("before lexer\n");
    //printf("prompt: [%s]\n", prompt);
    cmds = lexer(prompt); // split the input for delim and quotes
    //printf("cmds was created\n");
    input_list = delim_split(prompt); // split the input for pipe
    include_fds(input_list);
    
    //printf("\n----\nafter delim_split:\n");
    //print_the_stack(input_list);

    //include_fds(input_list);
    //printf("\n----\nafter include_fds:\n");
    //print_the_stack(input_list);

    
    //printf("\n----\nfds\n");
    //printf("input_list->fd_in: [%d]\n", input_list->fd_in);
    //printf("input_list->fd_out: [%d]\n", input_list->fd_out);

    i = -1;
    //int k = 1;
    //printf("\n----\nafter lexer:\n");
    while(cmds[++i])
    {
        //printf("\n---\n[%iº command]\n", k++);
        //printf("cmds[%i]: [%s]\n", i, cmds[i]);
        j = -1;
        args = args_split(cmds[i]); // split the input for space
        //printf("\n----\nafter args_split:\n");
        while (args[++j])
        {
            //printf("args[%i]: [%s]\n", j, args[j]);
            insert_types(&types, args[j]);
        }
        args = free_from_split(args);
    }
    cmds = free_from_split(cmds);
    //define_tokens(input_list, types);
    
    //printf("\n----\nprint the ptr of list:\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("input_list->types: [%p]\n", input_list->types);
    //printf("types: [%p]\n", types);
    
    //send to expander, rever $? e $ENV~xpto
    lets_expander(types, env_list, last_exit_status);
    /*printf("\n----\nprint the types list:\n");
    t_types *temp = types;
    printf("%s\n", types->cmd);
    printf("temp: [%p]\n", temp);
    while (temp)
    {
        printf("cms: [%p]_[%s]_[%u]_[%i]_[%i]\n", temp->cmd, temp->cmd, temp->type, temp->fd[0], temp->fd[1]);
        temp = temp->next;
    }*/
    //printf("\n----\npre send to exec\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("types: [%p]\n", types);
    //printf("env_list: [%p]\n", env_list);
    //enviar para execução
    last_exit_status = to_exec(input_list, types, env_list);
	
    /*printf("\n----\nconvertion of list to char**:\n");
    int j;
    for (j = 0; args[j]; j++)
        printf("args[%i]: [%s]\n", j, args[j]);
    */
    //args_list = parser(prompt); //a ideia aqui é interpretar cada strg a partir do split de pipe
    //será preciso splitar por estaço e tokenizar os comandos.

}