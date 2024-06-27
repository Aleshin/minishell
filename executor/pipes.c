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

void free_arr(char **arr)
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
void ft_exec_command(t_ast_node	*commands, t_env **env_var)
{
	char *path;
	char **argv;
    char **upd_envvar = linked_list_to_envp(env_var);
	
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
	if (execve(path, argv, upd_envvar) == -1) ///NULL stands for inherit env from the calling process, e.g. minishell
    { 
		perror("execve");
		free_arr(argv);
        free_arr(upd_envvar);
		argv = NULL;
        exit(EXIT_FAILURE);
    }
}

void handle_dup_and_close(int old_fd, int new_fd) {
    if (dup2(old_fd, new_fd) == -1) {
        perror("dup2");
        close(old_fd);
        exit(EXIT_FAILURE);
    }
    close(old_fd);
}

//INPUT REDIRECTIONS
int input_redir(t_ast_node *commands) {
    int file = -3;  // Initialize file descriptor
    t_ast_node *redirects = commands->first_child;
    if (redirects == 0)
        return -3; //no input redirects
    t_ast_node *current_redirect = redirects->first_child;

    // Traverse the list of redirects
    while (current_redirect != NULL) {
        if (current_redirect->type == redirect_in) 
        {
            if (file != -3)
                close(file);
            file = open(current_redirect->value, O_RDONLY);
            if (file == -1) {
                ft_perror("Error opening file for input redirection");
                exit(EXIT_FAILURE);
            }
        }
        else if (current_redirect->type == heredoc)
        {
            int pipefd[2];
            if (pipe(pipefd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }

            // Write heredoc content to the pipe
            ssize_t n = write(pipefd[WRITE_END], current_redirect->value, ft_strlen(current_redirect->value));
            if (n == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }

            close(pipefd[WRITE_END]);  // Close writing end of the pipe after writing all data

            // Use reading end of the pipe as input file descriptor
            file = pipefd[READ_END];
        }
        current_redirect = current_redirect->next_sibling;
    }
    return file;
}

//OUTPUT REDIRECTIONS
int output_redir(t_ast_node *commands) {
    int file = -3;  // Initialize file descriptor to an invalid value
    int flags;
    t_ast_node *redirects = commands->first_child;
    if (redirects == NULL)
        return -3; // No output redirects

    t_ast_node *current_redirect = redirects->first_child;

    while (current_redirect != NULL) {
        if (current_redirect->type == redirect_out || current_redirect->type == redirect_out_add) 
        {
            if (file != -3) {
                close(file); // Close previous file descriptor if any
            }

            // Determine flags based on the type of output redirection
            flags = O_WRONLY | O_CREAT;
            if (current_redirect->type == redirect_out_add) {
                flags |= O_APPEND; // Append mode for >>
            } else {
                flags |= O_TRUNC; // Truncate mode for >
            }

            // Open the file with determined flags and permissions
            file = open(current_redirect->value, flags, 0644);
            if (file == -1) {
                perror("Error opening file for output redirection");
                exit(EXIT_FAILURE);
            }   
        }
        current_redirect = current_redirect->next_sibling;
    }
    return file;
}


void ft_child_process(int fd_in, int pipefds[], t_ast_node *command, t_env **env_list) 
{
    // Handle input redirection
    int input_fd = input_redir(command); 

    // Handle output redirection
    int output_fd = output_redir(command);

    // Use input_fd as fd_in if it's not -3
    if (input_fd != -3)
        fd_in = input_fd;
    
    // Use output_fd as fd_in if it's not -3
    if (output_fd != -3)
        pipefds[WRITE_END] = output_fd;

    // Redirect input if fd_in is valid
    if (fd_in != 0)
        handle_dup_and_close(fd_in, STDIN_FILENO);

    // Setup output redirection or pipe
    if (command->next_sibling != NULL) {
        // Duplicate write end to stdout
        handle_dup_and_close(pipefds[WRITE_END], STDOUT_FILENO);
        
        close(pipefds[READ_END]); // Close unused read end
    } else {
        // This is the last command in the pipeline
        // Redirect output to the file specified in output_fd
        if (output_fd != -3) 
            handle_dup_and_close(output_fd, STDOUT_FILENO);
    }
    ft_exec_command(command, env_list);
    perror("execvp");
    exit(EXIT_FAILURE);
}

void ft_executor(t_ast_node *ast_tree, t_env **env_list) 
{
    t_ast_node *commands; // Commands list
    int fd_in = 0;  // Initial input file descriptor (stdin)
    int pipefds[2]; // Pipe file descriptors (in and out)
    pid_t pid;
    pid_t last_pid = -1; // PID of the last child process
    int status;

    commands = ast_tree->first_child;

    while (commands != NULL) {
        // Create pipe only if there is another command after this one
        if (commands->next_sibling != NULL) {
            if (pipe(pipefds) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        if (builtiner(commands, env_list) == 0)
        {
            if(commands->next_sibling != NULL)
            {
                //close write end in parent
                close(pipefds[WRITE_END]);
                fd_in = pipefds[READ_END];
            }
            commands = commands->next_sibling;
        }
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            // Child process changes in and out fd accordingly
            ft_child_process(fd_in, pipefds, commands, env_list);
        } else {
            // Parent process
            if (fd_in != 0) {
                close(fd_in); // Close the old input fd
            }
            if (commands->next_sibling != NULL) {
                close(pipefds[WRITE_END]); // Close write end in parent
                fd_in = pipefds[READ_END]; // Save read end for next command's input
            }
            last_pid = pid;
            // Move to the next command
            commands = commands->next_sibling;
        }
    }

// Wait for all child processes and capture the exit status of the last one
    while (waitpid(last_pid, &status, 0) == -1) {
        if (errno != EINTR) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
    }
    //checs if the child process terminated normally
    if (WIFEXITED(status)) {
        int exit_status = WEXITSTATUS(status);
        printf("Last command exited with status: %d\n", exit_status);
    } else {
        printf("Last command did not exit normally\n");
    }

    // Wait for all other child processes to finish
    while (wait(NULL) > 0);
}
