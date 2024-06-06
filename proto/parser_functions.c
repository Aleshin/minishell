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

t_ast_node	*rule_command_line(t_Token_node **token, t_ast_node *ast_node)
{
	t_ast_node	*reverse_node;

	reverse_node = rule_command(token); // Command line -> Command
	if (reverse_node != NULL)
	{
		add_child_node(ast_node, reverse_node);
		if ((*token)->next_token != NULL
			&& (*token)->next_token->value[0] == '|')
		{
			*token = (*token)->next_token;
			reverse_node = create_ast_node(PIPE, (*token)->value);
			add_child_node(ast_node, reverse_node);
			if ((*token)->next_token != NULL)
			{
				*token = (*token)->next_token;
				ast_node = rule_command_line(token, ast_node); // Recursive Command line -> Command line
			}
		}
	}
	return (ast_node);
}

t_ast_node	*rule_command(t_Token_node **token)
{
	t_ast_node	*command_node;
	t_ast_node	*reverse_node;

	command_node = NULL;
	reverse_node = rule_executable(token); // Command -> Executable
	if (reverse_node != NULL)
	{
		command_node = create_ast_node(command, "");
		add_child_node(command_node, reverse_node);
	}
	if ((*token)->next_token != NULL)
	{
		reverse_node = rule_arguments(&(*token)->next_token); // Command -> Arguments
		if (reverse_node != NULL)
		{
			*token = (*token)->next_token;
			add_child_node(command_node, reverse_node);
		}
	}
	return (command_node);
}

t_ast_node	*rule_executable(t_Token_node **token) // bottom of tree
{
	t_ast_node	*ast_node;

	ast_node = NULL;
	if ((*token)->type == lexem)
	{
		ast_node = create_ast_node(executable, (*token)->value);
	}
	return (ast_node);
}

t_ast_node	*rule_arguments(t_Token_node **token)
{
	t_ast_node	*arguments_node;
	t_ast_node	*reverse_node;

	arguments_node = NULL;
	reverse_node = rule_argument(token); // Arguments -> Argument (first)
	if (reverse_node != NULL)
	{
		arguments_node = create_ast_node(arguments, "");
		add_child_node(arguments_node, reverse_node);
		arguments_node = recursive_arguments(token, arguments_node); // Arguments -> Argument (all next)
	}
	return (arguments_node);
}

t_ast_node	*recursive_arguments(t_Token_node **token, t_ast_node *ast_node)
{
	t_ast_node	*reverse_node;

	if ((*token)->next_token != NULL && (*token)->next_token->type == lexem)
	{
		*token = (*token)->next_token;
		reverse_node = rule_argument(token); // -> argument
		if (reverse_node != NULL)
		{
			add_child_node(ast_node, reverse_node);
			ast_node = recursive_arguments(token, ast_node); // Recursive argument -> argument
		}
	}
	return (ast_node);
}

t_ast_node	*rule_argument(t_Token_node **token) // bottom of tree
{
	t_ast_node	*ast_node;

	ast_node = NULL;
	if ((*token)->type == lexem)
	{
		ast_node = create_ast_node(argument, (*token)->value);
	}
	return (ast_node);
}

int	print_ast_tree(t_ast_node *ast_node, int level)
{
	printf("%.*s%d, \"%s\"\n", level, "\t\t\t\t\t\t", ast_node->type, ast_node->value);
	if (ast_node->first_child != NULL)
	{
		print_ast_tree(ast_node->first_child, level + 1);
	}
	if (ast_node->next_sibling != NULL)
	{
		print_ast_tree(ast_node->next_sibling, level);
	}
//	ast_node = NULL;
	return (0);
}
