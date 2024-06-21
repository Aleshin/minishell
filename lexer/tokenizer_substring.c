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
		return (0);
	}
	return (1);
}

t_Input	*input_init(t_Token_node **token)
{
	t_Input		*input;

	input = (t_Input *)malloc(sizeof(t_Input));
	if (!input)
		return (NULL);
	input->current_char = 0;
	input->token_start = 0;
	input->current_token_type = (*token)->type;
	input->string = (*token)->value;
	input->token = *token;
	return (input);
}

int	tokenizer_double_quotes(t_Token_node **token)
{
	t_Input			*input_substring;
	t_Token_node	*token_temp;

	token_temp = token_last(token);
	if (token_temp->type != DOUBLE_QUOTED_STRING)
		return (1);
	input_substring = input_init(&token_temp);
	if (!input_substring)
		return (1);
	while (input_substring->string[input_substring->current_char] != '\0')
	{
		if (rule_var(&input_substring, &token_temp))
			if (rule_ws(&input_substring, &token_temp))
				rule_lexem(&input_substring, &token_temp);
	}
	while (token_temp != NULL)
	{
		if (expand_var(&token_temp) == 1)
			return (1);
		if (token_temp != NULL)
			token_temp = token_temp->next_token;
	}
	token_temp = input_substring->token;
	if (token_temp->next_token)
		delete_token(&token_temp);
	token_temp->type = SINGLE_QUOTED_STRING;
	while (token_temp != NULL && token_temp->next_token != NULL)
		join_next_token(&token_temp);
	free(input_substring);
	return (0);
}
