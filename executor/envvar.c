/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envvar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 23:07:15 by emikhayl          #+#    #+#             */
/*   Updated: 2024/08/08 22:52:52 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_env(t_env **env)
{
	t_env	*curr;

	curr = *env;
	//printf ("Head in env is %s\n", curr->name);
	printf ("Puntero en print env is %p\n", *env);
	while (curr != NULL)
	{
		if (ft_strcmp(curr->name, "?") != 0)
		{
			printf("%s=%s\n", curr->name, curr->value);
		}
		curr = curr->next;
	}
	return (0);
}

// void remove_node(t_env **lst, char *name)
// {
//     if (*lst == NULL)
//         return;

//     t_env *head = *lst;

//     // If the head node needs to be deleted
//     if (strcmp(head->name, name) == 0)
//     {
//         *lst = head->next; // Update the head of the list globally
//         free_env_node(head);
//         return;
//     }

//     // Traverse the list to find and remove the node
//     t_env *prev = head;
//     t_env *curr = head->next;
//     while (curr != NULL)
//     {
//         if (strcmp(curr->name, name) == 0)
//         {
//             prev->next = curr->next;
//             free_env_node(curr);
//             return;
//         }
//         prev = curr;
//         curr = curr->next;
//     }
// }

void	remove_node(t_env **lst, char *name)
{
	t_env	*tmp;
	t_env	*curr;

	if (*lst == NULL)
		return ;
	if (!ft_strcmp((*lst)->name, name))
	{		
		tmp = *lst;
		*lst = (*lst)->next;
		printf("Puntero TMP %p\n", tmp);
		printf("Puntero LST %p\n", *lst);
		printf("Puntero **LST %p\n", lst);
		free_env_node(tmp);
		tmp = NULL;
        if (*lst != NULL)
            printf("New head is %s con direccion %p\n", (*lst)->name, *lst);
        else
            printf("List is now empty.\n");
		return ;
	}
	curr = *lst;
	while (curr->next != NULL)
	{
		if (!ft_strcmp(curr->next->name, name))
		{
			tmp = curr->next;
			curr->next = curr->next->next;
			free_env_node(tmp);
			return ;
		}
		else
			curr = curr->next;
	}
}

// Updated ft_unset function
int ft_unset(t_env **list, t_ast_node *command)
{
    t_ast_node *cur_arg;

    cur_arg = command->first_child->next_sibling->next_sibling->first_child;
    while (cur_arg != NULL)
    {
        if (!check_varname(cur_arg->value, 0))
        {
            ft_env_error("unset", cur_arg->value, "not a valid identifier");
            return (1);
        }
        remove_node(list, cur_arg->value);
        cur_arg = cur_arg->next_sibling;
    }
    //print_env(list);
    return (0);
}

int	list_len(t_env *env)
{
	int	i;

	i = 0;
	if (env != NULL)
	{
		while (env != NULL)
		{
			env = env->next;
			i++;
		}
	}
	return (i);
}
