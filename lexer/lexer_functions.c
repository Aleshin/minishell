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

int	rule_terminals(t_Input **input, t_Token_node **token)
{
	int	i;

	i = (*input)->current_char;
	if ((*input)->string[i] == '|'
		|| (*input)->string[i] == '<'
		|| (*input)->string[i] == '>')
	{
		if ((*input)->current_token_type == lexem)
			if (token_add(token, input) == 1)
				return (1);
		(*input)->current_char++;
		(*input)->current_token_type = terminals;
		if (token_add(token, input) == 1)
			return (1);
		return (0);
	}
	return (1);
}

int	rule_quotes_helper(t_Input **input, t_Token_node **token)
{
	int	i;

	i = (*input)->current_char;
	if ((*input)->string[i] == '\'')
	{
		if ((*input)->current_token_type == lexem)
		{
			if (token_add(token, input) == 1)
				return (-1);
			(*input)->token_start = (*input)->current_char;
		}
		(*input)->current_token_type = SINGLE_QUOTED_STRING;
		i++;
		(*input)->token_start++;
		while ((*input)->string[i] != '\0')
		{
			if ((*input)->string[i] == '\'')
				break ;
			i++;
		}
	}
	return (i);
}

int	rule_quotes(t_Input **input, t_Token_node **token)
{
	int	i;

	i = rule_quotes_helper(input, token);
	if (i == -1)
		return (1);
	if (i > (*input)->current_char)
	{
		(*input)->current_char = i;
		if (token_add(token, input) == 1)
			return (1);
		(*input)->token_start++;
		(*input)->current_char++;
		return (0);
	}
	return (1);
}

int	rule_ws(t_Input **input, t_Token_node **token)
{
	int	i;

	i = (*input)->current_char;
	while ((*input)->string[i] != '\0')
	{
		if ((*input)->string[i] != ' '
			&& (*input)->string[i] != '\t'
			&& (*input)->string[i] != '\r'
			&& (*input)->string[i] != '\n')
			break ;
		if ((*input)->current_token_type == lexem)
			if (token_add(token, input) == 1)
				return (1);
		(*input)->current_token_type = WS;
		i++;
	}
	if (i > (*input)->current_char)
	{
		(*input)->current_char = i;
		if (token_add(token, input) == 1)
			return (1);
		(*input)->token_start = i;
		return (0);
	}
	return (1);
}

int	rule_lexem(t_Input **input, t_Token_node **token)
{
	(*input)->current_token_type = lexem;
	(*input)->current_char++;
	if ((*input)->string[(*input)->current_char] == '\0')
		if (token_add(token, input) == 1)
			return (1);
	return (0);
}

int	rule_word(t_Input **input, t_Token_node **token)
{
	int	i;

	i = (*input)->token_start;
	while ((*input)->string[i] != '\0')
	{
		if ((*input)->string[i] < 'a' || (*input)->string[i] > 'z')
			break ;
		(*input)->current_char = ++i;
	}
	if ((*input)->token_start != (*input)->current_char)
	{
		(*input)->current_token_type = WORD;
		if (token_add(token, input) == 1)
			return (1);
		return (0);
	}
	return (1);
}

int	rule_symbol_unknown(t_Input **input, t_Token_node **token)
{
	(*input)->current_char++;
	(*input)->current_token_type = SYMBOL_UNKNOWN;
	if (token_add(token, input) == 1)
		return (1);
	return (0);
}

int	tokenizer(t_Input **input, t_Token_node **token)
{
	while ((*input)->string[(*input)->current_char] != '\0')
	{
		if (rule_terminals (input, token))
			if (rule_ws(input, token))
				if (rule_quotes(input, token))
					rule_lexem(input, token);
	}
	return (0);
}

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
		join_next_token(token_temp);
		return (0);
	}	
	return (1);
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
			return (0);
		if (quotes_remover(token_temp) && *token_temp != NULL)
			token_temp = &(*token_temp)->next_token;
	}
	return (0);
}
