/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_status.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 19:59:27 by marvin            #+#    #+#             */
/*   Updated: 2024/09/13 19:59:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit_status(pid_t *last_pid)
{
	int	status;

	if (*last_pid != -1)
	{
		while (waitpid(*last_pid, &status, 0) == -1)
		{
			if (errno != EINTR)
			{
				perror("waitpid");
				return (-1);
			}
		}
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WTERMSIG(status))
			return (128 + WTERMSIG(status));
	}
	return (0);
}

void	ft_set_exit_status(t_env **env_list, int *last_pid)
{
	int	last_exit_status;

	last_exit_status = ft_exit_status(last_pid);
	set_exit_code(env_list, last_exit_status);
}

//function to set last exit code. it 
void	set_exit_code(t_env **lst, int code)
{
	char	*key;
	char	*value;
	t_env	*new_node;

	key = "?";
	value = ft_itoa(code);
	if (value == NULL)
	{
		perror("Malloc failed in itoa");
		return ;
	}
	if (!upd_envvar(key, value, *lst))
	{
		new_node = ft_lstnew_env(key, value, 1);
		if (new_node == NULL)
		{
			perror("Memory allocation failed for node");
			free(value);
			return ;
		}
		free(value);
		ft_lstadd_back_env(lst, new_node);
	}
	else
		free(value);
}

int	status_converter(int status)
{
	if (status == 13 || status == 8)
		status = 126;
	else if (status == 2 || status == 14)
		status = 127;
	return (status);
}
