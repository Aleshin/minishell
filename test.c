#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
    
    (void)argc;
    (void)argv;

    char *args[] = {"env", NULL};

    

    // Execute env command with modified environment variables
    if (execve("/usr/bin/env", args, envp) == -1) {
        perror("execve");
        return 1;
    }

    // This line will only be reached if execve fails
    printf("This won't be printed if execve succeeds.\n");

    return 0;
}