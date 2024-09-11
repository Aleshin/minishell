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
//detects 1-symbol terminals |, < and >

int	terminal_type(t_Input **input)
{
	int	i;
	int	token_type;

	i = (*input)->current_char;
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
//check if its 1-symbol terminal then add terminal token
//add previous opened lexem-token if need

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
	return (0);
}
//check if it ws-symbol then start, check continues
//ws-symbols and add ws-token
//add previous opened lexem-token if need

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
//every other symbols detects as "lexem". Function starts lexem-token
//and add lexem-token when input ends

int	rule_lexem(t_Input **input, t_Token_node **token)
{
	(*input)->current_token_type = lexem;
	(*input)->current_char++;
	if ((*input)->string[(*input)->current_char] == '\0')
		if (token_add(token, input) == 1)
			return (1);
	return (0);
}
//check every rule one by one for current char
//all undefined chars goes to lexem-symbol
//As a result of tokenizer we have low-level list of tokens with
//terminals |, < and >, '- and "-strings, expanded vars and lexem-tokens

int	tokenizer(t_Input **input, t_Token_node **token)
{
	int	res;

	if ((*input)->string[0] == '\0')
		return (-1);
	while ((*input)->string[(*input)->current_char] != '\0')
	{
		if (rule_terminals(input, token))
		{
			if (rule_ws(input, token))
			{
				res = rule_quotes(input, token);
				if (res == -1)
				{
					ft_putstr_fd("opened quotes error\n", STDERR_FILENO);
					set_exit_code(&(*input)->env, 2);
					return (-1);
				}
				if (res == 1)
					if (rule_var(input, token))
						rule_lexem(input, token);
			}
		}
	}
	return (0);
}
