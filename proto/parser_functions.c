/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 22:44:31 by saleshin          #+#    #+#             */
/*   Updated: 2024/05/18 22:44:37 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "proto.h"

int	rule_command_line(t_Token_node **token, t_ast_node **ast_node)
{
	if (rule_command(token, ast_node))
	{
	}
	return (0);
}

int	rule_command(t_Token_node **token, t_ast_node **ast_node)
{
	if (rule_executable(token, ast_node))
	{
	}
	return (0);
}

int	rule_executable(t_Token_node **token, t_ast_node **ast_node)
{
	if ((*token)->type == lexem)
	{
		(*ast_node) = create_ast_node((*token)->type, (*token)->value);
	}
	return (0);
}

int	rule_arguments(t_Token_node **token, t_ast_node **ast_node)
{
	(void)token;
	(void)ast_node;
	return (0);
}

int	rule_argument(t_Token_node **token, t_ast_node **ast_node)
{
	(void)token;
	(void)ast_node;
	return (0);
}

int	print_ast_tree(t_ast_node *ast_node, int level)
{
	printf("%.*s%d, \"%s\"\n", level, "\t\t\t\t\t\t\t\t\t\t\t\t", ast_node->type, ast_node->value);
	if (ast_node->first_child != NULL)
	{
		print_ast_tree(ast_node->first_child, level + 1);
	}
	if (ast_node->next_sibling != NULL)
	{
		print_ast_tree(ast_node->next_sibling, level);
	}
	ast_node = NULL;
	return (0);
}
