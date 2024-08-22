/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtiner.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 19:06:39 by saleshin          #+#    #+#             */
/*   Updated: 2024/08/08 22:48:58 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//success 0, error 1
//getcwd for get current working directory, getcwd allocate memory 
//in buffer that must be freed
int	ft_pwd(void)
{
	char	*buffer;

	buffer = getcwd(NULL, 0);
	if (buffer == NULL)
	{
		perror("getcwd error");
		return (1);
	}
	ft_putendl_fd(buffer, STDOUT_FILENO);
	free(buffer);
	return (0);
}

//success 0, error -1 and errno
int	ft_cd(t_env **env_lst, t_ast_node *command)
{
	char	*buffer;
	char	*path;
	int		err;

	err = 0;
	buffer = NULL;
	if (command->first_child->next_sibling->next_sibling->param == 0)
		return (0);
	path = command->first_child->next_sibling->next_sibling->first_child->value;
	if (chdir(path) != 0)
	{
		perror("cd");
		err = errno;
		return (err);
	}
	buffer = getcwd(NULL, 0);
	if (buffer == NULL)
	{
		perror("getcwd error");
		err = errno;
		return (errno);
	}
	upd_envvar("PWD", buffer, *env_lst);
	free(buffer);
	return (err);
}

// 1 if it is a builtin, 0 if not
//HERE TO CHECK IF EXEC IS A VALID BUILTIN
int	is_builtin(t_ast_node *command)
{
	char	*exec;
//	delete all before / like usr/bin/cd ---> cd
	exec = command->first_child->next_sibling->value;
	if (!exec)
		return (0);
	if (ft_strcmp(exec, "echo") == 0
		|| ft_strcmp(exec, "cd") == 0
		|| ft_strcmp(exec, "pwd") == 0
		|| ft_strcmp(exec, "export") == 0
		|| ft_strcmp(exec, "unset") == 0
		|| ft_strcmp(exec, "env") == 0)
	{
		return (1);
	}
	return (0);
}

// returns 0 or err code or -1 if TODO work with abs and rel pathi 
int	builtiner(t_ast_node *command, t_env **env_list)
{
	char	*exec;
	int		exit_code;

	exit_code = 0;
	if (command == NULL || command->first_child == NULL
		|| command->first_child->next_sibling == NULL)
		return (1);
	exec = command->first_child->next_sibling->value;
	if (!exec || !*exec)
		return (1);
	if (ft_strcmp(exec, "echo") == 0)
		exit_code = ft_echo(command);
	else if (ft_strcmp(exec, "cd") == 0)
		exit_code = ft_cd(env_list, command);
	else if (ft_strcmp(exec, "pwd") == 0)
	{
		exit_code = ft_pwd();
	}
	else if (ft_strcmp(exec, "export") == 0)
		exit_code = ft_export(env_list, command);
	else if (ft_strcmp(exec, "unset") == 0)
		exit_code = ft_unset(env_list, command);
	else if (ft_strcmp(exec, "env") == 0)
		exit_code = print_env(*env_list);
	return (exit_code);
}

//checks if the command is valid absolute path /usr/bin/ls, return 0
//if it is not valid access set errno
int ft_path_ok(char *command)
{
    if (access(command, F_OK) == -1)
        return (127);  // File does not exist
    if (access(command, X_OK) == -1)
	{
        if (errno == EACCES)
            return (126);  // No execute permission
		else if (errno == ENOTDIR)
            return (126);  // Component of the path is not a directory
        return (1);  // General error (other reasons)
    }
    return (0);  // Success: file exists and is executable
}

//to check if it is possible to have / at the end
//extracts string after last /
char *extract_exec(char *command)
{
	char	*ptr_start;

	if (ft_path_ok(command) != 0)
		return (NULL);
	while (*command)
	{
		if (*command == '/')
			ptr_start = command + 1;
		command++;
	}
	return (ptr_start);
}