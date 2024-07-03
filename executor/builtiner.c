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

int ft_pwd()
{
	size_t size = 1024;
    char *buffer = malloc(size);

    if (buffer == NULL) {
        perror("Unable to allocate buffer with malloc");
        exit(EXIT_FAILURE);;
    }

    // Get the current working directory
    if (getcwd(buffer, size) == NULL) {
        perror("getcwd error");
        free(buffer);
		exit(EXIT_FAILURE);
	}
	ft_putendl_fd(buffer, STDOUT_FILENO);
	free(buffer);
    return(1); //if not infinite loop in builtiner
}


//1 is a builtin, 0 is not
int is_builtin(t_ast_node *command) {
    char *exec = command->first_child->next_sibling->value;
    if (!exec)
    {
        return 0;
    }
    if (ft_strcmp(exec, "echo") == 0 ||
        ft_strcmp(exec, "cd") == 0 ||
        ft_strcmp(exec, "pwd") == 0 ||
        ft_strcmp(exec, "export") == 0 ||
        ft_strcmp(exec, "unset") == 0 ||
        ft_strcmp(exec, "env") == 0)
        {
            return 1; // Command is a built-in
        }
    return 0; // Command is not a built-in
}

//returns 0 if command is a builtin and 1 if it is not
int builtiner(t_ast_node *command, t_env **env_list) {
    if (command == NULL || command->first_child == NULL || command->first_child->next_sibling == NULL) {
        return -1; // Error code indicating invalid command structure
    }
    
    (void)env_list;
    char *exec = command->first_child->next_sibling->value;

    if (!exec || !*exec) {
        return -1;
    }

    if (ft_strcmp(exec, "echo") == 0) {
        ft_echo(command); // YES
    } else if (ft_strcmp(exec, "cd") == 0) {
        write(1,"my CD ", 6); // Handle cd command
    } else if (ft_strcmp(exec, "pwd") == 0) {
        write(1,"my PWD ", 7);
        ft_pwd(); // YES
    } else if (ft_strcmp(exec, "export") == 0) {
        ft_export(env_list, command); // YES
    } else if (ft_strcmp(exec, "unset") == 0) {
        ft_unset(env_list, command); // Handle unset command
    } else if (ft_strcmp(exec, "env") == 0) {
        write(1,"my env ", 7);
        print_env(env_list); // YES
    } else {
        return 1; // Command is not a built-in
    }
    return 0; // Command is a built-in
}