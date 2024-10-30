/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:50:29 by jparnahy          #+#    #+#             */
/*   Updated: 2024/10/29 22:30:09 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_builtin(char *cmd, t_envp *env_list, t_init_input *list)
{
	t_envp	*tmp;

	tmp = env_list;
	if (ft_strcmp(cmd, "print") == 0)
		print_stack(list);
	else if (ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "envp") == 0)
		print_envp_list(tmp);
	else if (ft_strcmp(cmd, "pwd") == 0)
		ft_pwd();
	else if (ft_strncmp(cmd, "echo", 4) == 0)
		ft_echo(cmd + 4, &tmp);
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		ft_cd(cmd + 2, &tmp);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		ft_export(cmd + 6, &tmp);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		ft_unset(cmd + 5, &tmp);
}

void    process_input(t_init_input *input_list, char *prompt, t_envp *env_list)
{
    t_init_input            *args_list;
    char                    **cmds;

    input_list = delim_split(prompt); // split the input for pipe
    cmds = list_to_char(input_list); 

	(void) env_list;
    printf("\n----\nconvertion of list to char**:\n");
    int i;
    for (i = 0; cmds[i]; i++)
        printf("cmds[%i]: %s\n", i, cmds[i]);
    
    printf("\n----\nprint the input_list:\n");
    print_the_stack(input_list);

    args_list = parser(input_list, cmds); //a ideia aqui é interpretar cada strg a partir do split de pipe
    //será preciso splitar por estaço e tokenizar os comandos.
    printf("\n----\nprint the args_list:\n");
    print_the_stack(args_list);

    while (input_list)
    {
        printf("input_list->string: [%s] - token: [%u]\n", input_list->string, input_list->token);

        if (input_list->token == 4)
        {
            //executa heredoc
            printf("has heredoc\n");
            //tackle_heredoc(cmd_list);
        }
        else if (input_list->token == 0)
        {
            //executa em cenário de pipe
            printf("has pipe\n");
        }
        else if (input_list->token == IN || input_list->token == OUT || input_list->token == APPEND)
        {
            //executa redirect
            printf("has redirect\n");
        }
        else if (input_list->token == 7)
        {

            execute_builtin(input_list->string, env_list, input_list);
        }
        else if (input_list->token == 8)
        {
            //executa o comando
            printf("has exec\n");
        }
        input_list = input_list->next;
    }
    free_list(input_list);
}