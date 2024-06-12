#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//*************compile "gcc -Wall -Wextra -Werror -I ./libft -o pipes pipes.c ./libft/libft.a" ***//

typedef struct s_cmd {
    char *exec;
    char *arg1;
    char *arg2;
    char *arg3;
    struct s_cmd *next;
} t_cmd;// Function to find the absolute path of a command

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

char *ft_find_abs_path(char *command) 
{
    char *path = getenv("PATH");
    char **arr = ft_split(path, ':');
    if (!arr)
        return NULL;

    char *path_to_command = NULL;
    int i = 0;
    while (arr[i]) {
        char *tmp = ft_strjoin(arr[i], "/", command);
        if (access(tmp, F_OK | X_OK) != -1) 
        {
            path_to_command = tmp;
            break;
        }
        free(tmp);
        i++;
    }
    free_arr(arr);
    return path_to_command;
}

//to convert node to array for execve function
char **cmd_to_argv(t_cmd *cmd) {
    // Calculate the number of non-NULL arguments
    int argc = 1; // Start with 1 for the exec
    if (cmd->arg1 != NULL) 
        argc++;
    if (cmd->arg2 != NULL) 
        argc++;
    if (cmd->arg3 != NULL) 
        argc++;

    // Allocate memory for the argv array (argc + 1 for the NULL terminator)
    char **argv = (char **)malloc((argc + 1) * sizeof(char *));
    if (argv == NULL) {
        perror("Failed to allocate memory for argv");
        exit(EXIT_FAILURE);
    }

    // Copy the exec and arguments to the argv array
    argv[0] = strdup(cmd->exec);
    if (argv[0] == NULL) {
        perror("Failed to allocate memory for exec");
        free_arr(argv);
        exit(EXIT_FAILURE);
    }

    int i = 1;
    if (cmd->arg1 != NULL) {
        argv[i] = strdup(cmd->arg1);
        if (argv[i++] == NULL) {
            perror("Failed to allocate memory for arg1");
            free(argv[0]);
            free_arr(argv);
            exit(EXIT_FAILURE);
        }
    }
    if (cmd->arg2 != NULL) {
        argv[i] = strdup(cmd->arg2);
        if (argv[i++] == NULL) {
            perror("Failed to allocate memory for arg2");
            for (int j = 0; j < i - 1; j++) {
                free(argv[j]);
            }
            free_arr(argv);
            exit(EXIT_FAILURE);
        }
    }
    if (cmd->arg3 != NULL) {
        argv[i] = strdup(cmd->arg3);
        if (argv[i++] == NULL) {
            perror("Failed to allocate memory for arg3");
            for (int j = 0; j < i - 1; j++) {
                free(argv[j]);
            }
            free_arr(argv);
            exit(EXIT_FAILURE);
        }
    }

    argv[i] = NULL; // Null-terminate the array

    return argv;
}


// Function to execute commands with or without pipes
void ft_pipes(t_cmd *commands) {
    int pipefds[2];
    pid_t pid;
    int fd_in = 0; // File descriptor for the input to the current command

    while (commands != NULL) {
        if (pipe(pipefds) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process

            // Set up input redirection
            if (dup2(fd_in, STDIN_FILENO) < 0) {
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            close(fd_in); // Close input file descriptor

            // Set up output redirection
            if (commands->next != NULL) {
                if (dup2(pipefds[1], STDOUT_FILENO) < 0) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }
            close(pipefds[0]); // Close read end of pipe
            close(pipefds[1]); // Close write end of pipe after duplicating it

            // Execute the command
            char *path = ft_find_abs_path(commands->exec);
            if (path == NULL) {
                
                fprintf(stderr, "Command not found: %s\n", commands->exec);
                exit(EXIT_FAILURE);
            }
            char **argv = cmd_to_argv(commands);
            if (execve(path, argv, NULL) == -1) {
                perror("execve");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            close(pipefds[1]); // Close write end of pipe
            fd_in = pipefds[0]; // Update input file descriptor for next command
            commands = commands->next; // Move to the next command
        }
    }

    // Close the remaining read end of the pipe in the parent process
    close(fd_in);

    // Wait for all child processes to finish
    while (wait(NULL) > 0);
}

// Helper function to create and append a new node to the list
void append(t_cmd **head, char *exec, char *arg1, char *arg2, char *arg3) {
    t_cmd *new_node = (t_cmd *)malloc(sizeof(t_cmd));
    if (new_node == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    new_node->exec = strdup(exec);
    if (new_node->exec == NULL) {
        perror("Failed to allocate memory for exec");
        free(new_node);
        exit(EXIT_FAILURE);
    }

    if (arg1 != NULL) {
        new_node->arg1 = strdup(arg1);
        if (new_node->arg1 == NULL) {
            perror("Failed to allocate memory for arg1");
            free(new_node->exec);
            free(new_node);
            exit(EXIT_FAILURE);
        }
    } else {
        new_node->arg1 = NULL;
    }

    if (arg2 != NULL) {
        new_node->arg2 = strdup(arg2);
        if (new_node->arg2 == NULL) {
            perror("Failed to allocate memory for arg2");
            free(new_node->arg1);
            free(new_node->exec);
            free(new_node);
            exit(EXIT_FAILURE);
        }
    } else {
        new_node->arg2 = NULL;
    }

    if (arg3 != NULL) {
        new_node->arg3 = strdup(arg3);
        if (new_node->arg3 == NULL) {
            perror("Failed to allocate memory for arg3");
            free(new_node->arg2);
            free(new_node->arg1);
            free(new_node->exec);
            free(new_node);
            exit(EXIT_FAILURE);
        }
    } else {
        new_node->arg3 = NULL;
    }

    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        t_cmd *last = *head;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = new_node;
    }
}

// Main function to test the implementation
int main()
{
    //TO TEST append any commands max 3 arg.
    t_cmd *head = NULL; 
    append(&head, "cati", "pipes.c", NULL, NULL);
    append(&head, "grep", ".c", NULL, NULL);
    append(&head, "wci", "-l", NULL, NULL);    


    ft_pipes(head);    
    
    // Free the list
    t_cmd *current = head;
    current = head;
    while (current != NULL) 
    {
        t_cmd *next = current->next;
        free(current->exec);
        if (current->arg1) 
            free(current->arg1);
        if (current->arg2) 
            free(current->arg2);
        if (current->arg3) 
            free(current->arg3);
        free(current);
        current = next;
    }    
    return 0;
}
