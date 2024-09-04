/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 21:39:48 by saleshin          #+#    #+#             */
/*   Updated: 2024/06/12 21:38:25 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

//command is ast_tree->first_child
void	ft_child_process(int fd_in, int pipefds[], t_ast_node *command,
	t_env **env_list)
{
	int	status;
	int	original_stdout;
	int	original_stdin;
	int	input_fd;
	int	output_fd;

	status = 0;
	original_stdout = dup(STDOUT_FILENO);
	if (original_stdout == -1)
	{
		perror("dup");
		exit(EXIT_FAILURE);
	}
	original_stdin = dup(STDIN_FILENO);
	if (original_stdin == -1)
	{
		perror("dup");
		exit(EXIT_FAILURE);
	}
	input_fd = input_redir(command);
	output_fd = output_redir(command);
    // Use input_fd as fd_in if it's not -3
	if (input_fd != -3)
		fd_in = input_fd;
    // Redirect input if fd_in is valid
	if (fd_in != 0)
		handle_dup_and_close(fd_in, STDIN_FILENO);
    // Setup output redirection or pipe
	if (command && command->next_sibling != NULL)
	{
		// Duplicate write end to stdout
		handle_dup_and_close(pipefds[WRITE_END], STDOUT_FILENO);
		close(pipefds[READ_END]); // Close unused read end
	}
	else
	{
		// This is the last command in the pipeline
        // Redirect output to the file specified in output_fd
		if (output_fd != -3)
		{
			handle_dup_and_close(output_fd, STDOUT_FILENO);
		}
	}
	// Execute the command (external or builtin)
	//TO DO HERE TO CHECK IF THE COMMAND IS ABSOLUTE PATH
	//command is ast_tree.first_child is command 1
	//command->first_child->next_sibling is EXEC
	    // Ensure command and its members are valid before use
    if (command && command->first_child && command->first_child->next_sibling) {
        if (is_builtin(command->first_child->next_sibling->value)) {
            status = builtiner(command, env_list);
        } else if (command->first_child->next_sibling->value != NULL && 
                   (command->first_child->next_sibling->value[0] != '\0' || 
                    command->first_child->param != 0)) {
            status = ft_exec_command(command, env_list);
        }
    } else {
        //ft_shell_error(command->first_child->next_sibling->value, "command not found");
		//fprintf(stderr, "No valid command found\n");
        status = 127; // or any other error code you use to indicate failure
		exit(EXIT_FAILURE);
	}
	// if (is_builtin(command))
	// {
	// 	status = builtiner(command, env_list);
	// }
	// else if (command->first_child->next_sibling != NULL
	// 		|| (command->first_child->next_sibling->value[0] == '\0'
	// 		&& command->first_child->param != 0) )
	// 	status = ft_exec_command(command, env_list);
    // Restore original stdin and stdout file descriptors
	handle_dup_and_close(original_stdout, STDOUT_FILENO);
	handle_dup_and_close(original_stdin, STDIN_FILENO);
	printf ("Last child status is %d\n", status);
	if (status == 13 || status == 8)
		status = 126;
	else if (status == 2 || status == 14)
	{
		ft_shell_error(command->value, "command not exist");
		status = 127;
	}
		
	exit(status);
    //return (status); // Ensure the child process exits
}

//returns 0 if no builtin || there is pipes (>1 cmd)
//ast_tree exists, exec/args might not
int	ft_handle_builtin(t_ast_node *ast_tree, t_env **env_list)
{
	int			err_code;
	int			outfile;
	int			infile;

	err_code = 0;
    // executes only if there is only one command and it is a built-in
	//or if there is no commands 
	//ast_tree.first_child.first_child.next_sibling - EXEC NODE
	// exec = command->first_child->next_sibling->value;
	//ast_tree.first_child.next_sibling - next command
	//condition 1 is: it is only one exec, it has value and it is a builtin
	//condition 2: no exec and yes redirects
	//ast_tree->first_child->first_child->param > 0
	//condition 3:return only if 0 if no exec no redirects, >1 commands
	if (ast_tree->first_child->next_sibling != NULL || ( !is_builtin(ast_tree->first_child->value)))
	{
		return (0); // Not a single built-in command and we go to ft_exec
	}
  
    // Handle output redirection
	outfile = output_redir(ast_tree->first_child);
	if (outfile != -3) //no redir
		handle_dup_and_close(outfile, STDOUT_FILENO);
	infile = input_redir(ast_tree->first_child);
	if (infile != -3) //no redir
		handle_dup_and_close(infile, STDIN_FILENO);
    // Execute the built-in command
	err_code = builtiner(ast_tree->first_child, env_list);
	set_exit_code(env_list, err_code);
	return (1);
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
		else
		{
            //printf("Last command did not exit normally\n");
			return (-1); // Indicate abnormal termination
		}
	}
	return (-1); // Return -1 if there was no child process,
}


int	ft_executor(t_ast_node *ast_tree, t_env **env_list)
{
	int		fd_in;// Initial input file descriptor (stdin)
	int		pipefds[2];// Pipe file descriptors (in and out)
	pid_t		pid;
	pid_t		last_pid; // PID of the last child process
	int		last_exit_status;

	fd_in = 0;
	last_pid = -1;

	//ast_tree.first_child.first_child.next_sibling - EXEC
	
	//can recieve from handle_builtin this: EXEC == NULL
	//   ast_tree->first_child->first_child->next_sibling == NULL
	//to check this not here
	// if (ast_tree->first_child->first_child->next_sibling == NULL && ast_tree->first_child->first_child->param == 0)
	// {
	// 	set_exit_code(env_list, 127);
	// 	return (-1);
	// }

	while (ast_tree->first_child != NULL)
	{
		// Create pipe only if there is another command after this one
		if (ast_tree->first_child->next_sibling != NULL)
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
			ft_child_process(fd_in, pipefds, ast_tree->first_child, env_list);
		}
		else
		{
        // Parent process
			if (fd_in != 0)
			{
				close(fd_in); // Close the old input fd
			}
			if (ast_tree->first_child->next_sibling != NULL)
			{
				close(pipefds[WRITE_END]); // Close write end in parent
				fd_in = pipefds[READ_END]; // Save read end for next command's input
			}
			last_pid = pid;     
 		}
		ast_tree->first_child = ast_tree->first_child->next_sibling;
	}
    // Handle the exit status of the last command --->$?
	last_exit_status = ft_exit_status(last_pid);
	//printf("------> last exit status is %d\n", last_exit_status);
	printf("LST EXIT STATUS IS %d\n", last_exit_status);
	set_exit_code(env_list, last_exit_status);
    // Wait for all other child processes to finish
	while (wait(NULL) > 0);
	return (0);
}
