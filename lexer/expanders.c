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
//Unite doubled < and > terminals

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
//remove <, > and >> and change type of next lexem to it

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
//Use next to heredoc-token lexem as delimiter
//read heredoc-input while no delimiter to string
//change value of delimiter to heredoc-input, change type of token to DQS
//Parsing DQS as substring, remove <<-token and go next token

int	expand_heredoc(t_Input **input, t_Token_node **token)
{
	char	*value;

	(void)input;
	if ((*token)->next_token != NULL
		&& (*token)->type == heredoc)
	{
		if ((*token)->next_token->type != lexem)
			return (1);
		(*token)->next_token->type = (*token)->type;
		value = heredoc_stdin((*token)->next_token->value);
		free((*token)->next_token->value);
		(*token)->next_token->value = value;
		(*token)->next_token->type = DOUBLE_QUOTED_STRING;
		tokenizer_double_quotes(input, &(*token)->next_token);
		(*token)->next_token->type = heredoc;
		delete_token(token);
		token = &(*token)->next_token;
	}
	return (0);
}
//check syntax errors with pipes

int	syntax_checker(t_Token_node **token)
{
	if ((*token)->type == PIPE
		&& ((*token)->next_token == NULL
			|| (*token)->prev_token == NULL
			|| ((*token)->next_token != NULL
				&& (*token)->next_token->type == PIPE)))
	{
		ft_putstr_fd("syntax error near unexpected token `|'\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}
//2nd pass of lexer with expanders, syntax checker and empty-lexem remover

int	expander(t_Input **input, t_Token_node **token_temp)
{
	while (*token_temp != NULL)
	{
		if (expand_redirects(token_temp) == 1)
			return (1);
		if (expand_heredoc(input, token_temp) == 1)
			return (1);
		if (syntax_checker(token_temp) == 1)
			return (1);
		if ((*token_temp)->type == lexem && (*token_temp)->value[0] == '\0')
			delete_token(token_temp);
		else if (*token_temp != NULL)
			token_temp = &(*token_temp)->next_token;
	}
	return (0);
}
