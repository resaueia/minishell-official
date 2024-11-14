/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/13 22:06:23 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_the_stack(t_init_input *list)
{
	t_init_input *current = list;

	// Traverse and print the linked list
	while (current != NULL)
	{
		printf("string: [%s] - token [%u]\n", current->string, current->token);
		current = current->next;
	}
}

int	add_to_history(char *line)
{
	int i;

	i = -1;
	if (!line)
		return (0);
	while (line[++i])
	{
		if (!ft_is_whitspace(line[i]))
		{
			add_history(line);
			return (1);
		}
	}
	return (0);
}

void	execute_builtin(char *args, t_envp *env_list, t_init_input *list)
{
	int saved_stdout;
    //int saved_stdin;
	
	saved_stdout = dup(STDOUT_FILENO);
	//saved_stdin = dup(STDIN_FILENO);
    if (list->fd_out != STDOUT_FILENO)
    {
		if (dup2(list->fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2 error encountered in builtin");
			close(saved_stdout);
			return ;
		}
        close(list->fd_out);
    }
    /*if (list->fd_in != STDIN_FILENO)
    {
        dup2(list->fd_in, STDIN_FILENO);
        close(list->fd_in);
    }*/
	if (ft_strcmp(args, "print") == 0)
		print_stack(list);
	else if (ft_strcmp(args, "env") == 0 || ft_strcmp(args, "envp") == 0)
		print_envp_list(env_list);
	else if (ft_strcmp(args, "pwd") == 0)
		ft_pwd(STDOUT_FILENO);
	else if (ft_strncmp(args, "echo", 4) == 0)
		ft_echo(args + 4, &env_list, STDOUT_FILENO);
	else if (ft_strncmp(args, "cd", 2) == 0)
		ft_cd(args + 2, &env_list);
	else if (ft_strncmp(args, "export", 6) == 0)
		ft_export(args + 6, &env_list);
	else if (ft_strncmp(args, "unset", 5) == 0)
		ft_unset(args + 5, &env_list);
	dup2(saved_stdout, STDOUT_FILENO);
	//dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	//close(saved_stdin);
}



void	prompt(char **envp)
{
	char			*prompt;
	char			*prompt_dup;
	char            **cmds;
	t_init_input	*input_list;
	t_envp			*env_list;

	env_list = get_envp(envp); 	// get the envp list
	input_list = init_list();
	while (1) // loop the shell.
	{
		//g_signal_received = 0;
		prompt = readline(PROGRAM_NAME); // the prompt
		if (!prompt)
		{
			printf("exit\n");
			break;
		}
		if (add_to_history(prompt)) // add the prompt to the history and go on
		{
			prompt_dup = ft_strdup(prompt);
		}
		if (ft_strcmp(prompt, "exit") == 0) //if the user types exit, the shell will exit.
		{
			free(prompt); //free the prompt
			exit(1); //exit the shell with error code 1
		}
		else
		{
			if (!input_check(prompt)) // check if the input is valid
			{
				//initiliaze list (start/set variables)
				input_list = delim_split(prompt_dup); // split the input into a linked list
				
				cmds = list_to_char(input_list);
				process_input(prompt_dup, input_list, cmds, env_list);
				
				//print_the_stack(input_list);
				//execute_builtin(prompt, env_list, input_list); // execute the command line
			}
			else
			{
				printf("minishell: syntax error\n"); // if the input is invalid, print an error message
				continue;
			}
			
		}
		free(prompt); 
		//free(input_list);
	}
}
