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

int	twin_redirects(t_Token_node **token)
{
	if ((*token)->next_token != NULL)
	{
		if ((*token)->type == redirect_in
		&& (*token)->next_token->type == redirect_in)
		{
			(*token)->type = heredoc;
			join_next_token(token);
		}
		if ((*token)->type == redirect_out
		&& (*token)->next_token->type == redirect_out)
		{
			(*token)->type = redirect_out_add;
			join_next_token(token);
		}
	}
	return (0);
}

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

int	expand_heredoc(t_Token_node **token)
{
	char	*value_temp;

	if ((*token)->next_token != NULL
	&& (*token)->type == heredoc)
	{
		if ((*token)->next_token->type != lexem)
			return (1);
		(*token)->next_token->type = (*token)->type;
		value_temp = (*token)->next_token->value;
		(*token)->next_token->value = heredoc_stdin((*token)->next_token->value);
		(*token)->next_token->type = DOUBLE_QUOTED_STRING;
		tokenizer_double_quotes(&(*token)->next_token);
		(*token)->next_token->type = heredoc;
		free(value_temp);
		delete_token(token);
		token = &(*token)->next_token;
	}
	return (0);
}

int	expand_var(t_Token_node **token)
{
	char	*value_temp;

	if ((*token)->next_token != NULL
	&& (*token)->type == var)
	{
		if ((*token)->next_token->type != lexem)
			return (1);
		value_temp = (*token)->next_token->value;
		if (getenv((*token)->next_token->value) == NULL)
			(*token)->next_token->value = ft_strdup("");
		else
			(*token)->next_token->value = ft_strdup(getenv((*token)->next_token->value));
		free(value_temp);
		delete_token(token);
		token = &(*token)->next_token;
	}
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
				break;
			}
			join_next_token(token_temp);
//			token_temp = &(*token_temp)->next_token;
		}
	}
	return (0);
}

int	lexer(t_Input **input, t_Token_node **token)
{
	t_Token_node	**token_temp;

	token_temp = token;
	tokenizer(input, token_temp);
	token_temp = token;
	while (*token_temp != NULL)
	{
		if (ws_remover(token_temp) == 1)
			break;
		twin_redirects(token_temp);
		if (quotes_remover(token_temp) && *token_temp != NULL)
			token_temp = &(*token_temp)->next_token;
	}
	token_temp = token;
	while (*token_temp != NULL)
	{
		if (expand_redirects(token_temp) == 1)
			return (1);
		if (expand_heredoc(token_temp) == 1)
			return (1);
		if (expand_var(token_temp) == 1)
			return (1);
//		if (double_quotes_remover(token_temp) == 1)
//			return (1);
		if (*token_temp != NULL)
			token_temp = &(*token_temp)->next_token;
	}
//	token_temp = token;
	return (0);
}
