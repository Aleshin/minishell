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

int	expander(t_Input **input, t_Token_node **token_temp)
{
	while (*token_temp != NULL)
	{
		if (syntax_checker(token_temp) == 1)
			return (1);
		if (expand_redirects(token_temp) == 1)
			return (1);
		if (expand_heredoc(input, token_temp) == 1)
			return (1);
		if (*token_temp != NULL)
			token_temp = &(*token_temp)->next_token;
	}
	return (0);
}

int	lexer(t_Input **input, t_Token_node **token)
{
	t_Token_node	**token_temp;

	token_temp = token;
	if (tokenizer(input, token_temp) == -1)
		return (-1);
	token_temp = token;
	while (*token_temp != NULL)
	{
		if (ws_remover(token_temp) == 1)
			break ;
		twin_redirects(token_temp);
		if (quotes_remover(token_temp) && *token_temp != NULL)
			token_temp = &(*token_temp)->next_token;
	}
	token_temp = token;
	if (expander(input, token_temp))
		return (1);
	return (0);
}
