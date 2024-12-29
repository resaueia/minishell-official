/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:35:37 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/28 22:02:05 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_ctrl_d_sig(t_init_input *input_list, t_envp *env_list)
{
	printf("exit\n");
	rl_clear_history();
	free_env(env_list);
	free_list(input_list);
	last_status(0);
	exit(0);
}

static int	parse_exit_status(char *prompt, int i)
{
	int	ret;

	ret = 0;
	while (prompt[i] == ' ')
		i++;
	while (prompt[i])
	{
		if (is_whitspace(prompt[i]))
		{
			printf("exit\nminishell: exit: too many arguments\n");
			return (-1);
		}
		if (!ft_isdigit(prompt[i]))
		{
			printf("exit\nminishell: exit: %s: numeric argument required\n",
				prompt);
			exit(255);
		}
		ret = ret * 10 + prompt[i] - '0';
		i++;
	}
	return (ret);
}

void	exit_shell(char *prompt, char *prompt_dup, t_init_input *input_list,
		t_envp *env_list)
{
	int	ret;
	int	i;

	i = 0;
	if (!prompt)
	{
		handle_ctrl_d_sig(input_list, env_list);
		return ;
	}
	while (prompt[i] == ' ')
		i++;
	ret = parse_exit_status(prompt, 4);
	if (ret == -1)
	{
		exit_mini(input_list, prompt, prompt_dup, env_list);
		exit(1);
	}
	exit_mini(input_list, prompt, prompt_dup, env_list);
	exit(ret);
}
