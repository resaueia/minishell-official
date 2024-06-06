/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:56:42 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/06/06 19:33:09 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_strlen(char *str)
{
	int i;

	i = 0;
	while(str[i])
		i++;
	return (i);
}

void handle_signals(int signo) {
    if (signo == SIGINT) {
        printf("\nminishell> ");
        fflush(stdout);
    }
}

// Handle signals (for example, Ctrl-C)
void handle_signals(int signo) {
    if (signo == SIGINT) {
        printf("\nminishell> ");
        fflush(stdout);
    }
}

// Execute commands (placeholder for now)
void execute_command(char **args) {
    printf("Executing command: ");
    for (int i = 0; args[i] != NULL; i++) {
        printf("%s ", args[i]);
    }
    printf("\n");
}

// Prompt function
void prompt() {
    char *input;
    const char *delimiters = "|<>";

    while ((input = readline("megatron> ")) != NULL) {
        if (strlen(input) > 0) {
            add_history(input);
            int num_commands = 0;
            char ***commands = ft_split(input, delimiters, &num_commands);
            for (int i = 0; i < num_commands; i++) {
                execute_command(commands[i]);
                // Free each command's tokens
                for (int j = 0; commands[i][j] != NULL; j++) {
                    free(commands[i][j]);
                }
                free(commands[i]);
            }
            free(commands);
        }
        free(input);
    }
}

int main() {
    signal(SIGINT, handle_signals);
    signal(SIGQUIT, SIG_IGN);

    prompt();
	return (0);
}
   

