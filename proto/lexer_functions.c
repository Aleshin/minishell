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
#include "proto.h"

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
		(*input)->current_token_type = WS;
		(*input)->current_char = i;
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

int	print_tokens(t_Token_node *token_temp)
{
	while (token_temp->next_token != NULL)
	{
		printf("command: %d, %s\n", token_temp->type, token_temp->value);
		token_temp = token_temp->next_token;
	}
	printf("command: %d, %s\n", token_temp->type, token_temp->value);
	return (0);
}
