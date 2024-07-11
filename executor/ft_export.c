/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 23:09:47 by emikhayl          #+#    #+#             */
/*   Updated: 2024/07/05 23:09:50 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int check_env_exist(char *name, char *value, t_env *lst) // 1 if exists, 0 if not
{
    t_env *curr;

    curr = lst;
    while (curr != NULL)
    {
        if (!ft_strcmp(name, curr->name))
        {
            free(curr->value);
            curr->value = ft_strdup(value);
            if (curr->value == NULL)
            {
                perror("Memory allocation failed");
                return (-1);
            }
            return (1);
        }
        curr = curr->next;
    }
    return (0);
}


int ft_export_node(t_env **lst, char *cur_arg)
{
    t_env *new_node;
    char **new_val;
    int env_exist_result;

    new_node = NULL;
    new_val = NULL;

    new_val = ft_split_global(cur_arg, '=');

    if (!new_val)
    {
        if (!check_varname(cur_arg))
        {
            ft_env_error("export", cur_arg, "not a valid identifier");
            return (1);
        }
        return (0);
    }

    if (!check_varname(new_val[0]))
    {
        ft_env_error("export", cur_arg, "not a valid identifier");
        free(new_val[0]);  // Free allocated memory
        free(new_val[1]);
        free(new_val);
        return (1);
    }

    // If no value is provided, set the value to an empty string
    if (new_val[1] == NULL)
    {
        new_val[1] = ft_strdup("");
        if (!new_val[1])
        {
            perror("Memory allocation failed for value");
            free(new_val[0]);  // Free allocated memory
            free(new_val);
            return (1);
        }
    }

    env_exist_result = check_env_exist(new_val[0], new_val[1], *lst);

    if (env_exist_result == 0)
    {
        new_node = ft_lstnew_env(new_val[0], new_val[1]);
        if (new_node == NULL)
        {
            perror("Memory allocation failed for node");
            free(new_val[0]);  // Free allocated memory
            free(new_val[1]);
            free(new_val);
            return (1);
        }
        ft_lstadd_back_env(lst, new_node);
    }
    else if (env_exist_result == -1)
    {
        // Memory allocation error inside check_env_exist
        free(new_val[0]);  // Free allocated memory
        free(new_val[1]);
        free(new_val);
        return (1);
    }
    free(new_val[0]);  // Free allocated memory
    free(new_val[1]);
    free(new_val);
    return (0);
}

int ft_export(t_env **lst, t_ast_node *command)
{
    t_ast_node *cur_arg;

    cur_arg = command->first_child->next_sibling->next_sibling->first_child;
    //printf("export %p\n", *lst);
    
    if (command->first_child->next_sibling->next_sibling->param == 0)
    {
        ft_print_sorted(*lst);
        return (0);
    }
    else
    {        
        while(cur_arg != NULL) 
        {
            ft_export_node(lst, cur_arg->value);
            cur_arg = cur_arg->next_sibling;
        }
    }
    return (1);
}
