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

int	terminal_type(t_Input **input)
{
	int	i;
	int	token_type;

	i = (*input)->current_char;
	if ((*input)->string[i] == '$')
		token_type = var;
	else
	if ((*input)->string[i] == '|')
		token_type = PIPE;
	else
	if ((*input)->string[i] == '<')
		token_type = redirect_in;
	else
	if ((*input)->string[i] == '>')
		token_type = redirect_out;
		else
		return (-1);
	return (token_type);
}

int	rule_var(t_Input **input, t_Token_node **token)
{
	if ((*input)->string[(*input)->current_char] == '$')
	{
		if ((*input)->current_token_type == lexem)
			if (token_add(token, input) == 1)
				return (1);
		(*input)->current_char++;
		(*input)->current_token_type = var;
		if (token_add(token, input) == 1)
			return (1);
	}
	return(0);
}

int	rule_terminals(t_Input **input, t_Token_node **token)
{
	int	token_type;

	token_type = terminal_type(input);
	if (token_type == -1)
		return (1);
	if ((*input)->current_token_type == lexem)
		if (token_add(token, input) == 1)
			return (1);
	(*input)->current_char++;
	(*input)->current_token_type = token_type;
		if (token_add(token, input) == 1)
			return (1);
	return(0);
}

int	rule_quotes_helper(t_Input **input, t_Token_node **token)
{
	int	i;

	if ((*input)->current_token_type == lexem)
	{
		if (token_add(token, input) == 1)
			return (-1);
		(*input)->token_start = (*input)->current_char;
	}
	i = (*input)->current_char;
	if ((*input)->string[i] == '\'')
		(*input)->current_token_type = SINGLE_QUOTED_STRING;
	else if ((*input)->string[i] == '"')
		(*input)->current_token_type = DOUBLE_QUOTED_STRING;
	else
		return (i);
	i++;
	(*input)->token_start++;
	while ((*input)->string[i] != '\0')
	{
		if (((*input)->string[i] == '\'' && (*input)->current_token_type == SINGLE_QUOTED_STRING)
		|| ((*input)->string[i] == '"' && (*input)->current_token_type == DOUBLE_QUOTED_STRING))
			break ;
		i++;
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

int	tokenizer_double_quotes(t_Token_node **token)
{
	t_Input			*input_substring;
	t_Token_node	**token_temp;

	input_substring = (t_Input *)malloc(sizeof(t_Input));
	if (!input_substring)
		return (1);
	input_substring->current_char = 0;
	input_substring->token_start = 0;
	input_substring->current_token_type = (*token)->type;
	input_substring->string = (*token)->value;
	while (input_substring->string[input_substring->current_char] != '\0')
	{	
		if(rule_var(&input_substring, token))
			if (rule_ws(&input_substring, token))
				rule_lexem(&input_substring, token);
	}
	token_temp = token;
	while (*token_temp != NULL)
	{
		if (expand_var(token_temp) == 1)
			return (1);
		if (*token_temp != NULL)
			token_temp = &(*token_temp)->next_token;
	}
	while (*token_temp != NULL && (*token_temp)->next_token != NULL)
	{
		join_next_token(token_temp);
		token_temp = &(*token_temp)->next_token;
	}
	(*token_temp)->type = SINGLE_QUOTED_STRING;
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
