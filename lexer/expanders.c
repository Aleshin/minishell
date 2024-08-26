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

int	expand_redirects(t_Token_node **token)
{
	if ((*token)->next_token != NULL
		&& ((*token)->type == redirect_in
			|| (*token)->type == redirect_out
			|| (*token)->type == redirect_out_add))
	{
		if ((*token)->next_token->type != lexem)
			return (1);
		(*token)->next_token->type = (*token)->type;
		delete_token(token);
		token = &(*token)->next_token;
	}
	return (0);
}

int	expand_heredoc(t_Input **input, t_Token_node **token)
{
	char	*value_temp;

	(void)input;
	if ((*token)->next_token != NULL
		&& (*token)->type == heredoc)
	{
		if ((*token)->next_token->type != lexem)
			return (1);
		(*token)->next_token->type = (*token)->type;
		value_temp = (*token)->next_token->value;
		(*token)->next_token->value
			= heredoc_stdin((*token)->next_token->value);
//		(*token)->next_token->type = DOUBLE_QUOTED_STRING;
//		tokenizer_double_quotes(input, &(*token)->next_token);
		(*token)->next_token->type = heredoc;
		free(value_temp);
		delete_token(token);
		token = &(*token)->next_token;
	}
	return (0);
}
