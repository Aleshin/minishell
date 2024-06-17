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
#include "./minishell.h"

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

void	command_init(t_ast_keys **ast_keys, t_ast_node **reverse_node)
{
	if (*reverse_node != NULL)
	{
		if (!(*ast_keys)->command)
		{
			(*ast_keys)->command = create_ast_node(command, "");
			(*ast_keys)->redirects = create_ast_node(redirects, "");
			add_child_node((*ast_keys)->command, (*ast_keys)->redirects);
		}
		if ((*reverse_node)->type == redirect_in
			|| (*reverse_node)->type == redirect_out
			|| (*reverse_node)->type == redirect_out_add
			|| (*reverse_node)->type == heredoc)
		{
			add_child_node((*ast_keys)->redirects, *reverse_node);
			(*ast_keys)->redirects->param = (*ast_keys)->redirects_num;
		}
		if ((*reverse_node)->type == executable)
		{
			add_child_node((*ast_keys)->command, *reverse_node);
			(*ast_keys)->arguments = create_ast_node(arguments, "");
			add_child_node((*ast_keys)->command, (*ast_keys)->arguments);
		}
	}
}

t_ast_node	*rule_command(t_Token_node **token)
{
	t_ast_node	*reverse_node;
	t_ast_keys	*ast_keys;

	ast_keys = malloc(sizeof(t_ast_keys));
	ast_keys->command = NULL;
	ast_keys->redirects = NULL;
	ast_keys->redirects_num = 0;
	ast_keys->arguments = NULL;
	ast_keys->arguments_num = 0;
	reverse_node = rule_redirect_recursive(token, &ast_keys);
	command_init(&ast_keys, &reverse_node);
	reverse_node = rule_executable(token); // Command -> Executable
	command_init(&ast_keys, &reverse_node);
	if (ast_keys->arguments != NULL)
		redirects_arguments(token, &ast_keys);
	reverse_node = ast_keys->command;
	free(ast_keys);
	return (reverse_node);
}

void	redirects_arguments(t_Token_node **token, t_ast_keys **ast_keys)
{
	t_ast_node	*reverse_node;

	reverse_node = rule_redirect_recursive(token, ast_keys);
	if (reverse_node != NULL)
	{
		add_child_node((*ast_keys)->redirects, reverse_node);
		redirects_arguments(token, ast_keys);
	}
	reverse_node = rule_argument_recursive(token, ast_keys);
	if (reverse_node != NULL)
	{
		add_child_node((*ast_keys)->arguments, reverse_node);
		redirects_arguments(token, ast_keys);
	}
	(*ast_keys)->redirects->param = (*ast_keys)->redirects_num;
	(*ast_keys)->arguments->param = (*ast_keys)->arguments_num;
}
