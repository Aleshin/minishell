/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 19:15:51 by marvin            #+#    #+#             */
/*   Updated: 2024/09/13 19:15:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_node(t_env *node)
{
	if (node != NULL)
	{
		free(node->name);
		free(node->value);
		free(node);
	}
}

void	lst_dealloc(t_env **head)
{
	t_env	*current;
	t_env	*aux;

	if (*head == NULL)
		return ;
	current = *head;
	while (current != NULL)
	{
		aux = current;
		current = current->next;
		free_env_node(aux);
	}
	*head = NULL;
}

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
		free(new->name);
		free(new);
		return (NULL);
	}
	new->to_env = n;
	new->next = NULL;
	return (new);
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
		if (!ft_strcmp(name, curr->name))
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
