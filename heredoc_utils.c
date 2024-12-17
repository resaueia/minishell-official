/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:57:07 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/17 19:29:33 by rsaueia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    clear_heredoc_files(void)
{
    DIR             *dir;
    struct dirent   *entry;
    char            filepath[256]; // Tamanho suficiente para caminhos completos em /tmp

    dir = opendir("/tmp");
    if (!dir) {
        perror("Failed to open /tmp directory");
        return;
    }
    while ((entry = readdir(dir)) != NULL) 
    {
        if (ft_strncmp(entry->d_name, "heredoc_", 8) == 0) 
        {
            if (ft_strlcpy(filepath, "/tmp/", sizeof(filepath)) >= sizeof(filepath)) 
            {
                fprintf(stderr, "Path truncation error: %s\n", "/tmp/");
                continue;
            }
            if (ft_strlcat(filepath, entry->d_name, sizeof(filepath)) >= sizeof(filepath)) 
            {
                fprintf(stderr, "Path truncation error: %s\n", entry->d_name);
                continue;
            }
            if (unlink(filepath) == -1) 
                perror("Failed to delete heredoc file");
            else 
                printf("Deleted temporary file: %s\n", filepath);
        }
    }
    closedir(dir);
}