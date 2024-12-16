/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 23:43:15 by jparnahy          #+#    #+#             */
/*   Updated: 2024/12/16 16:24:21 by jparnahy         ###   ########.fr       */
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
    if (c == '\'' || c == '\"')
        return (1);
    return (0);
}*/

static void status_expander(char *str, int i, int exit_status)
{
    printf("\non status_expander\n");
    char    *status_str;

    status_str = ft_itoa(exit_status);
    printf("status_str: [%s]\n", status_str);
    str[i] = '\0';
    str = ft_strjoin(str, status_str);
    printf("str: [%s]\n", str);
    free(status_str);
}

static void to_expander(char *str, int i, t_envp *env, int exit_status)
{
    printf("\non to_expander\n");
    printf("str: [%s]\n--\n", str);
    (void)env;
    (void)exit_status;
    //char    *key;
    //char    *value;
    //char    *expanded;

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
                printf("EXPANDIR EXIT_STATUS\n");
                status_expander(str, i, exit_status);
                printf("status_expander == str: [%s]\n", str);
                return ;
            }
            else if (str[i + 1] != '\0')
            {
                //EXPANDIR VARIAVEL DE AMBIENTE
                printf("EXPANDIR VARIAVEL DE AMBIENTE\n");  
                return ;
            }
        }
        i++;
    }
}

static char *expander_or_not(char *cmd, t_envp *env_list, int exit_status)
{
    printf("\n----\non expander_or_not\n");
    printf("cmd: [%s]\n", cmd);
    int     i;

    i = -1;
    if (cmd[0] == '\'') //se começar com aspas simples e estiver fechada, remover aspas apenas e não expandir
    {
        remove_quotes(&cmd);
        return (cmd);
    }
    while(cmd[++i])
    {
        printf("cmd[%i]: [%c]\n", i, cmd[i]);
        if (cmd[i] == '\"') //se começar com aspas duplas e estiver fechada, remover e continuar para expandir
            rmv_qts(&cmd);
        if (cmd[i] == '$')
        {
            printf("\n--\nhas dol\n--\n");
            printf("cmd[%i]: [%c]\n", i, cmd[i]);
            to_expander(cmd, i, env_list, exit_status);
            printf("\nAFTER EXPANDER\ncmd: [%s]\n--\n", cmd);
        }
    }
    return (cmd);
}

void    lets_expander(t_types *types, t_envp *env_list, int exit_status)
{
    printf("\n----\non lets_expander\n");
    t_envp  *env;

    env = env_list;
    while (types)
    {
        if (has_dol(types->cmd))
        {
            printf("has_dol\n");
            types->cmd = expander_or_not(types->cmd, env, exit_status);
        }
        types = types->next;
    }
    //free_env(env);
    env = NULL;
}


/*static int is_single_quoted(char *cmd)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (cmd[i])
    {
        if (cmd[i] == '\'')
            count++;
        i++;
    }
    return (count % 2);
}

static int  is_double_quoted(char *cmd)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (cmd[i])
    {
        if (cmd[i] == '\"')
            count++;
        i++;
    }
    return (count % 2);
}

static int  contains_dollar(char *cmd)
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

static char *prs_pre(char *result, char *cmd, char *start)
{
    size_t  prefix_len;
    char    *prefix;

    prefix_len = start - cmd; // calcula o tamanho do prefixo
    prefix = ft_substr(cmd, 0, prefix_len); // extrai o prefixo
    result = ft_strjoin_free(result, prefix);
    free(prefix);
    return result;
}

static char *prs_var(char *rslt, char **stt, t_envp *env, int last_exit_status)
{
    char *end;
    char *key;
    char *value;

    if (*((*stt) + 1) == '?') // Caso especial para `$?`
    {
        value = ft_itoa(last_exit_status);
        rslt = ft_strjoin_free(rslt, value);
        free(value);
        *stt += 2; // Move o ponteiro após `$?`
        return rslt;
    }
    end = *stt + 1;
    while (ft_isalnum(*end) || *end == '_') // Determina o final da variável
        end++;
    key = ft_substr((*stt) + 1, 0, end - (*stt) - 1); // Extrai a chave da variável
    value = get_value(key, env); // Busca o valor da variável
    rslt = ft_strjoin_free(rslt, value ? value : ""); // Adiciona o valor
    free(key);
    *stt = end; // Move o ponteiro após a variável
    return rslt;
}

char *expand_variables(char *cmd, t_envp *env_list, int last_exit_status)
{
    char *result;
    char *start;

    result = ft_strdup("");
    start = cmd;
    while ((start = ft_strchr(start, '$')) != NULL)
    {
        result = prs_pre(result, cmd, start); // Processa o prefixo antes do $
        result = prs_var(result, &start, env_list, last_exit_status); // Processa o $
    }
    result = ft_strjoin_free(result, start); // Adiciona o restante da string
    return result;
}

static char *expand_double_quoted(char *cmd, t_envp *env_list, int last_exit_status)
{
    char    *no_quotes;
    char    *expanded;

    no_quotes = ft_strdup(cmd);
    remove_quotes(&no_quotes);
    expanded = expand_variables(no_quotes, env_list, last_exit_status);
    free(no_quotes);
    return (expanded);
}

static char *expander_or_not(char *cmd, t_envp *env_list, int last_exit_status)
{
    char *expanded;

    if (is_single_quoted(cmd))
    {
        expanded = ft_strdup(cmd);
        remove_quotes(&expanded); // Aspas simples: remover e não expandir
        return (expanded);
    }
    if (is_double_quoted(cmd))
    {
        expanded = expand_double_quoted(cmd, env_list, last_exit_status); // Aspas duplas: expandir
        return (expanded);
    }    
    if (contains_dollar(cmd))
        expanded = expand_variables(cmd, env_list, last_exit_status); // Expansão de variáveis
    return expanded ? expanded : ft_strdup(cmd); // Retorna expandido ou original
}*/