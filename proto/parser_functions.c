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
		add_child_node(ast_node, reverse_node); // Add command node with exec node with or no args
		if (*token == NULL)
			return (ast_node);
		if ((*token)->type == PIPE)
		{
// Next 2 lines add token "pipe" to AST-tree
//			reverse_node = create_ast_node(PIPE, (*token)->value);
//			add_child_node(ast_node, reverse_node); // add pipe token
			*token = (*token)->next_token;
			ast_node = rule_command_line(token, ast_node); // Recursive Command line -> Command line
		}
	}
	return (ast_node);
}

t_ast_node	*rule_command(t_Token_node **token)
{
	t_ast_node		*command_node;
	t_ast_node		*reverse_node;

	command_node = NULL;
	reverse_node = rule_executable(token); // Command -> Executable
	if (reverse_node != NULL)
	{
		command_node = create_ast_node(command, ""); // Create command node and add exec node inside
		add_child_node(command_node, reverse_node);
		reverse_node = rule_arguments(token); // Command -> Arguments
		if (reverse_node != NULL)
		{
			add_child_node(command_node, reverse_node); // Add arguments node with list of arguments inside to command node
		}
	}
	return (command_node);
}

t_ast_node	*rule_executable(t_Token_node **token) // bottom of tree
{
	t_ast_node	*ast_node;

	ast_node = NULL;
	if (*token == NULL)
		return (NULL);
	if ((*token)->type == lexem)
	{
		ast_node = create_ast_node(executable, (*token)->value); // return exec node to command function
		*token = (*token)->next_token;
	}
	return (ast_node);
}

t_ast_node	*rule_arguments(t_Token_node **token)
{
	t_ast_node	*arguments_node;
	t_ast_node	*reverse_node;

//	if (*token == NULL)
//		return (NULL);
	arguments_node = create_ast_node(arguments, ""); // Create args node and add 1-st arg node inside 
	reverse_node = rule_argument(token); // Arguments -> Argument (first)
	if (reverse_node != NULL)
	{
		add_child_node(arguments_node, reverse_node);
		arguments_node->param = arguments_node->first_child->param + 1;
		arguments_node = recursive_arguments(token, arguments_node); // Arguments -> Argument (all next)
	}
	return (arguments_node);
}

t_ast_node	*recursive_arguments(t_Token_node **token, t_ast_node *ast_node)
{
	t_ast_node	*reverse_node;

	reverse_node = rule_argument(token); // -> argument
	if (reverse_node != NULL)
	{
		add_child_node(ast_node, reverse_node);
		ast_node->param++;
		ast_node = recursive_arguments(token, ast_node); // Recursive argument -> argument
	}
	return (ast_node);
}

t_ast_node	*rule_argument(t_Token_node **token) // bottom of tree
{
	t_ast_node	*ast_node;

	ast_node = NULL;
	if (*token == NULL)
		return (NULL);
	if ((*token)->type == lexem)
	{
		ast_node = create_ast_node(argument, (*token)->value);
		*token = (*token)->next_token;
	}
	return (ast_node);
}
