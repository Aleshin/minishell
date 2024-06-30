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

int	ft_echo(t_ast_node *command)
{
	printf("command \"%s\"\n", command->first_child->next_sibling->value);
	return (0);
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
	printf("MY PWD PRINTS THIS %s/n", buffer);
	free(buffer);
}

int is_builtin(char *command) {
    if (ft_strcmp(command, "echo") == 0 ||
        ft_strcmp(command, "cd") == 0 ||
        ft_strcmp(command, "pwd") == 0 ||
        ft_strcmp(command, "export") == 0 ||
        ft_strcmp(command, "unset") == 0 ||
        ft_strcmp(command, "env") == 0 ||
        ft_strcmp(command, "exit") == 0) {
        return 1; // Command is a built-in
    }
    return 0; // Command is not a built-in
}

int builtiner(t_ast_node *command, t_env **env_list) {
    char *cmd = command->first_child->next_sibling->value;

    if (ft_strcmp(cmd, "echo") == 0) {
        ft_echo(command); // Handle echo command
    } else if (ft_strcmp(cmd, "cd") == 0) {
        ft_cd(command); // Handle cd command
    } else if (ft_strcmp(cmd, "pwd") == 0) {
        ft_pwd(); // Handle pwd command
    } else if (ft_strcmp(cmd, "export") == 0) {
        ft_echo(command); // Handle export command
    } else if (ft_strcmp(cmd, "unset") == 0) {
        remove_node(env_list, cmd); // Handle unset command
    } else if (ft_strcmp(cmd, "env") == 0) {
        print_env(env_list); // Handle env command
    } else if (ft_strcmp(cmd, "exit") == 0) {
        ft_echo(command); // Handle exit command
    } else {
        return 1; // Command is not a built-in
    }
    return 0; // Command is a built-in
}


