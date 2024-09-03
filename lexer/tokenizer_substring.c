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

int	*token_change(t_Token_node **token, t_Token_node **token_temp)
{
	char	*value;

	value = ft_strdup((*token_temp)->value);
	free((*token)->value);
	(*token)->value = value;
	(*token)->type = lexem;
	free_tokens(token_temp);
	return (0);
}

int	tokenizer_double_quotes(t_Input **input, t_Token_node **token)
{
	t_Input			*input_substring;
	t_Token_node	*token_temp;

	if ((*token)->type != DOUBLE_QUOTED_STRING)
		return (1);
	token_temp = token_init(&(*token)->value);
	input_substring = input_init(&token_temp);
	if (!token_temp || !input_substring)
		return (1);
	input_substring->env = (*input)->env;
	while (input_substring->string[input_substring->current_char] != '\0')
	{
		if (rule_var(&input_substring, &token_temp))
			rule_lexem(&input_substring, &token_temp);
	}
	while (token_temp != NULL && token_temp->next_token != NULL)
		join_next_token(&token_temp);
//	printf("+++string tokens+++\n");
//	print_tokens(*token);
//	printf("+++substring tokens+++\n");
//	print_tokens(token_temp);
	token_change(token, &token_temp);
//	printf("+++string tokens+++\n");
//	print_tokens(*token);
	free(input_substring);
	return (0);
}
