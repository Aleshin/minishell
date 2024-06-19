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



void ft_handle_redirection(t_ast_node *redirects) 
{
	int file;
    t_ast_node *current_redirect = redirects->first_child;
	
	if (redirects == 0)
		return;  // No redirections to handle
    
    while (current_redirect != NULL) {
        if (current_redirect->type == redirect_out)  
		{
            //open file write only, create if not exist, if exist truncate
			file = open(current_redirect->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (file == -1)
                ft_perror("Error opening file for output redirection");
			//redirect standart output of command to "file"
            if (dup2(file, STDOUT_FILENO) < 0)
			{
				close(file);
				ft_perror("dup2 output redirection");
			}
                
            close(file);
        } 
		else if (current_redirect->type == redirect_out_add) 
		{
            file = open(current_redirect->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (file == -1)
                ft_perror("Error opening file for append output redirection");
            if (dup2(file, STDOUT_FILENO) < 0)
			{
				close(file);
				ft_perror("dup2 append output redirection");
			} 
            close(file);
        } 
		else if (current_redirect->type == redirect_in) 
		{
            file = open(current_redirect->value, O_RDONLY);
            if (file == -1)
                ft_perror("Error opening file for input redirection");
            if (dup2(file, STDIN_FILENO) < 0)
			{
				close(file);
				ft_perror("dup2 input redirection");
			}
                
            //close(file); //where to close it?????
        }
		else if (current_redirect->type == heredoc)
		{
		// char *tmp = heredoc_stdin(current_redirect->value);
		// printf("TMP %s/n", heredoc_stdin(current_redirect->value));
		// if (tmp == NULL) { 
		// 	ft_perror("Error reading heredoc input");
		// 	// Handle the error appropriately, maybe return or continue
		// }
		file = open(heredoc_stdin(current_redirect->value), O_RDONLY);
		printf("FD %d/n", file);
		if (file == -1) {
			ft_perror("Error opening temporary file for input redirection");
			free(heredoc_stdin(current_redirect->value)); // Free allocated memory for tmp
			return; // or handle error
		}
		if (dup2(file, STDIN_FILENO) < 0)
		{
			close(file);
			ft_perror("dup2 input redirection");
			free(heredoc_stdin(current_redirect->value)); // Free allocated memory for tmp
			return; // or handle error
		}
		close(file);
		unlink(heredoc_stdin(current_redirect->value)); // Delete the temporary file
		free(heredoc_stdin(current_redirect->value));   // Free allocated memory for tmp
		}
        current_redirect = current_redirect->next_sibling;
    }
}

// Function to execute commands with or without pipes
void	ft_executor(t_ast_node *ast_tree, char **envp)
{
	t_ast_node	*commands;

    int pipefds[2];//in and out
    pid_t pid;
    int fd_in; // File descriptor for the input to the current command

	fd_in = 0;
	commands = ast_tree->first_child;
	while (commands != NULL) 
	{
		if (pipe(pipefds) < 0) 
			ft_perror("pipe");
		pid = fork();
        if (pid < 0) 
			ft_perror("fork");
		//child process
		if (pid == 0) 
		{
        	// Set up input redirection
			if (dup2(fd_in, STDIN_FILENO) < 0) ///here the issue, first command is StdIN 
			{
				close(fd_in);
				ft_perror("dup2");
			}	
			close(fd_in); // Close input file descriptor

            // Set up output redirection
			if (commands->next_sibling != NULL) 
			{
				if (dup2(pipefds[1], STDOUT_FILENO) < 0)//redirect output
				{
					close(pipefds[1]);
					ft_perror("dup2");
				}
                    
			}
			close(pipefds[0]); // Close read end of pipe
			
			//here goes redirection
			ft_handle_redirection(commands->first_child);
			// Execute the command
			
			ft_exec_command(commands, envp);
        } 
		else 
		{
            // Parent process
            close(pipefds[1]); // Close write end of pipe
            fd_in = pipefds[0]; // Update input file descriptor for next command
            commands = commands->next_sibling; // Move to the next command
        }
    }
    // Wait for all child processes to finish
    while (wait(NULL) > 0);
}