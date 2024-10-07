/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/10/07 18:58:44 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
void	execute_command(char *cmd, t_envp *env_list, t_init_input *list)
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
void	prompt(char **envp)
{
	char			*prompt;
	char			*prompt_dup;
	t_init_input	*input_list;
	t_envp			*env_list;

	env_list = get_envp(envp); 	// get the envp list
	// for signal handling
	signal(SIGINT, handle_signals); // SIGINT is the signal sent by pressing Ctrl+C
	signal(SIGQUIT, SIG_IGN); // SIGQUIT is the signal sent by pressing Ctrl+D.
	while (1) // loop the shell.
	{
		prompt = readline(PROGRAM_NAME); // the prompt
		if (add_to_history(prompt)) // add the prompt to the history and go on
		{
			prompt_dup = ft_strdup(prompt);
			input_list = ft_split(prompt_dup);
		}
		if (ft_strcmp(prompt, "exit") == 0) //if the user types exit, the shell will exit.
		{
			free(prompt); //free the prompt
			exit(1); //exit the shell with error code 1
		}
		else
			execute_command(prompt, env_list, input_list); // execute the command line
		free(prompt); //free(input_list);
	}
}
