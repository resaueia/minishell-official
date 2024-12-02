/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:55:24 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/02 15:17:02 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*identify heredoc:
if heredoc, get a function to process it. how?
start capturing all input lines from the user until you come across
the initial delimiter.
write said lines in a pipe - pipe needs to get redirectioned as an entry for the cmd;
finally, those lines will get written on the terminal once said cmd is executed.
*/

static void include_eof(t_types *cmd)
{
    //printf("\n----\non include_eof\n\n");
    //printf("cmd: [%p]\n", cmd);
    t_types *head;

    head = cmd;
    while (cmd)
    {
        if (ft_strcmp(cmd->cmd, "<<") == 0 && cmd->next->cmd)
        {
            cmd = cmd->next;
            cmd->type = ENDOF;
        }
        cmd = cmd->next;
    }
    cmd = head;
    /*t_types *temp = cmd;
    while (temp)
    {
        printf("cms: [%p]_[%s]_[%u]\n", temp->cmd, temp->cmd, temp->type);
        temp = temp->next;
    }*/
}

static char **split_heredoc(t_types *type, int here_count)
{
    printf("\n----\non split_heredoc\n\n");
    //printf("type: [%p]\n", type);
    printf("here_count: [%d]\n", here_count);
    t_types *head;
    char    **delim;
    int     i;

    (void)head;
    head = type;
    delim = (char **)malloc(sizeof(*delim) * (here_count + 1));
    i = 0;
    while (type)
    {
        if (ft_strcmp(type->cmd, "<<") == 0 && type->next->cmd)
        {
            delim[i] = ft_strdup(type->next->cmd);
            //printf("delim[%i]: [%s]\n", i, delim[i]);
            i++;
        }
        type = type->next;
    }
    delim[i] = NULL;
    i = 0;
    while (delim[i])
    {
        printf("delim[%i]: [%s]\n", i, delim[i]);
        i++;
    }
    return (delim);
}

static void change_value(t_types *type, char *value)
{
    //printf("\n----\non change_value\n\n");
    //printf("type: [%p]\n", type);
    //printf("value: [%s]\n", value);
    t_types *head;

    head = type;
    while (type)
    {
        if (ft_strcmp(type->cmd, "<<") == 0 && type->next->cmd)
        {
            free(type->cmd);
            type->cmd = ft_strdup(value);
            type->next->cmd = NULL;
            return ;
        }
        type = type->next;
    }
    type = head;
}

int     is_heredoc(t_init_input *input_list, t_types *type)
{
    //printf("\n----\non is_heredoc\n\n");
    //printf("input_list: [%p]\n", input_list);
    //printf("type: [%p]\n", type);
    t_types *head;
    char    **delim;
    int     heredoc_fd;
    int     here_count; //contador de << encontrados
    (void) input_list;
    (void) delim;

    head = type;
    here_count = 0;
    while (type) // loop de contagem de << encontrados
    {
        if (ft_strcmp(type->cmd, "<<") == 0 && type->next->cmd)
            here_count++;
        type = type->next;
    }
    printf("here_count: [%d]\n", here_count);

    type = head;
    if (here_count == 1)
    {
        printf("one heredoc found\n");
        include_eof(type);
        while(type)
        {
            //printf("cmd: [%s]\n", temp->cmd);
            //printf("delim: [%s]\n", temp->next->cmd);
            if (ft_strcmp(type->cmd, "<<") == 0 && type->next->cmd)
            {
                heredoc_fd = tackle_heredoc(head, NULL, type->next->cmd);
                if (heredoc_fd == -1)
                {
                    type = head;
                    perror ("Error setting up heredoc");
                    return (-1);
                }
                type->fd[0] = heredoc_fd; // Configura o fd_in
                type = head;
                return (1);
            }
            type = type->next;
        }
    }
    else if (here_count > 1)
    {
        printf("multiple heredoc found\n");
        include_eof(type);
        delim = split_heredoc(type, here_count);
        while(type)
        {
            //printf("cmd: [%s]\n", temp->cmd);
            //printf("delim: [%s]\n", temp->next->cmd);
            if (ft_strcmp(type->cmd, "<<") == 0 && type->next->cmd)
            {
                printf("heredoc found\n");
                printf("start_delim: [%s]\n", delim[here_count - 2]);
                printf("last_delim: [%s]\n", delim[here_count - 1]);
                heredoc_fd = tackle_heredoc(head, delim[here_count - 2], delim[here_count - 1]);
                if (heredoc_fd == -1)
                {
                    type = head;
                    perror ("Error setting up heredoc");
                    return (-1);
                }
                type->fd[0] = heredoc_fd; // Configura o fd_in
                type = head;
                return (1);
            }
            type = type->next;
        }
    }
    
    //type = head;
    //printf("head: [%p]\n", head);
    
    type = head;
    return (0);
}

