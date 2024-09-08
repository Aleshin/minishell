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
//Find close qoute

int	end_quotes_finder(t_Input **input)
{
	int	i;

	i = (*input)->current_char;
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
//Finds start of quoted string
//add previous opened lexem-token if needed
//end finds close cuote

int	rule_quotes_helper(t_Input **input, t_Token_node **token)
{
	int	i;

	i = (*input)->current_char;
	if ((*input)->string[i] == '\'' || (*input)->string[i] == '"')
	{
		if ((*input)->current_token_type == lexem)
		{
			if (token_add(token, input) == 1)
				return (-1);
		}
		(*input)->current_token_type = SINGLE_QUOTED_STRING;
		if ((*input)->string[i] == '"')
			(*input)->current_token_type = DOUBLE_QUOTED_STRING;
	}
	else
		return (i);
	(*input)->current_char++;
	return (end_quotes_finder(input));
}
//Find quoted string and parse double quoted

int	rule_quotes(t_Input **input, t_Token_node **token)
{
	int				i;
	t_Token_node	*token_temp;

	i = rule_quotes_helper(input, token);
	if (i == -1)
		return (-1);
	if (i > (*input)->token_start
		&& ((*input)->current_token_type == DOUBLE_QUOTED_STRING
			|| (*input)->current_token_type == SINGLE_QUOTED_STRING))
	{
		(*input)->current_char = i;
		(*input)->token_start++;
		if (token_add(token, input) == 1)
			return (1);
		(*input)->current_char++;
		(*input)->token_start = (*input)->current_char;
		token_temp = token_last(token);
		tokenizer_double_quotes(input, &token_temp);
		return (0);
	}
	return (1);
}
