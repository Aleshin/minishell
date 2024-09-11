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

t_env	*ft_lstnew_env(char *name, char *value, int n)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (new == NULL)
		return (NULL);
	new->name = ft_strdup(name);
	if (new->name == NULL)
	{
		free(new);
		return (NULL);
	}
	new->value = ft_strdup(value);
	if (new->value == NULL)
	{
		//new->value = ft_strdup("");
		free(new->name);
		free(new);
		return (NULL);
	}
	new->to_env = n;
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

//check if name exists and if exists only changes value
int	upd_envvar(char *name, char *value, t_env *lst)
{
	t_env	*curr;

	curr = lst;
	if (!curr)
		return (-1);
	while (curr != NULL)
	{
		if (!ft_strcmp(name, curr->name))//return 0 if equal
		{
			free(curr->value);
			curr->to_env = 1;
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

//function returns NULL if there is no = 
//and name_val arr with valid name and value,
//if there is no value it sets val to ""
char	**get_name_val(char *cur_arg)
{
	char	**name_val;

	name_val = ft_split_global(cur_arg, '=');
	if (!name_val)
	{
        return (NULL);
    }
	if (name_val[1] == NULL)
	{
		name_val[1] = ft_strdup("");
		if (!name_val[1])
		{
			perror("Memory allocation failed for value");
			free(name_val[0]);
			free(name_val);
			return (NULL);
		}
	}
	return (name_val); //NULL if no =
}

char	**no_assign_val(char *cur_arg_val, char **new_val)
{
	// Allocate memory for new_val to hold two pointers (for name and value)
	new_val = (char **)malloc(3 * sizeof(char *));
	if (new_val == NULL)
	{
		perror("Memory allocation failed for new_val");
		return (NULL); // Return NULL if memory allocation fails
	}

	// Allocate and assign the name string (new_val[0])
	new_val[0] = ft_strdup(cur_arg_val);
	if (new_val[0] == NULL)
	{
		free(new_val); // Free new_val if allocation fails
		perror("Memory allocation failed for new_val[0]");
		return (NULL);
	}

	// Assign an empty string as the value (new_val[1])
	new_val[1] = ft_strdup("");
	if (new_val[1] == NULL)
	{
		free(new_val[0]); // Free previously allocated memory
		free(new_val);
		perror("Memory allocation failed for new_val[1]");
		return (NULL);
	}
	new_val[2] = NULL;

	// Successfully created new_val
	return new_val;
}

int	ft_export_node(t_env **lst, char *cur_arg_val)
{
	t_env	*new_node;
	char	**new_val;
	int		n;

	n=1;
	if (check_varname(cur_arg_val, 1) == 0)
	{
		ft_env_error("export", cur_arg_val, "not a valid identifier");
		return (1);
	}
	new_node = NULL;
	new_val = get_name_val(cur_arg_val);
	//if no =
	if (new_val == NULL)
	{
		n = 0;
		new_val = no_assign_val(cur_arg_val, new_val);
	}		
	//no need to update, create new var
	if (upd_envvar(new_val[0], new_val[1], *lst) == 0)
	{
		new_node = ft_lstnew_env(new_val[0], new_val[1], n);
		if (new_node == NULL)
		{
			perror("Memory allocation failed for node");
			free_arr(new_val);
			return (1);
		}
		ft_lstadd_back_env(lst, new_node);
	}
	free_arr(new_val);
	return (0);
}

int	ft_export(t_env **lst, t_ast_node *command)
{
	t_ast_node	*cur_arg;
	int	err_code;

	err_code = 0;
	cur_arg = command->first_child->next_sibling->next_sibling->first_child;
	if (command->first_child->next_sibling->next_sibling->param == 0) //no arg
	{
		ft_print_sorted(*lst);//not print if flag 0
		return (0);
	}
	else
	{
		while (cur_arg != NULL)
		{
			//printf("value is %s\n", cur_arg->value);
			err_code = ft_export_node(lst, cur_arg->value);
			cur_arg = cur_arg->next_sibling;
		}
	}
	return (err_code);
}
