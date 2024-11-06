/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 23:43:15 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/06 00:49:03 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lets_expander(t_types *types, t_envp *env_list)
{
    t_envp  *env;
    char    *value;

    env = env_list;

    //incluir verificação das aspas, para remoção

    while (types)
    {
        if (ft_strchr(types->cmd, '$'))
        {
            value = ft_strdup(types->cmd);
            types->cmd = ft_strdup(get_value(&value[1], env));
            if (types->cmd == NULL)
                types->cmd = ft_strdup(value);
            else
                value = free_char_ptr(value);
            //printf("after checkers => types->cmd: [%s]\n", types->cmd);
        }
        types = types->next;
    }
}
