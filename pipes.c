#include "minishell.h"

typedef struct s_command {
    char *cmd;
    struct s_command *next;
} t_command;



// // Function to append a command to the linked list
// void append_command(t_command **head, const char *cmd) {
//     t_command *new_command = (t_command *)malloc(sizeof(t_command));
//     if (!new_command) {
//         perror("malloc");
//         exit(EXIT_FAILURE);
//     }
//     new_command->cmd = ft_strdup(cmd);
//     if (!new_command->cmd) {
//         perror("strdup");
//         free(new_command);
//         exit(EXIT_FAILURE);
//     }
//     new_command->next = NULL;

//     if (*head == NULL) {
//         *head = new_command;
//     } else {
//         t_command *current = *head;
//         while (current->next != NULL) {
//             current = current->next;
//         }
//         current->next = new_command;
//     }
// }

// Function to free the memory allocated for the linked list of commands
void free_commands(t_command *head) {
    while (head != NULL) {
        t_command *temp = head;
        head = head->next;
        free(temp->cmd);
        free(temp);
    }
}

void ft_pipes(int num_pipes, t_command *commands) {
    int pipe_fds[num_pipes - 1][2];
    int pids[num_pipes];

    // Create pipes
    for (int i = 0; i < num_pipes - 1; i++) {
        if (pipe(pipe_fds[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Fork processes
    for (int i = 0; i < num_pipes; i++) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {
            // Child process
            if (i != 0) {
                dup2(pipe_fds[i - 1][0], STDIN_FILENO);
            }

            if (i != num_pipes - 1) {
                dup2(pipe_fds[i][1], STDOUT_FILENO);
            }

            // Close all pipe file descriptors in child process
            for (int j = 0; j < num_pipes - 1; j++) {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
            }

            // Tokenize the command and arguments
            char *cmd = strtok(commands->cmd, " ");
            char *args[100];
            int k = 0;
            while (cmd != NULL) {
                args[k++] = cmd;
                cmd = strtok(NULL, " ");
            }
            args[k] = NULL;

            // Execute the command
            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }

        // Move to the next command
        commands = commands->next;
    }

    // Close all pipe file descriptors in the parent process
    for (int i = 0; i < num_pipes - 1; i++) {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_pipes; i++) {
        waitpid(pids[i], NULL, 0);
    }
}

int main() {
    // Define commands
    // t_command cmd1 = { "ls -l", NULL };
    // t_command cmd2 = { "grep .c", NULL };
    // t_command cmd3 = { "wc -l", NULL };
    t_command cmd1;
    t_command cmd2;

    cmd1.cmd = malloc(strlen("ls -l") + 1); // Allocate memory for the string
    if (cmd1.cmd == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(cmd1.cmd, "ls -l");
    
    cmd2.cmd = malloc(strlen("grep .c") + 1); // Allocate memory for the string
    if (cmd2.cmd == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(cmd2.cmd, "grep .c");
    
    // cmd3.cmd = malloc(strlen("wc -l") + 1); // Allocate memory for the string
    // if (cmd3.cmd == NULL) {
    //     perror("malloc");
    //     exit(EXIT_FAILURE);
    // }
    // strcpy(cmd3.cmd, "wc -l");
    


    // Link commands into a linked list
    cmd1.next = &cmd2;
    cmd2.next = NULL;
    // cmd2.next = &cmd3;
    // cmd3.next = NULL;

    printf("%s %s", cmd1.cmd, cmd2.cmd);
    // Execute the pipeline
    //ft_pipes(3, &cmd1);
    free(cmd1.cmd);
    free(cmd2.cmd);
   


    return 0;
}
