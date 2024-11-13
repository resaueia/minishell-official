/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:02:07 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/13 17:36:08 by jparnahy         ###   ########.fr       */
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
}

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
}


void    process_input(t_init_input *input_list, t_types *types, char *prompt, t_envp *env_list)
{
    printf("\n----\nprocess_input\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("input_list->types: [%p]\n", input_list->types);
    //printf("types: [%p]\n", types);
    //printf("env_list: [%p]\n", env_list);
    char    **args;
    char    **cmds;
    int     i;
    int     j;

    (void) env_list;
    (void) input_list;

    cmds = lexer(prompt); // split the input for delim and quotes
    //input_list = delim_split(prompt); // split the input for pipe

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
            insert_types(&types, args[j]);
        args = free_from_split(args);
    }
    cmds = free_from_split(cmds);
    //define_tokens(input_list, types);
    
    //printf("\n----\nprint the ptr of list:\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("input_list->types: [%p]\n", input_list->types);
    //printf("types: [%p]\n", types);
    //send to expander, rever $? e $ENV~xpto
    lets_expander(types, env_list);
    printf("\n----\nprint the types list:\n");
    t_types *temp = types;
    while (temp)
    {
        printf("cms: [%s] - types: [%u]\n", temp->cmd, temp->type);
        temp = temp->next;
    }
    //printf("\n----\npre send to exec\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("types: [%p]\n", types);
    //printf("env_list: [%p]\n", env_list);
    //enviar para execução
    to_exec(input_list, types, env_list);
	
    /*printf("\n----\nconvertion of list to char**:\n");
    int j;
    for (j = 0; args[j]; j++)
        printf("args[%i]: [%s]\n", j, args[j]);
    */
    //args_list = parser(prompt); //a ideia aqui é interpretar cada strg a partir do split de pipe
    //será preciso splitar por estaço e tokenizar os comandos.

}