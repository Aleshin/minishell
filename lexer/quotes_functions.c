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

int	end_quotes_finder(t_Input **input, int i)
{
	while ((*input)->string[i] != '\0')
	{
		if (((*input)->string[i] == '\''
				&& (*input)->current_token_type == SINGLE_QUOTED_STRING)
			|| ((*input)->string[i] == '"'
				&& (*input)->current_token_type == DOUBLE_QUOTED_STRING))
			return (i);
		i++;
	}
	return (-1);
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
	return (end_quotes_finder(input, i));
}

int	rule_quotes(t_Input **input, t_Token_node **token)
{
	int	i;

	i = rule_quotes_helper(input, token);
	if (i == -1)
		return (-1);
	if (i > (*input)->current_char)
	{
		(*input)->current_char = i;
		if (token_add(token, input) == 1)
			return (1);
		(*input)->token_start++;
		(*input)->current_char++;
		tokenizer_double_quotes(input,token);
		return (0);
	}
	return (1);
}
