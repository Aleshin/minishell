#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ft_find_abs_path(char *command)
{
    char *path = getenv("PATH");
    char **arr = ft_split(path, ':');
    char *path_to_command = NULL;
    int i = 0;
	while (arr[i])
	{
		char *tmp = ft_strjoin(arr[i], "/", command);
		if (access(tmp, F_OK | X_OK) != -1) // R_OK W_OK rite write permision
        {
            path_to_command = tmp;
            break;
        }
        free(tmp);
		i++;
	}
	free(arr);
    return path_to_command;
}

typedef struct s_command {
    char *cmd;
    struct s_command *next;
} t_command;

// Function to free the memory allocated for the linked list of commands
void free_commands(t_command *head) {
    while (head != NULL) {
        t_command *temp = head;
        head = head->next;
        free(temp->cmd);
        free(temp);
    }
}

void ft_pipes(int num_pipes, char **commands) {
    int pipe_fds[num_pipes][2];
    pid_t pids[num_pipes + 1];
    
    // Create pipes
    for (int i = 0; i < num_pipes; i++) {
        if (pipe(pipe_fds[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Fork processes
    for (int i = 0; i < num_pipes + 1; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {
            // Child process
            if (i != 0) {
                dup2(pipe_fds[i - 1][0], STDIN_FILENO);
            }

            if (i != num_pipes) {
                dup2(pipe_fds[i][1], STDOUT_FILENO);
            }

            // Close all pipe file descriptors in child process
            for (int j = 0; j < num_pipes; j++) {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
            }

            // Tokenize the command and arguments
            char *cmd = strtok(commands[i], " ");
            char *args[100];
            int k = 0;
            while (cmd != NULL) {
                args[k++] = cmd;
                cmd = strtok(NULL, " ");
            }
            args[k] = NULL;

            // Execute the command
            char *path = ft_find_abs_path(args[0]);
            if (path != NULL) {
                if (execve(path, args, NULL) == -1) {
                    perror("execve");
                    free(path);
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Command not found: %s\n", args[0]);
                exit(EXIT_FAILURE);
            }
        }
    }

    // Close all pipe file descriptors in the parent process
    for (int i = 0; i < num_pipes; i++) {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_pipes + 1; i++) {
        waitpid(pids[i], NULL, 0);
    }
}


int main() {
    char *cmd[] = { "ls -la", "grep .c", "wc -l", NULL };
    ft_pipes(2, cmd);
    return 0;
}
