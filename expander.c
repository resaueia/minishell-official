/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 23:43:15 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/12 17:14:44 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *expander_or_not(char *cmd, t_envp *env_list)
{
    char    *temp;
    
    temp = ft_strdup(cmd);
    if (temp[0] == '\"') //se começar com aspas duplas e estiver fechada, remover e continuar para expandir
        remove_quotes(&temp);
    else if (temp[0] == '\'') //se começar com aspas simples e estiver fechada, remover aspas apenas e não expandir
    {
        remove_quotes(&temp);
        cmd = ft_strdup(temp);
        temp = free_char_ptr(temp);
        return (cmd);
    }
    if (temp[0] == '$') //se começar com $, verificar a str completa
    {
        temp++; //incrementing the pointer to the next character for check next conditions
		if (*temp == '\0') //if cmd come with $ and no args, it will print just a char '$'
			temp = cmd; //changing the pointer to the char '$'
		else //if cmd come with $ and args, need check what kind of args it is
		{
            if ((ft_isalpha(temp[0]) == 0) && ft_strncmp(temp, "_", 1)) //if echo come with $ and args in lower case, it will print just a newline
			{
                if(ft_strncmp(temp, "\'", 1) == 0 || ft_strncmp(temp, "\"", 1) == 0)
                    remove_quotes(&temp);
                else if (temp[0] == '0')
                    temp = ft_strjoin("bash", temp);
                else if (temp[0] == '-')
                    temp = ft_strjoin("himBHs", temp + 1);
                else if (ft_isdigit(temp[0]) || is_special(temp[0]))
                    temp++;
			}
			else if (ft_isalpha(temp[0]) || (ft_strncmp(temp, "_", 1) == 0)) //if echo come with $ and args in upper case, it will check if is a key of env list
			{
				if (is_key(temp, env_list) == 1) //if is a key, it will get the value of the key
                {
                    //se a key estiver seguida por um caractere especial, expandir e concatenar com o restante da string, contando com o caractere especial
					temp = get_value(temp, env_list); //changing the pointer to the value of the key
                }
				else //if is not a key, it will print just a newline
                    temp = ft_strdup("\n");
			}
		}
    }
    cmd = ft_strdup(temp);
    //temp = free_char_ptr(temp);
    return (cmd);
}

void	lets_expander(t_types *types, t_envp *env_list)
{
    t_envp  *env;

    env = env_list;
    while (types)
    {
        types->cmd = expander_or_not(types->cmd, env);
        types = types->next;
    }
}
