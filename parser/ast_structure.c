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

#include "./minishell.h"

t_ast_node	*create_ast_node(t_SymbolType type, const char *value)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		exit (1);
	node->type = type;
	node->value = ft_strdup(value);
	node->param = 0;
	node->first_child = NULL;
	node->next_sibling = NULL;
	return (node);
}

void	add_child_node(t_ast_node *parent, t_ast_node *child)
{
	t_ast_node	*current;

	if (parent->first_child == NULL)
	{
		parent->first_child = child;
	}
	else
	{
		current = parent->first_child;
		while (current->next_sibling != NULL)
			current = current->next_sibling;
		current->next_sibling = child;
	}
}

int	print_ast_tree(t_ast_node *ast_node, int level)
{
	printf("%.*s%d, \"%s\" (%d)\n", level, "\t\t\t\t\t\t",
		ast_node->type, ast_node->value, ast_node->param);
	if (ast_node->first_child != NULL)
	{
		print_ast_tree(ast_node->first_child, level + 1);
	}
	if (ast_node->next_sibling != NULL)
	{
		print_ast_tree(ast_node->next_sibling, level);
	}
	return (0);
}
