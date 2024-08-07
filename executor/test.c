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


int ft_exec_command(t_ast_node *commands, t_env **env_var)
{
    char *path;
    char **argv;
    char **upd_envvar;
    // Convert the environment list to an array of strings
    upd_envvar = linked_list_to_envp(env_var);

    // Check for invalid command structure
    if (commands == NULL || commands->first_child == NULL
        || commands->first_child->next_sibling == NULL)
    {
        free_arr(upd_envvar);
        return(1);
    }

    // Find the absolute path of the command
    path = ft_find_abs_path(commands->first_child->next_sibling->value, *env_var);

    // Convert command arguments
    argv = cmd_to_argv(commands->first_child->next_sibling);
	
    // Execute the command
    if (execve(path, argv, upd_envvar) == -1)
    {
        // execve failed; print error
		perror("execve");
        printf("errno number: %d\n", errno);
        printf("errno message: %s\n", strerror(errno));
        
        // Clean up and set exit code
        free_arr(argv);
        free_arr(upd_envvar);
        set_exit_code(env_var, 126); // Command not executable
        return (errno);
    }
}