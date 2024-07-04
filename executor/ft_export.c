#include "minishell.h"

t_env	*ft_lstnew_env(char *name, char *value)
{
	t_env	*new;

    new = malloc(sizeof(t_env));
    if (new == NULL)
		return (NULL);
	new->name = ft_strdup(name);
    if (new->name == NULL) 
    {
        free(new); // Free allocated t_env structure
        return (NULL);
    }
    new->value = ft_strdup(value);
	if (new->name == NULL) 
    {
        free(new->name);
        free(new); // Free allocated t_env structure
        
        return (NULL);
    }
    new->next = NULL;
	return (new);
}

void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*list;

	if (*lst == NULL)
		*lst = new;
	else
	{
		list = *lst;
		while (list->next != NULL)
		{
			list = list->next;
		}
		list->next = new;
	}
}

//export IN PROGRESS
//export without args same as env but alphabetically.
//export

int check_env_exist(char *arg, t_env *lst) //1 si, 0 no
{
    t_env *curr;

    curr = lst;

    while (curr != NULL)
    {
        if (!ft_strcmp(arg, curr->name))
        {
            free(curr->value);
            curr->value = ft_strdup(arg);
            return (1);
        }
        curr = curr->next;
    }
    return (0);
}

int ft_export_noarg(t_env **lst, t_ast_node *command)
{
    
    
    if (command->first_child->next_sibling->next_sibling->param == 0)
    {
        ft_print_sorted(*lst);
        return (0);
    }
    return (1);
}

int ft_export(t_env **lst, t_ast_node *command)
{
    // t_env *new_node;
    char *arg;
    // char *cmd;
    char **new_val;

    // cmd = command->first_child->next_sibling->value;
    arg = NULL; // arg
    // new_node = NULL;
    new_val = NULL;

    if (ft_export_noarg(lst, command) != 0)
    {
        arg = command->first_child->next_sibling->next_sibling->first_child->value;
        new_val = split_env(arg, '=');
        if (new_val)
            printf("name %s, val %s\n", new_val[0], new_val[1]);
        
        // if (new_val == NULL)
        // {
        //     return (0);
        // }

        // if (!check_varname(new_val[0]))
        // {
        //     ft_env_error(cmd, arg, "not a valid identifier");
        //     return (1);
        // }

        // // If no value is provided, set the value to an empty string
        // if (new_val[1] == NULL)
        // {
        //     new_val[1] = ft_strdup("");
        //     if (new_val[1] == NULL)
        //     {
        //         ft_env_error(cmd, arg, "memory allocation failed");
        //         return (1);
        //     }
        // }

        // if (!check_env_exist(new_val[0], *lst))
        // {
        //     new_node = ft_lstnew_env(new_val[0], new_val[1]);
        //     if (new_node == NULL)
        //     {
        //         perror("Memory allocation failed for node");
        //         free_env_node(new_node);
        //         return (1);
        //     }
            
        //     ft_lstadd_back_env(lst, new_node);
        // }
        return (0);
    }
    return (0);
}