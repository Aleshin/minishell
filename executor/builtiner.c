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

// 5 ft
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

// Updated ft_unset function
int	ft_unset(t_env **list, t_ast_node *command)
{
	t_ast_node	*cur_arg;

	cur_arg = command->first_child->next_sibling->next_sibling->first_child;
	while (cur_arg != NULL)
	{
		remove_node(list, cur_arg->value);
		cur_arg = cur_arg->next_sibling;
	}
	return (0);
}

// 1 if it is a builtin, 0 if not
//HERE TO CHECK IF EXEC IS A VALID BUILTIN
int	is_builtin(t_ast_node *command)
{
	char	*exec;

	if (!command->first_child->next_sibling)
		return (0);
	exec = command->first_child->next_sibling->value;
	if (ft_strcmp(exec, "echo") == 0
		|| ft_strcmp(exec, "cd") == 0
		|| ft_strcmp(exec, "pwd") == 0
		|| ft_strcmp(exec, "export") == 0
		|| ft_strcmp(exec, "unset") == 0
		|| ft_strcmp(exec, "env") == 0
		|| ft_strcmp(exec, "exit") == 0)
	{
		return (1);
	}
	return (0);
}

//when we get there it is a builtin. return exit code of execution
int	ft_exec_builtin(t_ast_node *command, t_env **env_list)
{
	int		exit_code;
	char	*exec;

	exit_code = 0;
	exec = command->first_child->next_sibling->value;
	if (ft_strcmp(exec, "echo") == 0)
		exit_code = ft_echo(command);
	else if (ft_strcmp(exec, "cd") == 0)
		exit_code = ft_cd(env_list, command);
	else if (ft_strcmp(exec, "pwd") == 0)
		exit_code = ft_pwd();
	else if (ft_strcmp(exec, "export") == 0)
		exit_code = ft_export(env_list, command);
	else if (ft_strcmp(exec, "unset") == 0)
		exit_code = ft_unset(env_list, command);
	else if (ft_strcmp(exec, "env") == 0)
		exit_code = print_env(env_list);
	else if (ft_strcmp(exec, "exit") == 0)
		exit_code = ft_exit(command);
	return (exit_code);
}
