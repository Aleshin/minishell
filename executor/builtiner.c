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

int	ft_pwd(void)
{
	char	*buffer;

	buffer = NULL;
	// Get the current working directory
	buffer = getcwd(NULL, 0);
	if (buffer == NULL)
	{
		perror("getcwd error");
		return (-1);
	}
	// Output the current working directory
	ft_putendl_fd(buffer, STDOUT_FILENO);
	// Free the allocated buffer by getcwd
	free(buffer);
	return (0);
}

int	ft_cd(t_env **env_lst, t_ast_node *command)
{
	size_t	size;
	char	*buffer;
	char	*path;

	size = 1024;
	buffer = malloc(size);
	// Check for the argument
	if (command->first_child->next_sibling->next_sibling->param == 0)
		return (0);
	path = command->first_child->next_sibling->next_sibling->first_child->value;
	if (!path)
		return (0);
	if (chdir(path) == -1)
	{
		perror("cd");
		free(buffer);
		return (-1);
	}
	// Get PWD
	if (getcwd(buffer, size) == NULL)
	{
		perror("getcwd error");
		free(buffer);
		return (-1);
	}
	// Update the PWD environment variable
	ft_export_node(env_lst, buffer);
	free(buffer);
	return (0);
}

// 1 if it is a builtin, 0 if not
int	is_builtin(t_ast_node *command)
{
	char	*exec;

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

// returns 0 if command is a builtin and 1 if it is not
int	builtiner(t_ast_node *command, t_env **env_list)
{
	char	*exec;

	if (command == NULL || command->first_child == NULL
    || command->first_child->next_sibling == NULL)
		return (-1);
	exec = command->first_child->next_sibling->value;
	if (!exec || !*exec)
		return (-1);
	if (ft_strcmp(exec, "echo") == 0)
		ft_echo(command);
	else if (ft_strcmp(exec, "cd") == 0)
		//printf("Here goes cd command\n");
        ft_cd(env_list, command);
	else if (ft_strcmp(exec, "pwd") == 0)
	{
		ft_pwd();
	}
	else if (ft_strcmp(exec, "export") == 0)
		ft_export(env_list, command);
	else if (ft_strcmp(exec, "unset") == 0)
		ft_unset(env_list, command);
	else if (ft_strcmp(exec, "env") == 0)
	{
		print_env(env_list);
	}
	else
		return (1);
	return (0);
}