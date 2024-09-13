/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 21:39:48 by saleshin          #+#    #+#             */
/*   Updated: 2024/09/11 20:50:21 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

void	ft_child_process(t_pipe *pipes, t_ast_node *command,
	t_env **env_list)
{
	int	status;

	status = 0;
	if (input_redir(command) != -3)
		pipes->fd_in = input_redir(command);
	if (pipes->fd_in != 0)
		handle_dup_and_close(pipes->fd_in, STDIN_FILENO);
	if (output_redir(command) != -3)
		handle_dup_and_close(output_redir(command), STDOUT_FILENO);
	else if (command && command->next_sibling != NULL)
	{
		handle_dup_and_close(pipes->pipefds[WRITE_END], STDOUT_FILENO);
		close(pipes->pipefds[READ_END]);
	}
	if (command->first_child && command->first_child->next_sibling)
	{
		if (is_builtin(command))
			status = ft_exec_builtin(command, env_list);
		else
			status = ft_exec_command(command, env_list);
	}
	status = status_converter(status);
	exit(status);
}

//returns 0 if no builtin
int	ft_handle_builtin(t_ast_node *ast_tree, t_env **env_list)
{
	t_ast_node	*command;
	int			err_code;
	int			original_stdout;
	int			out;

	command = ast_tree->first_child;
	err_code = 0;
	if (command->next_sibling == NULL && command->first_child->next_sibling
		!= NULL && is_builtin(command) == 1)
	{
		original_stdout = dup(STDOUT_FILENO);
		if (original_stdout == -1)
		{
			perror("dup");
			return (-1);
		}
		out = output_redir(command);
		if (out != -3)
			handle_dup_and_close(out, STDOUT_FILENO);
		err_code = ft_exec_builtin(command, env_list);
		set_exit_code(env_list, err_code);
		handle_dup_and_close(original_stdout, STDOUT_FILENO);
		return (1);
	}
	return (0);
}

void	ft_parent_process(t_pipe *pipes, t_ast_node *command)
{
	if (pipes->fd_in != 0)
		close(pipes->fd_in);
	if (command->next_sibling != NULL)
	{
		close(pipes->pipefds[WRITE_END]);
		pipes->fd_in = pipes->pipefds[READ_END];
	}
	pipes->last_pid = pipes->pid;
}

int	ft_create_pipe_if_needed(t_pipe *pipes, t_ast_node *command)
{
	if (command->next_sibling != NULL)
	{
		if (pipe(pipes->pipefds) == -1)
		{
			perror("pipe");
			return (-1);
		}
	}
	return (0);
}

int	ft_executor(t_ast_node *ast_tree, t_env **env_list)
{
	t_pipe		pipes;
	t_ast_node	*command;

	pipes.fd_in = 0;
	pipes.last_pid = -1;
	command = ast_tree->first_child;
	while (command != NULL)
	{
		if (ft_create_pipe_if_needed(&pipes, command) == -1)
			return (-1);
		pipes.pid = fork();
		if (pipes.pid == -1)
			return (perror("fork"), -1);
		if (pipes.pid == 0)
			ft_child_process(&pipes, command, env_list);
		else
			ft_parent_process(&pipes, command);
		command = command->next_sibling;
	}
	ft_set_exit_status(env_list, &pipes.last_pid);
	while (wait(NULL) > 0)
		;
	return (0);
}
