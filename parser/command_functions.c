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

t_ast_node	*rule_argument_recursive(t_Token_node **token,
	t_ast_keys **ast_keys)
{
	t_ast_node	*ast_node;

	ast_node = NULL;
	if (*token == NULL)
		return (NULL);
	if ((*token)->type == lexem)
	{
		ast_node = create_ast_node(argument, (*token)->value);
		(*ast_keys)->arguments_num++;
		*token = (*token)->next_token;
		ast_node->next_sibling = rule_argument_recursive(token, ast_keys);
	}
	return (ast_node);
}
