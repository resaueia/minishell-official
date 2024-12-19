/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:28:38 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/19 12:04:42 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int is_delim(int type)
{
    if (type == PIPE || type == IN || type == OUT || type == APPEND || type == HDOC)
        return (1);
    return (0);
}

static int  is_redirects(int type)
{
    if (type == IN || type == OUT || type == APPEND || type == HDOC)
        return (1);
    return (0);
}

static void handle_redirect(t_types *cmd)
{
    if (!cmd->next || !cmd->next->cmd)
        return; // Evita acessar ponteiros inválidos
    if ((ft_strcmp(cmd->cmd, ">") == 0 || ft_strcmp(cmd->cmd, "<") == 0))
        cmd->next->type = FLE;
    else if (ft_strcmp(cmd->cmd, "<<") == 0)
        cmd->next->type = ENDOF;
}


static void handle_builtin_first(t_types **cmd, int *node_ref)
{
    if (ft_strcmp((*cmd)->cmd, "echo") == 0)
    {
        if (!(*cmd)->next)
            return;
        *cmd = (*cmd)->next;
        while (*cmd && !is_delim((*cmd)->type))
        {
            (*cmd)->type = ARGS;
            *cmd = (*cmd)->next;
        }
        *node_ref = 0;
    }
}

static void handle_builtin_non_first(t_types **cmd)
{
    if (ft_strcmp((*cmd)->cmd, "echo") == 0 && (*cmd)->next)
    {
        *cmd = (*cmd)->next;
        while (*cmd && !is_delim((*cmd)->type))
        {
            (*cmd)->type = ARGS;
            *cmd = (*cmd)->next;
        }
    }
}

static void process_first_node(t_types **cmd, int *node_ref)
{
    if (is_redirects((*cmd)->type))
        handle_redirect(*cmd);
    else if (is_builtin((*cmd)->cmd))
        handle_builtin_first(cmd, node_ref);
    else if (ft_strcmp((*cmd)->cmd, "Makefile") == 0)
        (*cmd)->type = FLE;
    else
        (*cmd)->type = EXEC;
    *node_ref = 0;
}

static void process_non_first_node(t_types **cmd, int *node_ref)
{
    if ((*cmd)->type == PIPE)
    {
        *node_ref = 1;
        return;
    }
    if (is_redirects((*cmd)->type))
        handle_redirect(*cmd);
    else if (is_builtin((*cmd)->cmd))
        handle_builtin_non_first(cmd);
    else if (ft_strcmp((*cmd)->cmd, "Makefile") == 0)
        (*cmd)->type = FLE;
}

void args_of_cmds(t_types *cmd)
{
    t_types *head;
    int     node_ref;

    head = cmd; // Salva o ponteiro inicial
    node_ref = 1;
    while (cmd)
    {
        if (node_ref == 1)
        {
            if (cmd->next)
                process_first_node(&cmd, &node_ref);
        }
        else
        {
            if (cmd->next)
                process_non_first_node(&cmd, &node_ref);
            if (cmd && !is_delim(cmd->type) && cmd->prev && is_redirects(cmd->prev->type))// Verifica se o nó atual precisa ser marcado como FLE
                cmd->type = FLE;
        }
        if (cmd && !cmd->next && node_ref == 0 && !is_delim(cmd->type)) // Marca o último argumento de um comando como FLE (exemplo: cat Makefile)
            cmd->type = FLE;
        cmd = cmd->next; // Move para o próximo nó
    }
    cmd = head; // Restaura o ponteiro inicial (se necessário)
}


/*void args_of_cmds(t_types *cmd)
{
    t_types *head;
    int     node_ref;

    head = cmd;
    node_ref = 1;
    while (cmd)
    {
        if (node_ref == 1)
        {
            if (is_redirects(cmd->type))
            {
                if ((ft_strcmp(cmd->cmd, ">") == 0 || ft_strcmp(cmd->cmd, "<") == 0) && cmd->next->cmd)
                    cmd->next->type = FLE;
                else if (ft_strcmp(cmd->cmd, "<<") == 0 && cmd->next->cmd)
                    cmd->next->type = ENDOF;
            }
            else if (is_builtin(cmd->cmd) == 1)
            {
                if (ft_strcmp(cmd->cmd, "echo") == 0 && !cmd->next)
                    break;
                if (ft_strcmp(cmd->cmd, "echo") == 0 && cmd->next->cmd)
                {
                    cmd = cmd->next;
                    while (cmd && is_delim(cmd->type) == 0)
                    {
                        cmd->type = ARGS;
                        cmd = cmd->next;
                    }
                    if (!cmd)
                        break;
                }
            }
            else
                cmd->type = EXEC;
            node_ref = 0;
        }
        if (cmd->type == PIPE)
            node_ref = 1;
        if (node_ref == 0)
        {
            if (is_redirects(cmd->type))
            {
                if ((ft_strcmp(cmd->cmd, ">") == 0 || ft_strcmp(cmd->cmd, "<") == 0) && cmd->next->cmd)
                    cmd->next->type = FLE;
                else if (ft_strcmp(cmd->cmd, "<<") == 0 && cmd->next->cmd)
                    cmd->next->type = ENDOF;
            }
            else if (is_builtin(cmd->cmd) == 1)
            {
                if (ft_strcmp(cmd->cmd, "echo") == 0 && cmd->next->cmd)
                {
                    cmd = cmd->next;
                    while (cmd && is_delim(cmd->type) == 0)
                    {
                        cmd->type = ARGS;
                        cmd = cmd->next;
                    }
                    if (!cmd)
                        break;
                }
            }
            else if (ft_strcmp(cmd->cmd, "Makefile") == 0)
                cmd->type = FLE;
        }
        cmd = cmd->next;
    }
    cmd = head;
}*/
