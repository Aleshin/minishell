/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 19:39:57 by saleshin          #+#    #+#             */
/*   Updated: 2024/05/10 19:40:01 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "proto.h"

t_ast_node	*create_ast_node(t_SymbolType type, const char *value)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	node->type = type;
	node->value = strdup(value); // Дублирование строки для безопасного хранения
	node->first_child = NULL;
	node->next_sibling = NULL;
	return (node);
}

void	add_child_node(t_ast_node *parent, t_ast_node *child)
{
	t_ast_node	*current;

	if (parent->first_child == NULL)
	{
		parent->first_child = child; // Если у родителя нет детей, добавить первого ребенка
	}
	else
	{
		current = parent->first_child;
		while (current->next_sibling != NULL)
		{
			current = current->next_sibling; // Перемещение к последнему ребенку
		}
		current->next_sibling = child; // Добавление нового ребенка в конец списка братьев
	}
}

void	free_ast(t_ast_node *node)
{
	t_ast_node	*current;
	t_ast_node	*next;

	if (node->value)
	{
		free(node->value);
	}
	current = node->first_child;
	while (current != NULL)
	{
		next = current->next_sibling;
		free_ast(current);
		current = next;
	}
	free (node);
}
