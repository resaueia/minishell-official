/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:50:29 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/04 23:26:51 by jparnahy         ###   ########.fr       */
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

void    to_exec(t_init_input *input_list, t_envp *env_list)
{
    (void) input_list;
    char    **env;

    env = env_to_char(env_list);

    for (int i = 0; env[i]; i++)
        printf("[%s]\n", env[i]);

    while (input_list)
    {
        printf("input_list->string: [%s] - token: [%u]\n", input_list->string, input_list->token);

        if (input_list->token == 44) //heredoc
        {
            //executa heredoc
            printf("has heredoc\n");
            //tackle_heredoc(cmd_list);
        }
        else if (input_list->token == 11) //pipe
        {
            //executa em cenário de pipe
            printf("has pipe\n");
            //args_list = split_commands(cmds, &head, &tail);
            //printf("\n----\nprint the args_list:\n");
            //print_the_stack(args_list);
        }
        else if (input_list->token == 04 || input_list->token == 07 || input_list->token == 77) //redirects
        {
            //executa redirect
            printf("has redirect\n");
        }
        else if (input_list->token == 03) //exec
        {
            //executa o comando
            printf("has exec\n");
        }
        else //builtin
            execute_builtin(input_list->string, env_list, input_list);
        input_list = input_list->next;
    }
    free_list(input_list);
}