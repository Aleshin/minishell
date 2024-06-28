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
	printf("command \"%s\" output \"%s\"\n", command->first_child->next_sibling->value, command->first_child->next_sibling->next_sibling->first_child->value);
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

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;
	int	n;

	i = 0;
	n = ft_strlen(s2);
	if (ft_strlen(s1) > ft_strlen(s2))
		n = ft_strlen(s1);
	while ((*s1 || *s2) && i < n)
	{
		if (*s1 != *s2)
			return (0);
		s1++;
		s2++;
		i++;
	}
	return (1);
}

int is_builtin(char *command) {
    if (ft_strncmp(command, "echo", 4) == 0 ||
        ft_strncmp(command, "cd", 2) == 0 ||
        ft_strncmp(command, "pwd", 3) == 0 ||
        ft_strncmp(command, "export", 6) == 0 ||
        ft_strncmp(command, "unset", 5) == 0 ||
        ft_strncmp(command, "env", 3) == 0 ||
        ft_strncmp(command, "exit", 4) == 0) {
        return 1; // Command is a built-in
    }
    return 0; // Command is not a built-in
}

int builtiner(t_ast_node *command, t_env **env_list) {
    char *cmd = command->first_child->next_sibling->value;

    if (ft_strncmp(cmd, "echo", 4) == 0) {
        ft_echo(command); // Handle echo command
    } else if (ft_strncmp(cmd, "cd", 2) == 0) {
        ft_cd(command); // Handle cd command
    } else if (ft_strncmp(cmd, "pwd", 3) == 0) {
        ft_pwd(); // Handle pwd command
    } else if (ft_strncmp(cmd, "export", 6) == 0) {
        ft_echo(command); // Handle export command
    } else if (ft_strncmp(cmd, "unset", 5) == 0) {
        remove_node(env_list, cmd); // Handle unset command
    } else if (ft_strncmp(cmd, "env", 3) == 0) {
        print_env(env_list); // Handle env command
    } else if (ft_strncmp(cmd, "exit", 4) == 0) {
        ft_echo(command); // Handle exit command
    } else {
        return 1; // Command is not a built-in
    }
    return 0; // Command is a built-in
}


