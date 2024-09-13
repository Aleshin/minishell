/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 23:08:49 by emikhayl          #+#    #+#             */
/*   Updated: 2024/07/05 23:08:54 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*ft_find_abs_path(char *command, t_env *env_list)
{
	char	*path;
	char	**arr;
	char	*tmp;
	int		i;

	if (access(command, F_OK) != -1)
		return (command);
	path = ft_getenv(env_list, "PATH");
	if (!path)
		return (NULL);
	arr = ft_split(path, ':');
	i = 0;
	while (arr[i])
	{
		tmp = ft_strjoin(arr[i], "/", command);
		if (access(tmp, F_OK) == 0)
			return (free_arr(arr), tmp);
		free(tmp);
		i++;
	}
	free_arr(arr);
	return (NULL);
}

char	**cmd_to_argv(t_ast_node *cmd)
{
	int		argc;
	int		i;
	char	**argv;

	argc = cmd->next_sibling->param + 1;
	argv = (char **)malloc((argc + 1) * sizeof(char *));
	if (argv == NULL)
	{
		ft_perror("Failed to allocate memory for argv");
		return (NULL);
	}
	i = 0;
	argv[i++] = cmd->value;
	if (argc > 1)
		cmd = cmd->next_sibling->first_child;
	while (i < argc)
	{
		argv[i++] = cmd->value;
		cmd = cmd->next_sibling;
	}
	argv[i] = NULL;
	return (argv);
}

//function that checks path and if it exists execute execve
int	ft_exec_command(t_ast_node *commands, t_env **env_var)
{
	char	*path;
	char	**argv;
	char	**upd_envvar;

	upd_envvar = linked_list_to_envp(env_var);
	if (commands == NULL || commands->first_child == NULL
		|| commands->first_child->next_sibling == NULL)
	{
		free_arr(upd_envvar);
		return (1);
	}
	path = ft_find_abs_path(commands->first_child->next_sibling->value,
			*env_var);
	if (path == NULL)
		ft_shell_error(commands->first_child->next_sibling->value,
			"command not found");
	argv = cmd_to_argv(commands->first_child->next_sibling);
	if (execve(path, argv, upd_envvar) == -1)
	{
		free_arr(argv);
		free_arr(upd_envvar);
		return (errno);
	}
	return (0);
}
