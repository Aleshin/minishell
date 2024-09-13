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

/*function returns NULL if there is no =  and name_val arr 
with valid name and value,
if there is no value it sets val to "" */
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
	return (name_val);
}

//case when no = in assignment (export x)
char	**no_assign_val(char *cur_arg_val, char **new_val)
{
	new_val = (char **)malloc(3 * sizeof(char *));
	if (new_val == NULL)
	{
		perror("Memory allocation failed for new_val");
		return (NULL);
	}
	new_val[0] = ft_strdup(cur_arg_val);
	if (new_val[0] == NULL)
	{
		free(new_val);
		perror("Memory allocation failed for new_val[0]");
		return (NULL);
	}
	new_val[1] = ft_strdup("");
	if (new_val[1] == NULL)
	{
		free(new_val[0]);
		free(new_val);
		perror("Memory allocation failed for new_val[1]");
		return (NULL);
	}
	new_val[2] = NULL;
	return (new_val);
}

int	ft_export(t_env **lst, t_ast_node *command)
{
	t_ast_node	*cur_arg;
	int			err_code;

	err_code = 0;
	cur_arg = command->first_child->next_sibling->next_sibling->first_child;
	if (command->first_child->next_sibling->next_sibling->param == 0)
	{
		ft_print_sorted(*lst);
		return (0);
	}
	else
	{
		while (cur_arg != NULL)
		{
			err_code = ft_export_node(lst, cur_arg->value);
			cur_arg = cur_arg->next_sibling;
		}
	}
	return (err_code);
}

int	ft_export_node(t_env **lst, char *cur_arg_val)
{
	t_env	*new_node;
	char	**new_val;
	int		n;

	n = 1;
	if (check_varname_export(cur_arg_val) == 0)
	{
		ft_env_error("export", cur_arg_val, "not a valid identifier");
		return (1);
	}
	new_node = NULL;
	new_val = get_name_val(cur_arg_val);
	if (new_val == NULL)
	{
		n = 0;
		new_val = no_assign_val(cur_arg_val, new_val);
	}
	if (upd_envvar(new_val[0], new_val[1], *lst) == 0)
	{
		new_node = ft_lstnew_env(new_val[0], new_val[1], n);
		if (new_node == NULL)
			return (free_arr(new_val), 1);
		ft_lstadd_back_env(lst, new_node);
	}
	return (free_arr(new_val), 0);
}
