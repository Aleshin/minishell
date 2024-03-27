/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 22:31:31 by saleshin          #+#    #+#             */
/*   Updated: 2024/03/22 22:31:38 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"



char *ft_find_abs_path(char *command)
{
    char *path = getenv("PATH");
    char **arr = ft_split(path, ':');
    char *path_to_command = NULL;
    int i = 0;
	while (arr[i])
	{
		char *tmp = ft_strjoin(arr[i], "/", command);
		if (access(tmp, F_OK | X_OK) != -1) 
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


int main (int argc, char **argv, char **envp)
{
char *buf;
    int id;
    (void)argc;
    (void)argv;
    (void)envp;
    while (1) {
        buf = readline("$> "); // Prompt for input command
        if (buf == NULL || strcmp(buf, "exit") == 0) {
            // If user enters exit or closes input (Ctrl+D), exit the loop
            free(buf);
            break;
        }
        // Split buf into command and arguments
        char *command = strtok(buf, " ");
        char **args = (char **)malloc(sizeof(char *) * 2); // assuming maximum 1 argument
        args[0] = command;
        args[1] = strtok(NULL, " ");
        args[2] = NULL; // NULL terminate the array
        id = fork();
        if (id == -1) {
            // Fork failed
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (id == 0) {
            // Child process
            char *path = ft_find_abs_path(command);
            if (path != NULL) {
                if (execve(path, args, NULL) == -1) {
                    perror("execve");
                    exit(EXIT_FAILURE);
                }
                free(path); // Free the memory allocated for path
            } else {
                printf("Command not found: %s\n", command);
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            int status;
            waitpid(id, &status, 0); // Wait for child process to finish
            if (WIFEXITED(status)) {
                printf("Child process exited with status: %d\n", WEXITSTATUS(status));
            }
        }
        free(args); // Free the memory allocated for args
        free(buf);  // Free the memory allocated for buf
    }
    return 0;
}