/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtiner.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 19:06:39 by saleshin          #+#    #+#             */
/*   Updated: 2024/06/24 19:06:49 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int ft_echo(t_ast_node *command)
{
    t_ast_node *exec = command->first_child->next_sibling;
    t_ast_node *num_args = exec->next_sibling;
    t_ast_node *args;
    //printf("exec is %s , num args is %d\n", exec->value, num_args->param);

    if (exec != NULL && num_args->param < 1) {
        printf("command \"%s\"\n", exec->value);
    } else if (exec != NULL && num_args->param > 0)
    {
        args = command->first_child->next_sibling->next_sibling->first_child;
        printf("command \"%s\" ", exec->value);
        while(args != NULL)
        {
            printf("%s ", args->value);
            if (args->next_sibling == NULL)
                printf("\n");
            args = args->next_sibling;
        }
    }

    return 0; // Return success code
}

int	ft_cd(t_ast_node *command)
{
	printf("command \"%s\"\n", command->first_child->next_sibling->value);
	return (0);
}

void ft_pwd()
{
	size_t size = 1024;
    char *buffer = malloc(size);

    if (buffer == NULL) {
        perror("Unable to allocate buffer");
        exit(EXIT_FAILURE);
    }

    // Get the current working directory
    if (getcwd(buffer, size) == NULL) {
        perror("getcwd error");
        free(buffer);
		exit(EXIT_FAILURE);
	}
	printf("MY PWD PRINTS THIS %s\n", buffer);
	free(buffer);
}


//1 is a builtin, 0 is not
int is_builtin(t_ast_node *command) {
    char *exec = command->first_child->next_sibling->value;
    if (!*exec)
    {
        perror("no exec");
        return -2;//error no exec
    }
    if (ft_strcmp(exec, "echo") == 0 ||
        ft_strcmp(exec, "cd") == 0 ||
        ft_strcmp(exec, "pwd") == 0 ||
        ft_strcmp(exec, "export") == 0 ||
        ft_strcmp(exec, "unset") == 0 ||
        ft_strcmp(exec, "env") == 0 ||
        ft_strcmp(exec, "exit") == 0) 
        {
            return 1; // Command is a built-in
        }
    return 0; // Command is not a built-in
}

//returns 0 if command is a builtin and 1 if it is not
int builtiner(t_ast_node *command, t_env **env_list) {
    
    (void)env_list;
    char *exec = command->first_child->next_sibling->value;
    
    if (ft_strcmp(exec, "echo") == 0) {
        ft_echo(command); // Handle echo command
    } else if (ft_strcmp(exec, "cd") == 0) {
        ft_echo(command); // Handle cd command
    } else if (ft_strcmp(exec, "pwd") == 0) {
        ft_pwd(); // Handle pwd command
    } else if (ft_strcmp(exec, "export") == 0) {
        ft_echo(command); // Handle export command
    } else if (ft_strcmp(exec, "unset") == 0) {
        ft_echo(command);; // Handle unset command
    } else if (ft_strcmp(exec, "env") == 0) {
        ft_echo(command); // Handle env command
    } else if (ft_strcmp(exec, "exit") == 0) {
        ft_echo(command); // Handle exit command
    } else {
        return 1; // Command is not a built-in
    }
    return 0; // Command is a built-in
}

// //void ft_handle_builtin(t_ast_node *command, t_env **env_list) 
// void ft_handle_builtin(t_ast_node *command, t_env **env_list) 
// {
//     // Handle input redirection
//     int input_fd = input_redir(command);
//     if (input_fd == -1) {
//         perror("Error in input_redir");
//         exit(EXIT_FAILURE);
//     } else if (input_fd != -3) { // Check if input redirection is needed
//         if (dup2(input_fd, STDIN_FILENO) == -1) {
//             perror("dup2 input");
//             exit(EXIT_FAILURE);
//         }
//         close(input_fd); // Close original input_fd after successful dup2
//     }
    
//     // Handle output redirection
//     int output_fd = output_redir(command);
//     if (output_fd == -1) {
//         perror("Error in output_redir");
//         exit(EXIT_FAILURE);
//     } else if (output_fd != -3) { // Check if output redirection is needed
//         if (dup2(output_fd, STDOUT_FILENO) == -1) {
//             perror("dup2 output");
//             exit(EXIT_FAILURE);
//         }
//         close(output_fd); // Close original output_fd after successful dup2
//     }

//     // Execute the built-in command
//     builtiner(command, env_list);
// }


