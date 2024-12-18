/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:59:21 by rsaueia-          #+#    #+#             */
/*   Updated: 2024/12/18 13:42:54 by jparnahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(int fd_out)
{
	char cwd[1024]; // variable where we'll store the path of the current dir
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
	//alterar para t_types *args e alterar a lógica de verificação dos args. 
	//será necessário um loop para percorrer os args e verificar todos os argumentos existentes.
	(void)env_list;
	char	*args;
	int		newline;
	char	*tmp;

	newline = 1; //flag to print a newline
	if (cmds->cmd == NULL || cmds->next == NULL) //if echo come without args, it will print just a newline
	{
		ft_putstr_fd("\n", fd_out);
		return ;
	}
	args = args_to_str(cmds); //transforming the args to a string
	if (ft_strncmp(args, "-n", 2) == 0) //if echo come with -n, it will not print a newline
	{
		tmp = args;
		args++; //incrementing the pointer to check the next character
		while (*args == 'n')
			args++; //incrementing the pointer to check the next character
		if (*args == ' ' || *args == '\0')
		{
			newline = 0; //flag to not print a newline	
			args++; //incrementing the pointer to check the next character
		}
		else
		{
			args = tmp; //if the next character is not a space, it will return to the original pointer
			newline = 1; //flag to print a newline
		}
	}
	//remove_quotes(&args); //to remove quotes from the args
	if (newline == 1) //if newline is 1, it will print a newline
	{
		ft_putstr_fd(args, fd_out); //printing the args with a newline
		ft_putchar_fd('\n', fd_out);
	}
	else if (newline == 0) //if newline is 0, it will not print a newline
		ft_putstr_fd(args, fd_out); //printing the args without a newline
}

void	ft_cd(t_types *cmds, t_envp **env_list)
{
	char	*path;
	(void)env_list;

	path = args_to_str(cmds); //transforming the args to a string
	if (ft_strlen(path) == 1 && *path == '/') //path for root
	{
		chdir("/");
		change_path("/", "PWD", env_list); //change the value of the pwd in the env, and update the path of the oldpwd
	}
	else if (ft_strlen(path) >= 2) //path for a specific directory
	{
		if (ft_strncmp(path, "~/", 2) == 0)
			path = joinpath(path + 2, "HOME", env_list); //for include the path of home in the path
		if (chdir(path) == 0)
		{
			char	cwd[1024];
			getcwd(cwd, sizeof(cwd)); //get the current path
			change_path(cwd, "PWD", env_list); //change the value of the pwd in the env, and update the path of the oldpwd
		}
		else 
			printf("cd: %s: %s\n", strerror(errno), path); //print the error message
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

void	ft_export(t_types *cmds, t_envp **env_list)
{
	char	*var;
	char	*delim;
	t_envp	*current;

	var = args_to_str(cmds); //transforming the args to a string
	if (!validate_export(var)) //validating the export
	{
		printf("minishell: export: `%s': not a valid identifier\n", var);
		return ;
	}
	delim = ft_strchr(var, '=');
	if (delim)
	{
		*delim = '\0';
		current = *env_list;
		while (current)
		{
			if (ft_strcmp(current->key, var) == 0) // Here, we traverse the list to check for the existence of 'key'. If it's already there, we update its value and return.
				{
					free(current->value);
					current->value = ft_strdup(delim + 1);
					return ;
				}
			current = current->next;
		}
		create_new_node(env_list, var, delim + 1);
		// If 'key' is not present, we create it, by adding a new node to our var list.
	}
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
			// We traverse the list and look for the desired key. If found, we remove it from the list and free all memory associated with it.
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
