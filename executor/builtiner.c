/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtiner.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 19:06:39 by saleshin          #+#    #+#             */
/*   Updated: 2024/06/24 19:06:49 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//success 0, error 1
int	ft_pwd(void)
{
	char	*buffer;

	buffer = NULL;
	// Get the current working directory
	buffer = getcwd(NULL, 0);
	if (buffer == NULL)
	{
		perror("getcwd error");
		return (1);
	}
	// Output the current working directory
	ft_putendl_fd(buffer, STDOUT_FILENO);
	// Free the allocated buffer by getcwd
	free(buffer);
	return (0);
}

//success 0, error -1 and errno
int	ft_cd(t_env **env_lst, t_ast_node *command)
{
	char	*buffer;
	char	*path;
	int	err = 0;

	buffer = NULL;
	// Check for the argument
	if (command->first_child->next_sibling->next_sibling->param == 0)
		return (0);
	path = command->first_child->next_sibling->next_sibling->first_child->value;
	// if (!path)
	// 	return (0); //what to return here?
	if (chdir(path) != 0)
	{
		perror("cd");
		err = errno;
		return (err);
	}
	// Get PWD
	buffer = getcwd(NULL, 0);
	if (buffer == NULL)
	{
		perror("getcwd error");
		err = errno;
		return (errno);
	}
	// Update the PWD environment variable
	upd_envvar("PWD", buffer, *env_lst);
	//ft_export_node(env_lst, buffer);
	
	free(buffer);
	return (err);
}

// 1 if it is a builtin, 0 if not
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
		return (1); // Command is a built-in
	}
	return (0); // Command is not a built-in
}

// returns 0 or err code or -1 if 
int	builtiner(t_ast_node *command, t_env **env_list)
{
	char	*exec;
	int exit_code;

	exit_code = 0;
	if (command == NULL || command->first_child == NULL
    || command->first_child->next_sibling == NULL)
		return (1);
	exec = command->first_child->next_sibling->value;
	if (!exec || !*exec)
		return (1);
	//only return 0 in case of success
	if (ft_strcmp(exec, "echo") == 0)
		exit_code = ft_echo(command);
	else if (ft_strcmp(exec, "cd") == 0) //???? TODO should work with relative and abs path
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
	{
		exit_code = print_env(env_list);
	}
	return (exit_code);
}