/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/10/28 21:59:51 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_init_input	*init_list(void)
{
	t_init_input	*list;

	list = (t_init_input *)malloc(sizeof(t_init_input));
	if (!list)
		return (NULL);
	list->string = NULL;
	list->args = NULL;
	list->fd_in = dup(STDIN);
	list->fd_out = dup(STDOUT);
	list->token = (t_token){0};
	list->prev = NULL;
	list->next = NULL;
	return (list);
}

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

char    **process_input(t_init_input *cmd_list, char **cmds, t_envp *env_list)
{
	(void) env_list;
    t_init_input    *args_list;
    t_init_input    *args_tail;
    //char            *args;  Ideia que o allan deu de guardar o input inteiro
    
    args_list = NULL;
    args_tail = NULL;
    split_commands(cmds, &args_list, &args_tail); 
	printf("\n--> args_list:\n");
    print_the_stack(args_list);

    printf("\n--> cmd_list:\n");
    print_the_stack(cmd_list);
			
	//para enviar para execução
	/*if (is_heredoc(cmd_list)) //has heredoc
	{
		//executa heredoc
		tackle_heredoc(t_init_input *input_list);
	}
	else if (is_pipe(cmds_list)) //has pipe
	{
		//executa em cenário de pipe
	}
    else if (is_redirect(cmd_list))//has redirect
	{
		//executa redirect
	}
	else if (is_builtin(cmd_list))//has built-in
	{
		// execute the command line
		execute_builtin(cmds[0], env_list, cmd_list);
	}
	*/
	execute_builtin(cmds[0], env_list, cmd_list);
    free_list(cmd_list);
    free_list(args_list);
    return (cmds);
    // free the cmds array, free the cmds list and free the args list?
}

void	prompt(char **envp)
{
	char			*prompt;
	char			*prompt_dup;
	char            **cmds;
	char			**process;
	t_init_input	*input_list;
	t_envp			*env_list;

	env_list = get_envp(envp); 	// get the envp list
	input_list = init_list(); // initialize the input list
	while (1) // loop the shell.
	{
		signal(SIGINT, handle_signals); // SIGINT is the signal sent by pressing Ctrl+C
		signal(SIGQUIT, SIG_IGN); // SIGQUIT is the signal sent by pressing Ctrl+D.
		prompt = readline(PROGRAM_NAME); // the prompt
		if (add_to_history(prompt)) // add the prompt to the history and go on
			prompt_dup = ft_strdup(prompt);
		if (ft_strcmp(prompt, "exit") == 0) //if the user types exit, the shell will exit.
			exit_mini(input_list, prompt, prompt_dup, env_list); // exit the shell end clear the memory
		else
		{
			if (!input_check(prompt)) // check if the input is valid
			{
				input_list = delim_split(prompt_dup); // split the input into a linked list
				cmds = list_to_char(input_list);
				
				printf("\n----\nstruct after split:\n");
				print_the_stack(input_list);
				
				printf("\n----\nconvertion of struct to char**:\n");
				int i;
				for (i = 0; cmds[i]; i++)
					printf("cmds[%i]: %s\n", i, cmds[i]);

				printf("\n----\nsending to process_input:\n");

				process = process_input(input_list, cmds, env_list);

				printf("\n----\nretorn of process: [%s]\n", process[0]);
			}
			else
			{
				printf("minishell: syntax error\n"); // if the input is invalid, print an error message
				continue;
			}
		}
		free(prompt); // free the prompt
	}
}
