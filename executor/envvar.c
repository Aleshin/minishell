/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envvar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 23:07:15 by emikhayl          #+#    #+#             */
/*   Updated: 2024/07/05 23:07:26 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//печать списка (поменять принтф на что то другое)
//env
int print_env(t_env **env)
{
    t_env *curr = *env;

    while (curr != NULL)
    {
        // if (ft_strcmp(curr->name,"?") != 0)
        // {
            ft_putstr_fd(curr->name, STDOUT_FILENO);
            ft_putstr_fd("=", STDOUT_FILENO);
            ft_putendl_fd(curr->value, STDOUT_FILENO);
        //}
        curr = curr->next;
    }
    return(0);
}

//удаляет нодб если lst->name == name
void remove_node(t_env **lst, char *name)
{
    if (*lst == NULL)
        return ;

    t_env *head = *lst;

    // Delete first node
    if (!ft_strcmp(head->name, name))
    {
        *lst = head->next; 
        free_env_node(head);
        return;
    }
    // Delete in the middle or last
    t_env *prev = head;
    t_env *curr = head->next;
    while (curr != NULL)
    {
        if (!ft_strcmp(curr->name, name))
        {
            prev->next = curr->next;
            free_env_node(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

int ft_unset(t_env **list, t_ast_node *command) 
{
    t_ast_node *cur_arg;

    cur_arg = command->first_child->next_sibling->next_sibling->first_child;
    while (cur_arg != NULL) 
    {
        if (!check_varname(cur_arg->value, 0))
        {
            ft_env_error("unset", cur_arg->value, "not a valid identifier");
            return (0); //in mac is 1;
        }
        remove_node(list, cur_arg->value);
        cur_arg = cur_arg->next_sibling;
    }
    return (0);
}

int list_len(t_env *env)
{
    int i;
    
    i = 0;
    if (env != NULL)
	{
        while (env != NULL)
		{
            env = env->next;
            i++;
		}
	}
    return i;
}
