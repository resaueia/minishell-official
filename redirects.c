/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:37:09 by rsaueia           #+#    #+#             */
/*   Updated: 2024/12/01 22:36:51 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int setup_redirection(t_init_input *args_list, t_types *type)
{
    //printf("\n----\non setup_redirection\n\n");
    //printf("types: [%p]\n", type);
    (void)type;
    (void)args_list;
    //t_init_input    *temp;
    t_types         *type_head;
    t_types         *type_echo;
    int             temp_fd;

    //temp = args_list;
    if (ft_strncmp(type->cmd, "echo", 4) == 0)
            type_echo = type;
    type_head = type;
    while (type)
    {
        //printf("type->cmd: [%s]       | fd_in: [%i] | fd_out: [%i]\n", type->cmd, type->fd[0], type->fd[1]);
        //if (type->next)
            //printf("type->next->cmd: [%s] | fd_in: [%i] | fd_out: [%i]\n", type->next->cmd, type->next->fd[0], type->next->fd[1]);
        if (ft_strcmp(type->cmd, "<") == 0 && type->next)
        {
            //printf("has [<]\n");
            if (access(type->next->cmd, R_OK) == -1) // check if the file has read permission
            {
                fprintf(stderr, "minishell: no such file or directory: %s\n", type->cmd); // print error message
                return (-1); // return -1
            }
            if (type->fd[0] != STDIN_FILENO)
                close(type->fd[0]);
            temp_fd = open(type->next->cmd, O_RDONLY); // open the file
            if (temp_fd == -1) // if the file descriptor is invalid
            {
                perror("Error opening fd for input redirect"); // print error message
                return (-1); // return -1
            }
            type->fd[0] = temp_fd;
            type->next->fd[0] = temp_fd; // set the fd_in to the file descriptor
            //printf("type:       [%p]_[%s]_[%u]_[%i]_[%i]\n", type->cmd, type->cmd, type->type, type->fd[0], type->fd[1]);
            //printf("type->next: [%p]_[%s]_[%u]_[%i]_[%i]\n", type->next->cmd, type->next->cmd, type->next->type, type->next->fd[0], type->next->fd[1]);
            //printf("type: [%p]_[%s]_[%u]_[%i]_[%i]\n", type->cmd, type->cmd, type->type, type->fd[0], type->fd[1]);
        }
        else if (ft_strcmp(type->cmd, ">") == 0 && type->next)
        {
            //printf("has [>]\n");
            if (access(type->next->cmd, W_OK) == -1 && errno != ENOENT)
            {
                perror("No write permission for output redirect");
                return (-1);
            }
            if (type->fd[1] != STDOUT_FILENO)
                close(type->fd[1]);
            temp_fd = open(type->next->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            printf("temp_fd: [%i]\n", temp_fd);
            if (temp_fd == -1)
            {
                perror("Error opening fd for output redirect");
                return (-1);
            }
            //type_echo->fd[1] = temp_fd;
            type->fd[1] = temp_fd;
            //type->next->fd[1] = temp_fd;
            //printf("type:       [%p]_[%s]_[%u]_[%i]_[%i]\n", type->cmd, type->cmd, type->type, type->fd[0], type->fd[1]);
            //printf("type->next: [%p]_[%s]_[%u]_[%i]_[%i]\n", type->next->cmd, type->next->cmd, type->next->type, type->next->fd[0], type->next->fd[1]);
            //printf("type_echo: [%p]_[%s]_[%u]_[%i]_[%i]\n", type_echo->cmd, type_echo->cmd, type_echo->type, type_echo->fd[0], type_echo->fd[1]);
        }
        else if (ft_strcmp(type->cmd, ">>") == 0 && type->next)
        {
            //printf("has [>>]\n");
            if (access(type->next->cmd, W_OK) == -1 && errno != ENOENT)
            {
                perror("No write permission for append redirect");
                return (-1);
            }
            if (type->fd[1] != STDOUT_FILENO)
                close(type->fd[1]);
            temp_fd = open(type->next->cmd, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (temp_fd == -1)
            {
                perror("Error opening fd for append redirect");
                return (-1);
            }
            //type_echo->fd[1] = temp_fd;
            type->fd[1] = temp_fd;
            //type->next->fd[1] = temp_fd;
            //printf("type:       [%p]_[%s]_[%u]_[%i]_[%i]\n", type->cmd, type->cmd, type->type, type->fd[0], type->fd[1]);
            //printf("type->next: [%p]_[%s]_[%u]_[%i]_[%i]\n", type->next->cmd, type->next->cmd, type->next->type, type->next->fd[0], type->next->fd[1]);
            //printf("type_echo: [%p]_[%s]_[%u]_[%i]_[%i]\n", type_echo->cmd, type_echo->cmd, type_echo->type, type_echo->fd[0], type_echo->fd[1]);
        }
        if (ft_strncmp(type_head->cmd, "echo", 4) == 0)
                type_echo->fd[1] = temp_fd; //set the fd_out
        type = type->next;
    }
    type = type_head;
    return (0);
}