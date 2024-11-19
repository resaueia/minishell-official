/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:50:29 by jparnahy          #+#    #+#             */
/*   Updated: 2024/11/19 16:23:56 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_builtin(char *args, t_envp *env_list, t_init_input *list)
{
    int saved_stdout = dup(STDOUT_FILENO);

    if (list->fd_out != STDOUT_FILENO) {
        dup2(list->fd_out, STDOUT_FILENO);
        close(list->fd_out);
    }

    if (ft_strcmp(args, "print") == 0) {
        print_stack(list);
    } else if (ft_strcmp(args, "env") == 0 || ft_strcmp(args, "envp") == 0) {
        print_envp_list(env_list);
    } else if (ft_strcmp(args, "pwd") == 0) {
        ft_pwd(list->fd_out);
    } else if (ft_strncmp(args, "echo", 4) == 0) {
        ft_echo(args + 4, &env_list, list->fd_out);
    } else if (ft_strncmp(args, "cd", 2) == 0) {
        ft_cd(args + 2, &env_list);
    } else if (ft_strncmp(args, "export", 6) == 0) {
        ft_export(args + 6, &env_list);
    } else if (ft_strncmp(args, "unset", 5) == 0) {
        ft_unset(args + 5, &env_list);
    }

    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
}




void find_command_path(t_types *type, t_envp *env_list) 
{
    (void)env_list;
    char *path = getenv("PATH"); // Obtém o PATH do sistema
    char *path_dup;
    char *dir;
    char *full_path;

    if (!path)
    {
        fprintf(stderr, "minishell: PATH not found\n");
        return ;
    }

    path_dup = strdup(path); //faz uma cópia do path para manipulação
    if (!path_dup)
    {
        perror("Erro ao duplicar PATH");
        exit(EXIT_FAILURE);
    }

    dir = strtok(path_dup, ":");
    while (dir != NULL) 
    {
        full_path = malloc(strlen(dir) + strlen(type->cmd) + 2); // +2 para '/' e '\0'
        if (!full_path) 
        {
            perror("Erro ao alocar memória para full_path");
            free(path_dup);
            exit(EXIT_FAILURE);
        }
        sprintf(full_path, "%s/%s", dir, type->cmd);
        if (access(full_path, X_OK) == 0) // Verifica se o caminho é executável
        { 
            free(type->cmd);        // Libera o conteúdo anterior de `type->cmd`
            type->cmd = full_path;  // Atualiza `type->cmd` com o novo caminho completo
            free(path_dup);         // Libera a cópia do PATH
            return;                 // Sai da função após encontrar o comando
        }
        free(full_path); // Libera full_path se não for válido
        dir = strtok(NULL, ":");
    }
    // Caso o comando não seja encontrado, imprime uma mensagem de erro
    fprintf(stderr, "minishell: comando não encontrado: %s\n", type->cmd);
    free(path_dup); // Libera a cópia do PATH
}

void exec_command(t_init_input *cmd, char **envp)
{
    if (execve(cmd->args[0], cmd->args, envp) == -1)
    {
        perror("Error executing command");
        exit(EXIT_FAILURE);
    }
}


void exec_cmd(t_init_input *cmd_list, t_types *type, char **envp) {
    if (!type || !type->cmd) {
        fprintf(stderr, "Command not found\n");
        exit(127);
    }

    char **args = list_to_char(cmd_list);

    if (execve(type->cmd, args, envp) == -1) {
        perror("execve failed");
        exit(errno);
    }
}



int to_exec(t_init_input *input_list, t_types *types, char *prompt, t_envp *env_list)
{
    process_input(input_list, types, prompt, env_list);
    return (0);
}

