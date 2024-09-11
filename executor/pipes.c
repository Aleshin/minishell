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

void	ft_child_process(int fd_in, int pipefds[], t_ast_node *command,
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
		fd_in = input_fd;
	// Redirect input if fd_in is valid
	if (fd_in != 0)
		handle_dup_and_close(fd_in, STDIN_FILENO);
	// Handle output redirection or pipe
	if (output_fd != -3)
	{
		// Redirect output to the file specified in output_fd
		handle_dup_and_close(output_fd, STDOUT_FILENO);
	}
	else if (command && command->next_sibling != NULL)
	{
		// If there is no output redirection, set up the pipe
		handle_dup_and_close(pipefds[WRITE_END], STDOUT_FILENO);
		close(pipefds[READ_END]); // Close unused read end
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
        {
            perror("dup");
            return (-1);
        }
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

int	ft_exit_status(pid_t last_pid)
{
	int	status;
	int	exit_status;
    // Wait for the last child process and capture its exit status
	if (last_pid != -1)
	{
//if it == -1 there is no child process
		while (waitpid(last_pid, &status, 0) == -1)
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
            //here should retirn 128+signal with WTERMSIG
			exit_status = 128 + WTERMSIG(status);
            //printf("Last command did not exit normally\n");
			return (exit_status); // Indicate abnormal termination
		}
	}
	return (0); // Return -1 if there was no child process,
}

int ft_executor(t_ast_node *ast_tree, t_env **env_list)
{
	t_ast_node  *command;
	int			fd_in;//Initial input file descriptor (stdin)
	int			pipefds[2];//Pipe file descriptors (in and out)
	pid_t		pid;
	pid_t		last_pid;//PID of the last child process
	int			last_exit_status;

	fd_in = 0;
	last_pid = -1;
	command = ast_tree->first_child;
	while (command != NULL)
	{
        // Create pipe only if there is another command after this one
		if (command->next_sibling != NULL)
		{
			if (pipe(pipefds) == -1)
			{
				perror("pipe");
				return (-1);
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (-1);
		}
		if (pid == 0)
		{
            // Child process changes in and out fd accordingly
			ft_child_process(fd_in, pipefds, command, env_list);
		}
		else
		{
        // Parent process
			if (fd_in != 0)
				close(fd_in); // Close the old input fd
			if (command->next_sibling != NULL)
			{
				close(pipefds[WRITE_END]); // Close write end in parent
				fd_in = pipefds[READ_END]; // Save read end for next command's input
			}
			last_pid = pid;
		}
		command = command->next_sibling;
	}
    // Handle the exit status of the last command --->$?
	last_exit_status = ft_exit_status(last_pid);
    //printf("------> last exit status is %d\n", last_exit_status);
	set_exit_code(env_list, last_exit_status);
    // Wait for all other child processes to finish
	while (wait(NULL) > 0);
	return (0);
}
