#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void execute_command(void) {
    char *path = "/bin/lr";                // Path to the shell
    char *argv[] = {"/bin/lr", "-la", NULL};  // Arguments to pass to the shell
    char *envp[] = {NULL};                // Use default environment variables
    
    // Execute the command
    if (execve(path, argv, envp) == -1) {
        perror("execve failed");
        printf("errno %d", errno);
        exit(EXIT_FAILURE);  // Ensure the child process exits if execve fails
    }
}

int main(void) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        execute_command();
    } else {
        // Parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            printf("Exit status of the child was %d\n", exit_status);
        } else if (WIFSIGNALED(status)) {
            int signal_num = WTERMSIG(status);
            printf("Child terminated by signal %d, exit code: %d\n", signal_num, 128 + signal_num);
        }
    }

    return 0;
}