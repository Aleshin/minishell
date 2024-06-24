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

t_ast_node	*rule_redirect_recursive(t_Token_node **token,
	t_ast_keys **ast_keys)
{
	t_ast_node	*ast_node;

	ast_node = NULL;
	if (*token == NULL)
		return (NULL);
	if ((*token)->type == redirect_in
		|| (*token)->type == redirect_out
		|| (*token)->type == heredoc
		|| (*token)->type == redirect_out_add)
	{
		ast_node = create_ast_node((*token)->type, (*token)->value);
		(*ast_keys)->redirects_num++;
		*token = (*token)->next_token;
		ast_node->next_sibling = rule_redirect_recursive(token, ast_keys);
	}
	return (ast_node);
}