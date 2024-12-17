/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 23:43:15 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/17 16:12:52 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static char *expander_or_not(char *cmd, t_envp *env_list, int last_exit_status)
{
    printf("\n----\non expander_or_not\n");
    printf("cmd: [%s]\n", cmd);
    char    *temp;
    char    *expanded;
    char    *key;
    char    *suffix;
    
    temp = ft_strdup(cmd);
    if (temp[0] == '\"') //se começar com aspas duplas e estiver fechada, remover e continuar para expandir
        remove_quotes(&temp);
    else if (temp[0] == '\'') //se começar com aspas simples e estiver fechada, remover aspas apenas e não expandir
    {
        remove_quotes(&temp);
        cmd = ft_strdup(temp);
        temp = free_char_ptr(temp);
        return (cmd);
    }
    if (temp[0] == '$') //se começar com $, verificar a str completa
    {
        temp++; //incrementing the pointer to the next character for check next conditions
		if (*temp == '\0') //if cmd come with $ and no args, it will print just a char '$'
			temp = cmd; //changing the pointer to the char '$'
		else //if cmd come with $ and args, need check what kind of args it is
		{
            // Caso especial para `$?`
            if (temp[0] == '?') 
            {
                char *status_str = ft_itoa(last_exit_status); // Converte o status para string
                cmd = ft_strdup(status_str); 
                free(status_str);
                free(temp - 1); // Liberar a memória original de temp
                return (cmd);
            }
            if ((ft_isalpha(temp[0]) == 0) && ft_strncmp(temp, "_", 1)) //if echo come with $ and args in lower case, it will print just a newline
			{
                if(ft_strncmp(temp, "\'", 1) == 0 || ft_strncmp(temp, "\"", 1) == 0)
                    remove_quotes(&temp);
                else if (temp[0] == '0')
                    temp = ft_strjoin("bash", temp);
                else if (temp[0] == '-')
                    temp = ft_strjoin("himBHs", temp + 1);
                else if (ft_isdigit(temp[0]) || is_special(temp[0]))
                    temp++;
			}
			else if (ft_isalpha(temp[0]) || (ft_strncmp(temp, "_", 1) == 0)) //if echo come with $ and args in upper case, it will check if is a key of env list
			{
                // Caso seja uma variável de ambiente com ou sem caracteres adicionais
                key = extract_key(temp); // Extrai a chave da variável (ex: "USER" de "USER_123")
                suffix = temp + ft_strlen(key); // O restante após a chave (ex: "_123" de "USER_123")
				if (is_key(temp, env_list) == 1) //if is a key, it will get the value of the key
                {
                    expanded = get_value(key, env_list); // Obtém o valor da variável
                    cmd = ft_strjoin(expanded, suffix); // Concatena o valor expandido com o sufixo
                    free(expanded); // Libera a memória do valor expandido
                }
				else //if is not a key, it will print just a newline
                    temp = ft_strdup("\n");
                free(key);
                //free(temp - 1); // Liberar a memória original de temp
                return (cmd);
			}
		}
    }
    cmd = ft_strdup(temp);
    return (cmd);
}*/

static int  has_dol(char *cmd)
{
    int i;

    i = 0;
    while (cmd[i])
    {
        if (cmd[i] == '$')
            return (1);
        i++;
    }
    return (0);
}

static void rmv_qts(char **str)
{
	char	*src;
	char	*dst;

	src = *str;
	dst = *str;

    while (*src)
    {
        if (*src != '\"')
            *dst++ = *src;
        src++;
    }
    *dst = '\0';
}

/*static int  char_valid(char c)
{
    if (char valido)
        return (1);
    return (0);
}*/

char *ft_strjoin_three(char *s1, char *s2, char *s3)
{
    char *joined;
    size_t len;

    len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1;
    joined = (char *)malloc(sizeof(char) * len);
    if (!joined)
        return (NULL);
    ft_strlcpy(joined, s1, len);
    ft_strlcat(joined, s2, len);
    ft_strlcat(joined, s3, len);
    return joined;
}


static char *status_expander(char *str, int i, int exit_status)
{
    printf("\non status_expander\n");
    char    *prefix;
    char    *suffix;
    char    *status_str;
    char    *expanded;

    prefix = ft_substr(str, 0, i); // Extrai o prefixo até o $
    status_str = ft_itoa(exit_status); // Converte o status para string
    suffix = ft_strdup(str + i + 2); // Extrai o sufixo após o $?
    expanded = ft_strjoin_three(prefix, status_str, suffix); // Concatena o prefixo, status e sufixo

    free(prefix);
    free(status_str);
    free(suffix);
    free(str);
    return (expanded);
}

static char *env_var_expander(char *str, int i, t_envp *env_list)
{
    char *prefix;
    char *key;
    char *value;
    char *suffix;
    char *new_str;
    int j;

    j = i + 1;
    while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
        j++;
    prefix = ft_substr(str, 0, i);
    key = ft_substr(str, i + 1, j - i - 1);
    value = get_value(key, env_list);
    suffix = ft_strdup(str + j);
    new_str = ft_strjoin_three(prefix, value ? value : "", suffix);

    free(prefix);
    free(key);
    free(suffix);
    free(str);
    return new_str;
}


static char *to_expander(char *str, int i, t_envp *env, int exit_status)
{
    //printf("\non to_expander\n");
    //printf("str: [%s]\n--\n", str);
    char    *expanded;

    i = 0;
    while (str[i])
    {
        if (str[i] == '$')
        {
            /*if (char_valid(str[i - 1])) //verificar char valido antes do $
            {
                i++;
                continue ;
            }*/
            if (str[i + 1] == '?')
            {
                //EXPANDIR EXIT_STATUS
                expanded = status_expander(str, i, exit_status);;
                return (expanded);
            }
            else if (str[i + 1] != '\0' && str[i + 1] != '?')
            {
                //EXPANDIR VARIAVEL DE AMBIENTE
                expanded = env_var_expander(str, i, env);
                return (expanded);
            }
        }
        i++;
    }
    return (str);
}

static char *expander_or_not(char *cmd, t_envp *env_list, int exit_status)
{
    //printf("\n----\non expander_or_not\n");
    //printf("cmd: [%s]\n", cmd);
    int     i;

    i = 0;
    while(cmd[i])
    {
        if (cmd[i] == '\'')//se tiver com aspas simples e estiver fechada, remover aspas apenas e não expandir
        { 
            remove_quotes(&cmd);
            return (cmd);
        }
        i++;
    }
    i = 0;
    while(cmd[i])
    {
        if (cmd[i] == '\"') //se começar com aspas duplas e estiver fechada, remover e continuar para expandir
            rmv_qts(&cmd);
        if (cmd[i] == '$')
            cmd = to_expander(cmd, i, env_list, exit_status);
        i++;
    }
    return (cmd);
}

void    lets_expander(t_types *types, t_envp *env_list, int exit_status)
{
    //printf("\n----\non lets_expander\n");
    t_envp  *env;

    env = env_list;
    while (types)
    {
        if (has_dol(types->cmd))
            types->cmd = expander_or_not(types->cmd, env, exit_status);
        types = types->next;
    }
    env = NULL;
}
