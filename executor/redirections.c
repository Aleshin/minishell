#include "./minishell.h"

//INPUT REDIRECTIONS
int input_redir(t_ast_node *commands) 
{
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
int output_redir(t_ast_node *commands)
{
    int file = -3;  // Initialize file descriptor to an invalid value
    int flags;
    t_ast_node *redirects = commands->first_child;
    if (redirects == NULL)
        return -3; // No output redirects

    t_ast_node *current_redirect = redirects->first_child;

    while (current_redirect != NULL) {
        if (current_redirect->type == redirect_out || current_redirect->type == redirect_out_add) {
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

void handle_dup_and_close(int old_fd, int new_fd)
{
    if (dup2(old_fd, new_fd) == -1) {
        perror("dup2");
        close(old_fd);
        exit(EXIT_FAILURE);
    }
    close(old_fd);
}