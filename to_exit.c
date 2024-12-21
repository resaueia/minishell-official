/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:44:40 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/21 18:45:11 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Function: exit_mini
 * Frees all dynamically allocated resources used during the program's
 * execution, including input lists, prompts, and environment variables.
 */

void	exit_mini(t_init_input *list, char *prompt, char *prompt_dup,
		t_envp *env_list)
{
	if (list)
		free_list(list);
	if (prompt)
		free_char_ptr(prompt);
	if (prompt_dup)
		free_char_ptr(prompt_dup);
	if (env_list)
		free_env(env_list);
}
