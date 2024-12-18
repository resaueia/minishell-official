/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:57:07 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/18 19:34:05 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    clear_heredoc_files(void)
{
    DIR *dir;
    struct dirent *entry;
    char filepath[64]; // Tamanho suficiente para caminhos completos em /tmp

    dir = opendir("/tmp");
    if (!dir) 
    {
        perror("Failed to open /tmp directory");
        return;
    }
    while ((entry = readdir(dir)) != NULL) // Percorre todos os arquivos no diretório
    {
        if (ft_strncmp(entry->d_name, "heredoc_", 8) == 0) 
        {
            // Constrói o caminho completo do arquivo
            if (ft_strlcpy(filepath, "/tmp/", sizeof(filepath)) >= sizeof(filepath)) 
            {
                fprintf(stderr, "Path truncation error: %s\n", "/tmp/");
                continue;
            }
            if (ft_strlcat(filepath, entry->d_name, sizeof(filepath)) >= sizeof(filepath)) 
            {
                fprintf(stderr, "Path truncation error: %s\n", entry->d_name);
                continue;
            }// Remove o arquivo
            if (unlink(filepath) == -1) 
                perror("Failed to delete heredoc file");
            else 
                printf("Deleted temporary file: %s\n", filepath);
        }
    }
    closedir(dir); // Fecha o diretório
}
