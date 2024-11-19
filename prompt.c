/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:03 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/19 16:14:13 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_signals(int sig, siginfo_t *info, void *context)
{
	(void) context;
	printf("\n");
	if (sig == SIGINT && info->si_pid)
	{
		rl_on_new_line ();
		rl_replace_line ("", 0);
		rl_redisplay ();
	}
}

/*static t_types	*init_types(void)
{
	t_types	*types;

	types = (t_types *)malloc(sizeof(t_types));
	if (!types)
		return (NULL);
	types->cmd = NULL;
	types->type = 0;
	types->prev = NULL;
	types->next = NULL;
	return (types);
}*/

static t_init_input	*init_list(void)
{
	t_init_input	*list;
	struct	sigaction	act;

	ft_bzero(&act, sizeof(act));
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handle_signals;
	sigaction(SIGINT, &act, NULL);
	signal(SIGQUIT, SIG_IGN);
	list = (t_init_input *)malloc(sizeof(t_init_input));
	if (!list)
		return (NULL);
	list->string = NULL;
	list->args = NULL;
	list->fd_in = STDIN_FILENO;
	list->fd_out = STDOUT_FILENO;
	list->token = (t_token){0};
	list->types = NULL;
	list->prev = NULL;
	list->next = NULL;
	return (list);
}

int	add_to_history(char *line)
{
	int i;

	i = 0;
	if (!line)
		return (0);
	while (line[i] != '\0')
	{
		if (!is_whitspace(line[i]))
		{
			add_history(line);
			return (1);
		}
		i++;
	}
	return (0);
}

void prompt(char **envp)
{
    char *prompt;
    t_init_input *input_list;
    t_envp *env_list;

    env_list = get_envp(envp);

    while (1) {
        prompt = readline("minishell> ");
        if (!prompt) {
            printf("exit\n");
            break;
        }

        if (ft_strcmp(prompt, "exit") == 0) {
            free(prompt);
            break;
        }

        // Inicializa a lista de entrada
        input_list = init_list();
        if (!input_list) {
            perror("Failed to initialize input list");
            free(prompt);
            continue;
        }

        // Processa o input apenas se a lista foi inicializada
        input_list = delim_split(prompt); // Popula a lista com o input do usuário
        if (input_list) {
            process_input(input_list, NULL, prompt, env_list);
            free_list(input_list); // Libera a lista ao final
        }
        free(prompt);
    }
    free_env(env_list);
}
