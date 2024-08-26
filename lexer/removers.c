/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 22:20:01 by saleshin          #+#    #+#             */
/*   Updated: 2024/05/11 22:20:04 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "./minishell.h"

int	ws_remover(t_Token_node **token)
{
	if ((*token)->type == WS)
		delete_token(token);
	if (*token == NULL)
		return (1);
	return (0);
}

int	quotes_remover(t_Token_node **token)
{
	t_Token_node	**token_temp;

	token_temp = token;
	if ((*token_temp)->type == SINGLE_QUOTED_STRING)
		(*token_temp)->type = lexem;
	if (((*token_temp)->next_token != NULL)
		&& (((*token_temp)->type == lexem
				&& (*token_temp)->next_token->type == SINGLE_QUOTED_STRING)
			|| ((*token_temp)->type == SINGLE_QUOTED_STRING
				&& (*token_temp)->next_token->type == lexem)
			|| ((*token_temp)->type == lexem
				&& (*token_temp)->next_token->type == lexem)))
	{
		(*token_temp)->type = lexem;
		join_next_token(token_temp);
		return (0);
	}
	return (1);
}

int	double_quotes_remover(t_Token_node **token)
{
	t_Token_node	**token_temp;

	token_temp = token;
	if ((*token_temp)->type == DOUBLE_QUOTED_STRING)
	{
		delete_token(token_temp);
		while (*token_temp != NULL && (*token_temp)->next_token != NULL)
		{
			if ((*token_temp)->next_token->type == DOUBLE_QUOTED_STRING)
			{
				delete_token(&(*token_temp)->next_token);
				break ;
			}
			join_next_token(token_temp);
		}
	}
	return (0);
}
