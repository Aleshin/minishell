#include "minishell.h"

void free_arr(char **arr)
{
    int i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

// Function to find the absolute path of a command
char	*ft_find_abs_path(char *command, t_env *env_list)
{
	char	*path;
	char	**arr;
	char	*path_to_command;
	int	i;
	char	*tmp;

	path = ft_getenv(env_list, "PATH");//
	arr = ft_split(path, ':');
	path_to_command = NULL;
	i = 0;
	while (arr[i])
	{
		tmp = ft_strjoin(arr[i], "/", command);
		if (access(tmp, F_OK | X_OK) != -1)
		{	
			path_to_command = tmp;
			break ;
		}
		free(tmp);
		i++;
	}
	free_arr(arr);
	return (path_to_command);
}

char	**cmd_to_argv(t_ast_node *cmd) //"exec" node inside "command" node
{
	int	argc;
	int	i;
	char	**argv;

	argc = cmd->next_sibling->param + 1; // receive args num from "args" node
	argv = (char **)malloc((argc + 1) * sizeof(char *)); //allocate memory for 2d-array
	if (argv == NULL)
		ft_perror("Failed to allocate memory for argv");
	
	i = 0;
	argv[i++] = cmd->value;
	if (argc > 1)
		cmd = cmd->next_sibling->first_child;
	while (i < argc)
	{
		argv[i++] = cmd->value;
		cmd = cmd->next_sibling;
	}
	argv[i] = NULL; // Null-terminate the array
	return (argv);
}

//function that checks path and if it exists execute execve
void ft_exec_command(t_ast_node *commands, t_env **env_var)
{
    char *path;
    char **argv;
    char **upd_envvar = linked_list_to_envp(env_var);

    // Check for invalid command structure
    if (commands == NULL || commands->first_child == NULL || commands->first_child->next_sibling == NULL) {
        //exit(EXIT_FAILURE);
        return ;
    }

    // Handle external commands
    path = ft_find_abs_path(commands->first_child->next_sibling->value, *env_var);
    if (path == NULL) {
        ft_shell_error(commands->first_child->next_sibling->value, "command not found");
        exit(EXIT_FAILURE);
    }

    argv = cmd_to_argv(commands->first_child->next_sibling);

    // Execute the command using execve
    if (execve(path, argv, upd_envvar) == -1) {
        perror("execve");
        free_arr(argv);
        free_arr(upd_envvar);
        exit(EXIT_FAILURE);
    }

    free_arr(argv);
    free_arr(upd_envvar);
}