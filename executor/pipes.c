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

//command is ast_tree->first_child
//int fd_in, int pipefds[]
void	ft_child_process(t_pipe *pipes, t_ast_node *command,
	t_env **env_list)
{
	int	status;
	int	input_fd;
	int	output_fd;

	status = 0;
	input_fd = input_redir(command);
	output_fd = output_redir(command);
	// Use input_fd as fd_in if it's not -3
	if (input_fd != -3)
		pipes->fd_in = input_fd;
	// Redirect input if fd_in is valid
	if (pipes->fd_in != 0)
		handle_dup_and_close(pipes->fd_in, STDIN_FILENO);
	// Handle output redirection or pipe
	if (output_fd != -3)
	{
		// Redirect output to the file specified in output_fd
		handle_dup_and_close(output_fd, STDOUT_FILENO);
	}
	else if (command && command->next_sibling != NULL)
	{
		// If there is no output redirection, set up the pipe
		handle_dup_and_close(pipes->pipefds[WRITE_END], STDOUT_FILENO);
		close(pipes->pipefds[READ_END]); // Close unused read end
	}
	// Execute the command (external or builtin)
	if (command->first_child && command->first_child->next_sibling)
	{
		if (is_builtin(command))
			status = ft_exec_builtin(command, env_list);
		else
			status = ft_exec_command(command, env_list);
	}
	// Handle exit status and terminate child process
	if (status == 13 || status == 8)
		status = 126;
	else if (status == 2 || status == 14)
		status = 127;
	exit(status);
}

void	ft_parent_process(t_pipe *pipes, t_ast_node *command)
{
    // Close the old input fd if it's not stdin
	if (pipes->fd_in != 0)
		close(pipes->fd_in);
    // If there's a next command, set up the pipe
	if (command->next_sibling != NULL)
	{
		close(pipes->pipefds[WRITE_END]);// Close the write end in the parent
		pipes->fd_in = pipes->pipefds[READ_END];  // Save the read end for the next command's input
	}
	pipes->last_pid = pipes->pid;  // Store the last process ID
}

//returns 0 if no builtin
int ft_handle_builtin(t_ast_node *ast_tree, t_env **env_list)
{
    t_ast_node  *command;
    int         err_code;
    int         original_stdout;
    int         out;

    command = ast_tree->first_child;
    err_code = 0;
    // Check if there is only one command and it is a built-in
    if (command->next_sibling == NULL && command->first_child->next_sibling != NULL && is_builtin(command) == 1)
    {
	original_stdout = dup(STDOUT_FILENO);
	if (original_stdout == -1)
		return (perror("dup"), -1);
        // Handle output redirection
	out = output_redir(command);
	if (out != -3)
		handle_dup_and_close(out, STDOUT_FILENO);
        // Execute the built-in command
	err_code = ft_exec_builtin(command, env_list);
	set_exit_code(env_list, err_code);
        // Restore stdout
	handle_dup_and_close(original_stdout, STDOUT_FILENO);
	return (1);
	}
	return (0);
}

int	ft_exit_status(pid_t *last_pid)
{
	int	status;
	int	exit_status;
    // Wait for the last child process and capture its exit status
	if (*last_pid != -1)
	{
//if it == -1 there is no child process
		while (waitpid(*last_pid, &status, 0) == -1)
		{
			if (errno != EINTR)
			{
				perror("waitpid");
				return (-1);
			}
		}
        // Check if the child process terminated normally
		if (WIFEXITED(status))
		{
			exit_status = WEXITSTATUS(status);
			return (exit_status);
		}
		else if (WTERMSIG(status))
		{
			exit_status = 128 + WTERMSIG(status);
			return (exit_status); // Indicate abnormal termination
		}
	}
	return (0); // Return -1 if there was no child process,
}

int	ft_executor(t_ast_node *ast_tree, t_env **env_list)
{
	t_pipe	pipes;
	int		last_exit_status;

	pipes.fd_in = 0;
	pipes.last_pid = -1;

	while (ast_tree->first_child != NULL)
	{
		if (ast_tree->first_child->next_sibling != NULL)
		{
			if (pipe(pipes.pipefds) == -1)
				return (perror("pipe"), -1);
		}
		pipes.pid = fork();
		if (pipes.pid == -1)
			return (perror("fork"), -1);
		if (pipes.pid == 0)
			ft_child_process(&pipes, ast_tree->first_child, env_list);
		else
			ft_parent_process(&pipes, ast_tree->first_child);
		ast_tree->first_child = ast_tree->first_child->next_sibling;
	}
	last_exit_status = ft_exit_status(&pipes.last_pid);
	set_exit_code(env_list, last_exit_status);
	while (wait(NULL) > 0)
		;
	return (0);
}
