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

void ft_env(char **envp) {
	int i;
	i = 0;
	while (envp[i] != NULL)
    {
        printf("%s\n", envp[i]);
		i++;
    }
}

int	builtiner(t_ast_node *command, t_env **env_list)
{

	if (ft_strcmp(command->first_child->next_sibling->value, "echo"))
		ft_echo(command); //PROTOTYPE
	else if (ft_strcmp(command->first_child->next_sibling->value, "cd"))
		ft_cd(command); //TO DO
	else if (ft_strcmp(command->first_child->next_sibling->value, "pwd"))
		ft_pwd(); //PROTOTYPE
	else if (ft_strcmp(command->first_child->next_sibling->value, "export"))
		ft_cd(command); //TO DO
	else if (ft_strcmp(command->first_child->next_sibling->value, "unset"))
		ft_cd(command);//remove_node(lst, argv[1]);
	else if (ft_strcmp(command->first_child->next_sibling->value, "env"))
		print_env(env_list);//in process maybe with &
	else if (ft_strcmp(command->first_child->next_sibling->value, "exit"))
		ft_cd(command);//TO DO
	else
		return (1);

	return (0);
}
