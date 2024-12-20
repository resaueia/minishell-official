/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:59:21 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/20 15:18:06 by rsaueia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(int fd_out)
{
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putstr_fd(cwd, fd_out);
		ft_putchar_fd('\n', fd_out);
	}
	else
		perror("getcwd() incurred in unexpected error");
}
static char	*args_to_str(t_types *args)
{
	char	*str;
	t_types	*temp;

	temp = args;
	if (temp->next == NULL)
	{
		str = ft_strdup("");
		return (str);
	}
	if (temp->type == 3 && temp->next)
		temp = temp->next;
	str = ft_strdup(temp->cmd);
	temp = temp->next;
	while (temp && temp->type == 20)
	{
		str = ft_strjoin(str, " ");
		str = ft_strjoin(str, temp->cmd);
		temp = temp->next;
	}
	return (str);
}

void	ft_echo(t_types *cmds, t_envp **env_list, int fd_out) 
{
	(void)env_list;
	char	*args;
	int		newline;
	char	*tmp;

	newline = 1;
	if (cmds->cmd == NULL || cmds->next == NULL)
	{
		ft_putstr_fd("\n", fd_out);
		return ;
	}
	args = args_to_str(cmds);
	if (ft_strncmp(args, "-n", 2) == 0)
	{
		tmp = args;
		args++;
		while (*args == 'n')
			args++;
		if (*args == ' ' || *args == '\0')
		{
			newline = 0;
			args++;
		}
		else
			args = tmp;
	}
	ft_putstr_fd(args, fd_out);
	if (newline == 1)
		ft_putchar_fd('\n', fd_out);
}

void	ft_cd(t_types *cmds, t_envp **env_list)
{
	char	*path;
	(void)env_list;

	path = args_to_str(cmds);
	if (ft_strlen(path) == 1 && *path == '/')
	{
		chdir("/");
		change_path("/", "PWD", env_list);
	}
	else if (ft_strlen(path) >= 2)
	{
		if (ft_strncmp(path, "~/", 2) == 0)
			path = joinpath(path + 2, "HOME", env_list);
		if (chdir(path) == 0)
		{
			char	cwd[1024];
			getcwd(cwd, sizeof(cwd));
			change_path(cwd, "PWD", env_list);
		}
		else 
			printf("cd: %s: %s\n", strerror(errno), path);
	}
	else if (!*path || *path == '~') //caminho para HOME
	{
		path = change_path(path, "HOME", env_list); //alterar o value do pwd do env, path para home
		chdir(path); //aplicar chdir para modificação do diretório.
	}
	else if (*path == '-') //caminho para o último diretório
	{
		path = change_path(path, "OLDPWD", env_list); //alterar o value do pwd do env, recebendo o path do oldpwd
		chdir(path);//aplicar chdir para modificação do diretório.
	}
}

static int	validate_export(char *arg)
{
	if (!ft_isalpha(arg[0]) && arg[0] != '_') //if the first character is not a letter or an underscore, it will return 0
		return (0);
	else 
		return (1); //if the arg is valid, it will return 1
}

int	update_env_var(t_envp *env_list, char *key, char *value)
{
	t_envp	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0) // Se encontrar a chave
		{
			free(current->value); // Atualiza o valor
			current->value = ft_strdup(value);
			return (1); // Indica que a atualização foi realizada
		}
		current = current->next;
	}
	return (0); // Indica que a chave não foi encontrada
}

void	ft_export(t_types *cmds, t_envp **env_list)
{
	char	*var;
	char	*delim;

	var = args_to_str(cmds); // Converte os argumentos em uma string
	if (!validate_export(var)) // Valida o identificador
	{
		printf("minishell: export: `%s': not a valid identifier\n", var);
		free(var);
		return;
	}
	delim = ft_strchr(var, '=');
	if (delim)
	{
		*delim = '\0'; // Separa a chave do valor
		if (update_env_var(*env_list, var, delim + 1)) // Tenta atualizar a variável existente
		{
			free(var);
			return;
		}
		create_new_node(env_list, var, delim + 1); // Cria uma nova variável
	}
	free(var);
}

void	ft_unset(t_types *cmds, t_envp **env_list)
{
	char	*var;
	t_envp *current;
	t_envp *prev;

	var = args_to_str(cmds); //transforming the args to a string
	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, var) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env_list = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

static void	handle_ctrl_d_sig(t_init_input *input_list, t_envp *env_list)
{
	printf("exit\n");
	rl_clear_history(); //clear the history
	free_env(env_list); //free the env list
	free_list(input_list); //free the input list
	exit(0); //exit the shell with success
}

static int	parse_exit_status(char *prompt, int i)
{
	int ret;

	ret = 0; //initialize the return value
	while (prompt[i] == ' ') //skip the spaces
		i++;
	while (prompt[i]) //while the prompt has characters
	{
		if (is_whitspace(prompt[i]))
		{
			printf("exit\nminishell: exit: too many arguments\n");
			return (-1); //if the prompt has more than one argument, it will return -1
		}
		if (!ft_isdigit(prompt[i]))
		{
			printf("exit\nminishell: exit: %s: numeric argument required\n", prompt);
			exit(255);
		}
		ret = ret * 10 + prompt[i] - '0'; //convert the string to an integer
		i++;
	}
	return (ret); //return the integer
}

void	exit_shell(char *prompt, char *prompt_dup, t_init_input *input_list, t_envp *env_list)
{
	int ret;
	int i;
	
	i = 0;
	if (!prompt)
	{
		handle_ctrl_d_sig(input_list, env_list);
		return ;
	}
	while (prompt[i] == ' ') //skip the spaces
		i++;
	ret = parse_exit_status(prompt, 4);
	if (ret == -1)
	{
		exit_mini(input_list, prompt, prompt_dup, env_list); // exit the shell end clear the memory
		exit(1); //exit the shell with error "too many arguments" or "invalid argument"
	}
	exit_mini(input_list, prompt, prompt_dup, env_list); // exit the shell end clear the memory
	exit(ret);
}
