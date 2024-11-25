/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:37:09 by rsaueia           #+#    #+#             */
/*   Updated: 2024/11/24 21:03:40 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int setup_redirection(t_init_input *args_list, t_types *type)
{
    //printf("\n----\non setup_redirection\n\n");
    //print_the_stack(args_list);
    /*printf("\n--\nprint the types:\n");
    t_types *tmp = type;
    while (tmp)
    {
        printf("cms: [%p]_[%s]_[%u]\n", tmp->cmd, tmp->cmd, tmp->type);
        tmp = tmp->next;
    }
    printf("\n--\n");*/
    (void)type;
    //t_init_input    *temp;
    t_types         *type_head;
    int             temp_fd;

    //temp = args_list;
    type_head = type;
    while (type)
    {
        //printf("type->cmd: [%s]\n", type->cmd);
        if (ft_strcmp(type->cmd, "<") == 0 && type->next)
        {
            //printf("has [<]\n");
            if (args_list->fd_in != STDIN_FILENO) // if fd_in is not the default stdin
            {
                close(args_list->fd_in); // close the current fd_in
                args_list->fd_in = STDIN_FILENO; // set fd_in to stdin
            }
            if (access(type->next->cmd, R_OK) == -1) // check if the file has read permission
            {
                fprintf(stderr, "minishell: no such file or directory: %s\n", type->cmd); // print error message
                return (-1); // return -1
            }
            temp_fd = open(type->next->cmd, O_RDONLY); // open the file
            if (temp_fd == -1) // if the file descriptor is invalid
            {
                perror("Error opening fd for input redirect"); // print error message
                return (-1); // return -1
            }
            //printf("temp_fd: [%d]\n", temp_fd);
            args_list->fd_in = temp_fd; // set the fd_in to the file descriptor
        }
        else if (ft_strcmp(type->cmd, ">") == 0 && type->next)
        {
            //printf("has [>]\n");
            if (access(type->next->cmd, W_OK) == -1 && errno != ENOENT)
            {
                perror("No write permission for output redirect");
                return (-1);
            }
            temp_fd = open(type->next->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if (temp_fd == -1)
            {
                perror("Error opening fd for output redirect");
                return (-1);
            }
            //printf("temp_fd: [%d]\n", temp_fd);
            args_list->fd_out = temp_fd;
        }
        else if (ft_strcmp(type->cmd, ">>") == 0 && type->next)
        {
            //printf("has [>>]\n");
            if (access(type->next->cmd, W_OK) == -1 && errno != ENOENT)
            {
                perror("No write permission for append redirect");
                return (-1);
            }
            temp_fd = open(type->next->cmd, O_WRONLY | O_CREAT | O_APPEND, 0777);
            if (temp_fd == -1)
            {
                perror("Error opening fd for append redirect");
                return (-1);
            }
            //printf("temp_fd: [%d]\n", temp_fd);
            args_list->fd_out = temp_fd;
        }
        type = type->next;
    }
    type = type_head;
    return (0);
}