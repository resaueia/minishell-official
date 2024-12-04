/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:55:24 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/03 22:35:44 by jparnahy         ###   ########.fr       */
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
}

static char **split_heredoc(t_types *type, int here_count)
{
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
            i++;
        }
        type = type->next;
    }
    delim[i] = NULL;
    /*i = 0;
    while (delim[i])
    {
        printf("delim[%i]: [%s]\n", i, delim[i]);
        i++;
    }*/
    return (delim);
}

static void change_value(t_types *type, char *value)
{
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
    t_types *head;
    char    **delim;
    int     heredoc_fd;
    int     here_count; //for count heredocs
    (void) input_list;
    (void) delim;

    head = type;
    here_count = 0;
    while (type) // loop to count the number of heredocs
    {
        if (ft_strcmp(type->cmd, "<<") == 0 && type->next->cmd)
            here_count++;
        type = type->next;
    }
    type = head;
    if (here_count == 1) //if there is only one heredoc
    {
        include_eof(type); //include the end of file type
        while(type) // loop to process the heredoc
        {
            if (ft_strcmp(type->cmd, "<<") == 0 && type->next->cmd)
            {
                heredoc_fd = tackle_heredoc(head, NULL, type->next->cmd);
                if (heredoc_fd == -1)
                {
                    type = head;
                    perror ("Error setting up heredoc");
                    return (-1);
                }
                type->fd[0] = heredoc_fd; // set the fd_in
                type = head; // reset the pointer
                return (1);
            }
            type = type->next; // go to the next node
        }
    }
    else if (here_count > 1) //if there are multiple heredocs
    {
        include_eof(type); //include the end of file type
        delim = split_heredoc(type, here_count); //split the heredocs for get the final delimiter value
        while(type)
        {
            if (ft_strcmp(type->cmd, "<<") == 0 && type->next->cmd)
            {
                //process the heredoc with the start and end delimiters
                heredoc_fd = tackle_heredoc(head, delim[here_count - 2], delim[here_count - 1]);
                if (heredoc_fd == -1)
                {
                    type = head;
                    perror ("Error setting up heredoc");
                    return (-1);
                }
                type->fd[0] = heredoc_fd; // set the fd_in
                type = head; // reset the pointer
                return (1);
            }
            type = type->next; // go to the next node
        }
    }
    type = head; // reset the pointer
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
    temp_file[i++] = '/';
    temp_file[i++] = 't';
    temp_file[i++] = 'm';
    temp_file[i++] = 'p';
    temp_file[i++] = '/';
    temp_file[i++] = 'h';
    temp_file[i++] = 'e';
    temp_file[i++] = 'r';
    temp_file[i++] = 'e';
    temp_file[i++] = 'd';
    temp_file[i++] = 'o';
    temp_file[i++] = 'c';
    temp_file[i++] = '_';
    while (pid > 0) {
        temp_file[i++] = (pid % 10) + '0';
        pid /= 10;
    }
    temp_file[i++] = '.';
    temp_file[i++] = 't';
    temp_file[i++] = 'x';
    temp_file[i++] = 't';
    temp_file[i++] = '\0';

    // Criar o arquivo temporário
    temp_fd = open(temp_file, O_CREAT | O_RDWR | O_TRUNC, 0600);
    if (temp_fd == -1) {
        perror("Error creating temporary file");
        return -1;
    }
    //Escrever o conteúdo do heredoc no arquivo temporário
    if (start_delim == NULL)
    {
        start_write = 1; //flag para indicar que o arquivo temporário deve começar a ser escrito
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
