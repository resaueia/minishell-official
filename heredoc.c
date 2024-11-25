/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:55:24 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/11/24 21:04:50 by jparnahy         ###   ########.fr       */
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

/*how to create a pipe:
int pipe_fd[2];
if (pipe(pipe_fd) == -1)
{
    perror("pipe");
    exit(EXIT_FAILURE);
}
*/

/*int     is_heredoc(t_init_input *input_list, t_types *type)
{
    t_types    *temp;
    int             heredoc_fd;

    (void) input_list;

    temp = type;
    while(temp)
    {
        printf("cmd: [%s]\n", temp->cmd);
        //printf("delim: [%s]\n", temp->next->cmd);
        if (ft_strcmp(temp->cmd, "<<") == 0 && temp->next->cmd)
        {
            printf("\n----\n type is hdoc && has next node\n");

            printf("cmd is: [%s]\n", temp->cmd);
            printf("delim: [%s]\n", temp->next->cmd);
                        
            heredoc_fd = tackle_heredoc(temp->next->cmd);
            if (heredoc_fd == -1)
            {
                perror ("Error setting up heredoc");
                return (-1);
            }
            input_list->fd_in = heredoc_fd;
            return (1);
        }
        temp = temp->next;
    }
    return (0);
}*/

/*int    tackle_heredoc(char *delim)
{
    printf("\n----\non tackle_heredoc\n\n");
    printf("delim: [%s]\n", delim);
    char            *line;
    int             pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        perror("Error creating heredoc pipe\n");
        return (-1);
    }
    while (1)
    {
        line = readline("heredoc> ");
        if (!line)
        {
            perror("Error reading line\n");
            break ;
        }
        if (ft_strcmp(line, delim) == 0)
        {
            printf("delim found\n");
            free(line);
            break ;
        }
        write(pipe_fd[1], line, ft_strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }*/
    //close(pipe_fd[1]);
    //if (input_list->fd_in != STDIN_FILENO)
      //  close(input_list->fd_in);
    
    //return (pipe_fd[0]);
    //dup2(pipe_fd[0], STDIN_FILENO);
    //close(pipe_fd[0]);
    /*After writing stuff on the pipe, I close its writing end, use
    the dup2 function to set its reading end to the standard input (which used to be the keyboard)
    and then proceed to close the actual read end of the pipe, hence finishing the whole process.*/

    //Not sure if the execution part of this should be written here already or elsewhere.
    //Not sure if the execution part of this should be written here already or elsewhere.
//}

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
    t_types    *head;
    int             heredoc_fd;

    (void) input_list;

    head = type;
    //printf("head: [%p]\n", head);
    while(type)
    {
        //printf("cmd: [%s]\n", temp->cmd);
        //printf("delim: [%s]\n", temp->next->cmd);
        if (ft_strcmp(type->cmd, "<<") == 0 && type->next->cmd)
        {
            //printf("\n----\ntype is hdoc && has next node\n");

            //printf("cmd is: [%s]\n", type->cmd);
            //printf("delim: [%s]\n", type->next->cmd);
                        
            heredoc_fd = tackle_heredoc(head, type->next->cmd);
            if (heredoc_fd == -1)
            {
                type = head;
                perror ("Error setting up heredoc");
                return (-1);
            }
            input_list->fd_in = heredoc_fd; // Configura o fd_in
            //printf("\n----\nback to is_heredoc\n\n");
            //printf("cmd: [%s]\n", type->cmd);
            //printf("delim: [%s]\n", type->next->cmd);
            type = head;
            return (1);
        }
        type = type->next;
    }
    type = head;
    return (0);
}

int tackle_heredoc(t_types *type, char *delim) 
{
    //printf("\n----\ntackle_heredoc\n");
    //printf("delim: [%s]\n", delim);
    //printf("type: [%p]\n", type);
    char *line;
    char temp_file[64]; // Buffer para o nome do arquivo
    int temp_fd;

    // Gerar um nome único para o arquivo temporário
    snprintf(temp_file, sizeof(temp_file), "/tmp/heredoc_%d.tmp", getpid());

    // Criar o arquivo temporário
    temp_fd = open(temp_file, O_CREAT | O_RDWR | O_TRUNC, 0600);
    if (temp_fd == -1) {
        perror("Error creating temporary file");
        return -1;
    }

    //printf("Temporary file created: %s\n", temp_file);

    // Escrever o conteúdo do heredoc no arquivo temporário
    while (1) 
    {
        line = readline("heredoc> ");
        if (!line) 
        {
            perror("Error reading line");
            break;
        }
        if (ft_strcmp(line, delim) == 0) 
        {
            //printf("delim found\n");
            free(line);
            break;
        }
        write(temp_fd, line, ft_strlen(line));
        write(temp_fd, "\n", 1);
        free(line);
    }

    // Reposicionar o cursor no início do arquivo para leitura futura
    lseek(temp_fd, 0, SEEK_SET);

    //printf("\n");
    //printf("pre type->cmd: [%s]\n", type->next->cmd);
    change_value(type, temp_file); // Substitui o `cmd` no nó do heredoc pelo caminho do arquivo temporário
    //printf("\n--\npos type->cmd: [%s]\n", type->next->cmd);

    // Retorna o descritor de arquivo
    //printf("Temporary file descriptor: %d\n", temp_fd);
    return temp_fd;
}
