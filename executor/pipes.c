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

static void free_arr(char **arr)
{
    int i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

void ft_perror(char *str)
{
	perror(str);
    exit(EXIT_FAILURE);
}

static void	print_error(char *command) 
{
    int cmd_len = ft_strlen(command);
  
    // Write "command not found: "
    if (write(2, "command not found: ", 19) < 0) 
        ft_perror("write");
    // Write the command
    if (write(2, command, cmd_len) < 0)
        ft_perror("write");
    // Write newline character
    if (write(2, "\n", 1) < 0)
        ft_perror("write");
}

// Function to find the absolute path of a command
char	*ft_find_abs_path(char *command)
{
	char	*path;
	char	**arr;
	char	*path_to_command;
	int	i;
	char	*tmp;

	path = getenv("PATH");
	arr = ft_split(path, ':');
	path_to_command = NULL;
	i = 0;
	while (arr[i])
	{
		tmp = ft_strjoin(arr[i], "/", command);
		if (access(tmp, F_OK | X_OK) != -1)
		{	
			path_to_command = tmp;
			break ;
		}
		free(tmp);
		i++;
	}
	free_arr(arr);
	return (path_to_command);
}

char	**cmd_to_argv(t_ast_node *cmd) //"exec" node inside "command" node
{
	int	argc;
	int	i;
	char	**argv;

	argc = cmd->next_sibling->param + 1; // receive args num from "args" node
	argv = (char **)malloc((argc + 1) * sizeof(char *)); //allocate memory for 2d-array
	if (argv == NULL)
		ft_perror("Failed to allocate memory for argv");
	
	i = 0;
	argv[i++] = cmd->value;
	if (argc > 1)
		cmd = cmd->next_sibling->first_child;
	while (i < argc)
	{
		argv[i++] = cmd->value;
		cmd = cmd->next_sibling;
	}
	argv[i] = NULL; // Null-terminate the array
	return (argv);
}
//function that checks path and if it exists execute execve
void ft_exec_command(t_ast_node	*commands, char **envp)
{
	char *path;
	char **argv;
	
	if (commands == NULL || commands->first_child == NULL || commands->first_child->next_sibling == NULL) 
        exit(EXIT_FAILURE);

	path = ft_find_abs_path(commands->first_child->next_sibling->value);
    if (path == NULL) 
	{
		print_error(commands->first_child->next_sibling->value);
        //fprintf(stderr, "Command not found: %s\n", commands->first_child->value);
        exit(EXIT_FAILURE);
    }
	argv = cmd_to_argv(commands->first_child->next_sibling);
	
	//execve(path, argv, env  var);
	if (execve(path, argv, envp) == -1) ///NULL stands for inherit env from the calling process, e.g. minishell
    { 
		perror("execve");
		free_arr(argv);
		argv = NULL;
        exit(EXIT_FAILURE);
    }

}

void write_string_to_file(const char *filename, const char *content) {
    // Open the file (create if it doesn't exist, truncate if it does)
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    // Write the content to the file
    ssize_t bytes_written = write(fd, content, sizeof(char) * ft_strlen(content));
    if (bytes_written == -1) {
        perror("Error writing to file");
        close(fd);
        return;
    }

    // Close the file
    if (close(fd) == -1) {
        perror("Error closing file");
    } else {
        printf("Content written to file successfully.\n");
    }
}


void ft_child_process(t_ast_node *commands, char **envp, int *pipefds, int fd_in) {
    close(pipefds[0]); // Close read end of the pipe in the child process
    int outfile = -1; // Initialize outfile file descriptor
    int infile = -1;  // Initialize infile file descriptor

    t_ast_node *redirects = commands->first_child;
    t_ast_node *current_redirect = redirects->first_child;

    if (redirects == NULL)
        return;  // No redirections to handle

    while (current_redirect != NULL) {
        if (current_redirect->type == redirect_out) {
            // Close the previously opened file descriptor
            if (outfile != -1) {
                close(outfile);
            }

            // Open file write only, create if not exist, if exist truncate
            outfile = open(current_redirect->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (outfile == -1) {
                ft_perror("Error opening file for output redirection");
            }

            // Redirect stdout (file descriptor 1) to outfile
            if (dup2(outfile, STDOUT_FILENO) < 0) {
                close(outfile);
                ft_perror("dup2 output redirection");
            }

            // Close outfile after redirection
            close(outfile);
        } else if (current_redirect->type == redirect_out_add) {
            // Close the previously opened file descriptor
            if (outfile != -1) {
                close(outfile);
            }

            // Open file write only, create if not exist, if exist append
            outfile = open(current_redirect->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (outfile == -1) {
                ft_perror("Error opening file for append output redirection");
            }

            // Redirect stdout (file descriptor 1) to outfile
            if (dup2(outfile, STDOUT_FILENO) < 0) {
                close(outfile);
                ft_perror("dup2 append output redirection");
            }

            // Close outfile after redirection
            close(outfile);
        } else if (current_redirect->type == redirect_in) {
            // Close the previously opened file descriptor
            if (infile != -1) {
                close(infile);
            }

            // Open file read only
            infile = open(current_redirect->value, O_RDONLY);
            if (infile == -1) {
                ft_perror("Error opening file for input redirection");
            }

            // Redirect stdin (file descriptor 0) to infile
            if (dup2(infile, STDIN_FILENO) < 0) {
                close(infile);
                ft_perror("dup2 input redirection");
            }

            // Close infile after redirection
            close(infile);
        }

        current_redirect = current_redirect->next_sibling;
    }

    // Redirect stdin (file descriptor 0) if necessary (for input redirections)
    if (fd_in != 0) {
        if (dup2(fd_in, STDIN_FILENO) < 0) {
            close(fd_in);
            ft_perror("dup2 input redirection");
        }
        close(fd_in); // Close input file descriptor after redirection
    }

    // Execute the command
    ft_exec_command(commands, envp);
    exit(EXIT_SUCCESS); // Exit the child process after command execution
}

void ft_executor(t_ast_node *ast_tree, char **envp) {
    t_ast_node *commands;
    int pipefds[2]; // Pipe file descriptors (in and out)
    int fd_in = 0;  // Initial input file descriptor (stdin)

    commands = ast_tree->first_child;
    while (commands != NULL) {
        // Create a pipe if there is a next command to connect with a pipe
        if (commands->next_sibling != NULL && pipe(pipefds) < 0) {
            ft_perror("pipe");
        }

        pid_t pid = fork();
        if (pid < 0) {
            ft_perror("fork");
        }

        if (pid == 0) {
            // Child process
            close(pipefds[0]); // Close read end of the pipe in the child process
            ft_child_process(commands, envp, pipefds, fd_in);
            exit(EXIT_SUCCESS); // Exit the child process after command execution
        } else {
            // Parent process
            close(pipefds[1]); // Close write end of the pipe in the parent process
            fd_in = pipefds[0]; // Update input file descriptor for the next command
            commands = commands->next_sibling; // Move to the next command
        }
    }

    // Wait for all child processes to finish
    while (wait(NULL) > 0);
}