int tackle_heredoc(t_types *type, char *start_delim, char *last_delim) 
{
    //printf("\n----\ntackle_heredoc\n");
    //printf("start_delim: [%s]\n", start_delim);
    //printf("last_delim: [%s]\n", last_delim);
    //printf("type: [%p]\n", type);
    char    *line;
    char    temp_file[64]; // Buffer para o nome do arquivo
    int     temp_fd;
    int     pid; // para gerar o identificador único do arquivo
    int     start_write;

    // Gerar um nome único para o arquivo temporário
    pid = getpid();
    int i = 0;
    while (i < 64 && pid > 0) {
        temp_file[i++] = (pid % 10) + '0';
        pid /= 10;
    }
    temp_file[i++] = '_';
    temp_file[i++] = 'h';
    temp_file[i++] = 'e';
    temp_file[i++] = 'r';
    temp_file[i++] = 'e';
    temp_file[i++] = 'd';
    temp_file[i++] = 'o';
    temp_file[i++] = 'c';
    temp_file[i++] = '\0';

    // Criar o arquivo temporário
    temp_fd = open(temp_file, O_CREAT | O_RDWR | O_TRUNC, 0600);
    if (temp_fd == -1) {
        perror("Error creating temporary file");
        return -1;
    }

    //printf("Temporary file created: %s\n", temp_file);

    // Escrever o conteúdo do heredoc no arquivo temporário
    //incluir o verificador de quantidade de << encontrados
    //flag para indicar que o arquivo temporário deve começar a ser escrito
    if (start_delim == NULL)
    {
        start_write = 1;
        while (1) 
        {
            line = readline("heredoc> ");
            if (!line) 
            {
                perror("Error reading line");
                break;
            }
            if (ft_strcmp(line, last_delim) == 0 && start_write == 1)
            {
                //printf("delim found\n");
                free(line);
                break;
            }
            if (start_write == 1)
            {
                write(temp_fd, line, ft_strlen(line));
                write(temp_fd, "\n", 1);
            }
            free(line);
        }
    }
    else
    {
        start_write = 0;
        while (1) 
        {
            line = readline("heredoc> ");
            if (!line) 
            {
                perror("Error reading line");
                break;
            }
            if (ft_strcmp(line, last_delim) == 0 && start_write == 1)
            {
                //printf("delim found\n");
                free(line);
                break;
            }
            if (start_delim == NULL || ft_strcmp(line, start_delim) == 0)
                start_write = 1;
            else if (start_write == 1)
            {
                write(temp_fd, line, ft_strlen(line));
                write(temp_fd, "\n", 1);
            }
            free(line);
        }
    }

    //printf("\n");
    //printf("pre type->cmd: [%s]\n", type->next->cmd);
    change_value(type, temp_file); // Substitui o `cmd` no nó do heredoc pelo caminho do arquivo temporário
    //printf("\n--\npos type->cmd: [%s]\n", type->next->cmd);

    // Retorna o descritor de arquivo
    //printf("Temporary file descriptor: %d\n", temp_fd);
    return temp_fd;
}
