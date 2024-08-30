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

void	del_token(t_Token_node **token_temp, t_Token_node **token)
{
	if ((*token_temp)->next_token)
	{
		if (*token == *token_temp)
		{
			delete_token(token_temp);
			*token = *token_temp;
		}
		else
			delete_token(token_temp);
	}
}

int	tokenizer_double_quotes(t_Input **input, t_Token_node **token)
{
	t_Input			*input_substring;
	t_Token_node	*token_temp;

	token_temp = token_last(token);
//	printf("token=%s, token_temp=%s", (*token)->value, token_temp->value);
	if (token_temp->type != DOUBLE_QUOTED_STRING)
		return (1);
	input_substring = input_init(&token_temp);
	input_substring->env = (*input)->env;
	if (!input_substring)
		return (1);
	while (input_substring->string[input_substring->current_char] != '\0')
	{
		if (rule_var(&input_substring, &token_temp))
			rule_lexem(&input_substring, &token_temp);
	}
	token_temp = input_substring->token;
	del_token(&token_temp, token);
	token_temp->type = SINGLE_QUOTED_STRING;
	while (token_temp != NULL && token_temp->next_token != NULL)
		join_next_token(&token_temp);
	free(input_substring);
	return (0);
}
