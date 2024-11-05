/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:02:07 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/04 23:22:56 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    process_input(t_init_input *input_list, char *prompt, t_envp *env_list)
{
    //t_init_input    *args_list;
    //t_init_input    *head;
    //t_init_input    *tail;
    //char            **args;
    char            **cmds;

    cmds = lexer(prompt); // split the input for space and quotes
    input_list = delim_split(prompt); // split the input for pipe
    //args = list_to_char(input_list); // convert the list to char**

    //printf("\n----\nprint the input_list:\n");
    //print_the_stack(input_list);

    (void) env_list;
    printf("\n----\ncmds from lexer:\n");
    int i;
    for (i = 0; cmds[i]; i++)
        printf("args[%i]: [%s]\n", i, cmds[i]);

    if (!cmds)
    {
        ft_putstr_fd("Error: unclosed quotes\n", 2);
        cmds = free_from_split(cmds);
        return ;
    }
    
    to_exec(input_list, env_list);
	
    /*printf("\n----\nconvertion of list to char**:\n");
    int j;
    for (j = 0; args[j]; j++)
        printf("args[%i]: [%s]\n", j, args[j]);
    */
    //args_list = parser(prompt); //a ideia aqui é interpretar cada strg a partir do split de pipe
    //será preciso splitar por estaço e tokenizar os comandos.

}