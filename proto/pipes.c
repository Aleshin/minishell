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
#include "proto.h"

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

static void	print_error(char *command) {
    int cmd_len = strlen(command);
  
    // Write "command not found: "
   
    if (write(2, "command not found: ", 19) < 0) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Write the command
    
    if (write(2, command, cmd_len) < 0) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Write newline character
    
    if (write(2, "\n", 1) < 0) {
        perror("write");
        exit(EXIT_FAILURE);
    }
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
	{
		perror("Failed to allocate memory for argv");
		exit(EXIT_FAILURE);
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
	argv[i] = NULL; // Null-terminate the array
	return (argv);
}

// Function to execute commands with or without pipes
void	ft_pipes(t_ast_node *ast_tree)
{
	char **argv;
	t_ast_node	*commands;


	commands = ast_tree->first_child;
    	int pipefds[2];
    	pid_t pid;
    	
	int fd_in = 0; // File descriptor for the input to the current command

	while (commands != NULL) 
	{
		if (pipe(pipefds) < 0) 
		{
            		perror("pipe");
            		exit(EXIT_FAILURE);
        	}

		pid = fork();
        	if (pid < 0) 
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) 
		{
            // Child process

            // Set up input redirection
		if (dup2(fd_in, STDIN_FILENO) < 0) 
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(fd_in); // Close input file descriptor

            // Set up output redirection
		if (commands->next_sibling != NULL) 
		{
			if (dup2(pipefds[1], STDOUT_FILENO) < 0) 
			{
                    		perror("dup2");
                    		exit(EXIT_FAILURE);
                	}
		}
		close(pipefds[0]); // Close read end of pipe

            // Execute the command
		char *path = ft_find_abs_path(commands->first_child->value);
            if (path == NULL) 
			{
                //write custom fprintf!!!!!!
				//printf is not working here
				print_error(commands->first_child->value);
                //fprintf(stderr, "Command not found: %s\n", commands->first_child->value);
                exit(EXIT_FAILURE);
            }
            argv = cmd_to_argv(commands->first_child);
            //execve(path, argv, NULL);
            if (execve(path, argv, NULL) == -1) ///NULL stands for inherit env from the calling process, e.g. minishell
                { 
                    perror("execve");
					free_arr(argv);
					argv = NULL;
                    exit(EXIT_FAILURE);
                }
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
//	if(argv != NULL)
//		free(argv);
}
