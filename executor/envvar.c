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

//varname can start with _or alpha
int	check_varname(char *str) // 1 for yes, 0 for no
{
	if (*str == '\0')
		return (0);
	if (!(ft_isalpha(*str) || *str == '_'))
		return (0);
	str++;
	while (*str != '\0')
	{
		if (!(ft_isalpha(*str) || ft_isdigit(*str)
				|| *str == '_' || *str == '='))
			return (0);
		str++;
	}
	return (1);
}

// 1 valid, 0 not
int	check_varname_export(char *str)
{
	if (*str == '\0')
		return (0);
	if (!(ft_isalpha(*str) || *str == '_'))
		return (0);
	str++;
	while (*str != '\0' && *str != '=')
	{
		if (!(ft_isalpha(*str) || ft_isdigit(*str) || *str == '_'))
			return (0);
		str++;
	}
	return (1);
}

//prints linked list
int	print_env(t_env **env)
{
	t_env	*curr;

	curr = *env;
	while (curr != NULL)
	{
		if (ft_strcmp(curr->name, "?") != 0 && curr->to_env == 1)
		{
			printf("%s=%s\n", curr->name, curr->value);
		}
		curr = curr->next;
	}
	return (0);
}

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
		free_env_node(tmp);
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
