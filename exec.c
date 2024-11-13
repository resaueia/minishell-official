/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:50:29 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/13 18:21:27 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_builtin(char *cmd, t_envp *env_list, t_init_input *list, t_types *types)
{
    (void)cmd;
	t_envp	*tmp;

	tmp = env_list;
	if (ft_strcmp(types->cmd, "print") == 0)
		print_stack(list);
	else if (ft_strcmp(types->cmd, "env") == 0 || ft_strcmp(cmd, "envp") == 0)
		print_envp_list(tmp);
	else if (ft_strcmp(types->cmd, "pwd") == 0)
		ft_pwd();
	else if (ft_strncmp(types->cmd, "echo", 4) == 0)
		ft_echo(types->next->cmd, &tmp);
	else if (ft_strncmp(types->cmd, "cd", 2) == 0)
		ft_cd(types->next->cmd, &tmp);
	else if (ft_strncmp(types->cmd, "export", 6) == 0)
		ft_export(types->next->cmd, &tmp);
	else if (ft_strncmp(types->cmd, "unset", 5) == 0)
		ft_unset(types->next->cmd, &tmp);
}

static void find_command_path(char *cmd, t_envp *env_list)
{
    //preciso buscar o path do comando na env_list e substitur pelo executavel
    t_envp	*tmp;
    char	*path;
    char	*cmd_path;

    tmp = env_list;
    cmd_path = cmd;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, "PATH") == 0)
        {
            path = ft_strdup(tmp->value);
            break ;
        }
        tmp = tmp->next;
    }
    cmd_path = ft_strjoin(path, cmd);
    free(path);
    free(cmd_path);
}

/*void	exec_cmd(char **args, char **env)
{
    char    **args;
    pid_t	pid;
    int		status;

    args = list_to_char(type);
    pid = fork();
    if (pid == 0)
    {
        if (execve(args[0], args, env) == -1)
        {
            ft_putstr_fd("Error: command not found\n", 2);
            exit(1);
        }
    }
    else if (pid < 0)
    {
        ft_putstr_fd("Error: fork failed\n", 2);
        exit(1);
    }
    else
    {
        waitpid(pid, &status, 0);
    }
}*/

void    to_exec(t_init_input *input_list, t_types *type, t_envp *env_list)
{
    //printf("\n----\nto_exec\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("env_list: [%p]\n", env_list);
    //printf("type: [%p]\n", type);
    //printf("input_list->types: [%p]\n", input_list->types);
    (void) input_list;
    char    **env;
    t_types *tmp;
    
    //printf("\n----\nafter declarations\n");
    env = env_to_char(env_list);
    tmp = type;
    (void) env;
    (void) type;
    (void) tmp;

    //printf("\n----\nafter insert values on vars\n");
    //printf("env: [%p]\n", env);
    //printf("tmp: [%p]\n", tmp);
    /*if (!tmp)
        printf("type is NULL\n");
    else
        printf("type is not NULL\n");
    while (tmp)
    {
        printf("cms: [%s] - types: [%u]\n", tmp->cmd, tmp->type);
        tmp = tmp->next;
    }*/
    if (is_hdoc(type)) //heredoc
    {
        //executa heredoc
        printf("has heredoc\n");
        //tackle_heredoc(cmd_list);
    }
    else if (is_pp(type)) //pipe
    {
        //executa em cenário de pipe
        printf("has pipe\n");
        //args_list = split_commands(cmds, &head, &tail);
        //printf("\n----\nprint the args_list:\n");
        //print_the_stack(args_list);
    }
    else if (is_rdrct(type)) //redirects
    {
        //executa redirect
        printf("has redirect\n");
    }
    else if (is_btin(type)) //builtin
    {
        execute_builtin(type->cmd, env_list, input_list, type);
        //executa o comando
    }
    else if (is_exec(type)) //execve
    {
        printf("has execve\n");
        //procura o path do comando na env_list
        find_command_path(type->cmd, env_list);
        printf("cmd_path: [%s]\n", type->cmd);
        //executa execve
        //exec_cmd(input_list->fd_in, input_list->fd_out, type, env);
    }
    //free(env);
}