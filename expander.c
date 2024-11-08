/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 23:43:15 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/08 18:23:33 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
static void    print_types_node(t_types *types)
{
    t_types *temp;

    if (!types)
    {
        printf("types is NULL\n");
        return ;
    }
    temp = types;
    printf("cmd: [%s] | type: [%i]\n", temp->cmd, temp->type);
}*/
/*
static void    print_types(t_types *types)
{
    t_types *temp;

    temp = types;
    printf("\n----\nprint_types\n");
    while (temp)
    {
        printf("L36 | no while\n");
        printf("cmd: [%s] | type: [%i]\n", temp->cmd, temp->type);
        
        printf("ptr types: [%p]\n", types);
        printf("ptr types: [%p]\n", types->prev);
        printf("ptr types: [%p]\n", types->next);
        
        printf("\n----\ntypes  P R E V\n");
        print_types_node(types->prev);
        
        printf("\n----\ntypes  N E X T\n");
        print_types_node(types->next);
        
        printf("\n----\n");
        temp = temp->next;
    }
}*/

static char *expander_or_not(char *cmd)
{
    char *temp;
    
    
        //se for seguido por aspas (duplas ou simples), remover o $ e não expandir
        //se for seguido por um caractere alfabético, expandir ou concatenar
        //se não for seguido de caracteres alfabéticos, não expandir
    if (cmd[0] == '\"') //se começar com aspas duplas e estiver fechada, remover e continuar para expandir
    {
        printf("cmd: [%s]\n", cmd);
        temp = remove_quotes(&cmd);
        //cmd = ft_strndup(&temp[1], ft_strlen(temp) - 2);
        //temp = free_char_ptr(temp);
        printf("tmp: [%s]\n", tmp);
    }
    else if (cmd[i] == '\'') //se começar com aspas simples e estiver fechada, remover aspas apenas e não expandir
    {
        printf("cmd: [%s]\n", cmd);
        temp = remove_quotes(&cmd);
        //cmd = ft_strndup(&temp[1], ft_strlen(temp) - 2);
        //temp = free_char_ptr(temp);
        printf("tmp: [%s]\n", tmp);
    }
    else if (cmd[i] == '$') //se começar com $, verificar a str completa
    {
        cmd++; //incrementing the pointer to the next character for check next conditions
		if (*cmd == '\0') //if echo come with $ and no args, it will print just a char '$'
			cmd = &dollar; //changing the pointer to the char '$'
		else //if echo come with $ and args, need check what kind of args it is
		{
			if (ft_is_alpha(cmd) == 1) //if echo come with $ and args in lower case, it will print just a newline
			{
				printf("\n");
				return ;
			}
			else if (is_lower(cmd) == 0) //if echo come with $ and args in upper case, it will check if is a key of env list
			{
				if (is_key(cmd, *env_list) == 1) //if is a key, it will get the value of the key
					cmd = get_value(args, *env_list); //changing the pointer to the value of the key
				else //if is not a key, it will print just a newline
				{
					printf("\n");
					return ;
				}
			}
		}
    }
}


static char	*split_copy(char **mat)
{
	char	*str;
	char	*temp;
	int		i;

	str = ft_strdup(mat[0]);
	i = 0;
	while (mat[++i])
	{
		temp = str;
		str = ft_strjoin(temp, mat[i]);
		temp = free_char_ptr(temp);
	}
	mat = free_from_split(mat);
	return (str);
}

static char	*to_quotes_expand(char *cmd)
{
    printf("\n----\nto_quotes_expand\n");
    printf("cmd before: [%s]\n", cmd);
	int		i;
	int		quotes;
	char	**ret;

	i = -1;
	quotes = 0;
	while (cmd[++i])
	{
		quotes = to_quotes(cmd[i], quotes);
		if ((cmd[i] == '\'' || cmd[i] == '\"') && !quotes)
			cmd[i] = 29;
		else if ((cmd[i] == '\'' && quotes == 1) || \
		(cmd[i] == '\"' && quotes == 2))
			cmd[i] = 29;
	}
    printf("cmd afer: [%s]\n", cmd);
	ret = the_split(cmd, 29);
	if (!*ret)
	{
		ret = free_from_split(ret);
		return (ft_strdup(""));
	}
	return (split_copy(ret));
}

void	lets_expander(t_types *types, t_envp *env_list)
{
    printf("\n----\nlets_expander\n");

    t_envp  *env;
    char    *value;

    env = env_list;
    types->cmd = expander_or_not(types->cmd);
    while (types)
    {
        printf("cmd: [%s]\n", types->cmd);
        // adequar lógica abaixo com a de cima
        if (ft_strchr(types->cmd, '$'))
        {
            value = ft_strdup(types->cmd);
            types->cmd = ft_strdup(get_value(&value[1], env));
            if (types->cmd == NULL)
                types->cmd = ft_strdup(value);
            else
                value = free_char_ptr(value);
            printf("after $ checkers => types->cmd: [%s]\n", types->cmd);
        }
        if (!types->prev || (types->prev && types->prev->type != HDOC))
        {
            printf("cmd scd if: [%s]\n", types->cmd);
            value = ft_strdup(types->cmd);
			types->cmd = to_quotes_expand(types->cmd);
			value = free_char_ptr(value);
        }
        printf("\n---\n");
        types = types->next;
        quotes = 0;
    }
}
