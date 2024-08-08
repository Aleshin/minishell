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
//child process does not exit!!!!
void ft_child_process(int fd_in, int pipefds[], t_ast_node *command, t_env **env_list) 
{
    // Save original stdin and stdout file descriptors
    int status = 0;
    int original_stdout = dup(STDOUT_FILENO);
    if (original_stdout == -1)
    {
        perror("dup");
        exit(EXIT_FAILURE);
        //return (-1);
    }
    int original_stdin = dup(STDIN_FILENO);
    if (original_stdin == -1)
    {
        perror("dup");
        exit(EXIT_FAILURE);
        //return (-1);
    }
    int input_fd = input_redir(command);
    int output_fd = output_redir(command);
    // Use input_fd as fd_in if it's not -3
    if (input_fd != -3)
        fd_in = input_fd;
    // Redirect input if fd_in is valid
    if (fd_in != 0)
        handle_dup_and_close(fd_in, STDIN_FILENO);
    // Setup output redirection or pipe
    if (command->next_sibling != NULL)
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
            handle_dup_and_close(output_fd, STDOUT_FILENO);
    }

    // Execute the command (external or builtin)
    if (is_builtin(command))
    {
        status = builtiner(command, env_list);
    } else if (command->first_child->next_sibling != NULL)
        status = ft_exec_command(command, env_list);
    // Restore original stdin and stdout file descriptors
    handle_dup_and_close(original_stdout, STDOUT_FILENO);
    handle_dup_and_close(original_stdin, STDIN_FILENO);
    printf ("Last child status is %d\n", status);
    if (status == 13 || status == 8)  //check this
        status = 126;
    else if (status == 2 || status == 14)
        status = 127;
    exit(status);
    //return (status); // Ensure the child process exits
}


//returns 0 if no builtin or no exec
int ft_handle_builtin(t_ast_node *ast_tree, t_env **env_list)
{
    t_ast_node *command;
    command = ast_tree->first_child;
    int err_code;

    err_code = 0;
    // Check if there is only one command and it is a built-in
    if (command == NULL || command->first_child == NULL || 
        command->first_child->next_sibling == NULL || 
        command->next_sibling != NULL || !is_builtin(command))
    {
        //ft_sintax_error(ast_tree->value);
        return (0); // Not a single built-in command
    }
    // Save the original stdout file descriptor
    int original_stdout = dup(STDOUT_FILENO);
    if (original_stdout == -1) 
    {
        perror("dup");
        return(-1);
    }
    // Handle output redirection
    int out = output_redir(command);
    if (out != -3)
        handle_dup_and_close(out, STDOUT_FILENO);
    // Execute the built-in command
    err_code = builtiner(command, env_list);
    set_exit_code(env_list, err_code);
    // Restore stdout
    handle_dup_and_close(original_stdout, STDOUT_FILENO);
    return (1); // Built-in command was handled
}

int ft_exit_status(pid_t last_pid) 
{
    int status;
    // Wait for the last child process and capture its exit status
    if (last_pid != -1)
    { //if it == -1 there is no child process
        while (waitpid(last_pid, &status, 0) == -1)
        {
            if (errno != EINTR)
            {
                perror("waitpid");
                return(-1);
            }
        }
        // Check if the child process terminated normally
        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
            //printf("Last command exited with status: %d\n", exit_status);
            return (exit_status); // Return the exit status of the last command
        } else
        {
            //printf("Last command did not exit normally\n");
            return -1; // Indicate abnormal termination
        }
    }
    return -1; // Return -1 if there was no child process,
}


int ft_executor(t_ast_node *ast_tree, t_env **env_list) 
{
    t_ast_node *commands;
    int fd_in = 0;  // Initial input file descriptor (stdin)
    int pipefds[2]; // Pipe file descriptors (in and out)
    pid_t pid;
    pid_t last_pid; // PID of the last child process
    int last_exit_status;
  
    last_pid = -1;
    commands = ast_tree->first_child;
    while (commands != NULL)
    { 
        // Create pipe only if there is another command after this one
        if (commands->next_sibling != NULL)
        {
            if (pipe(pipefds) == -1)
            {
                perror("pipe");
                return(-1);
            }
        }
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return(-1);
        }
        if (pid == 0)
        {
            // Child process changes in and out fd accordingly
            ft_child_process(fd_in, pipefds, commands, env_list);
        } 
        else
        {
        // Parent process
            if (fd_in != 0)
            {
                close(fd_in); // Close the old input fd
            }
            if (commands->next_sibling != NULL)
            {
                close(pipefds[WRITE_END]); // Close write end in parent
                fd_in = pipefds[READ_END]; // Save read end for next command's input
            }
            last_pid = pid;     
        }
        commands = commands->next_sibling;
    }
    // Handle the exit status of the last command --->$?
    last_exit_status = ft_exit_status(last_pid);
    printf("------> last exit status is %d\n", last_exit_status);
    set_exit_code(env_list, last_exit_status);
    // Wait for all other child processes to finish
    while (wait(NULL) > 0);
    return (0);
}