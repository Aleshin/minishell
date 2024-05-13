/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 22:19:30 by saleshin          #+#    #+#             */
/*   Updated: 2024/05/11 22:19:34 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "proto.h"

int	copy_substring(Input *input, Token_node *token, int start, int end)
{
	int	i;
	int	j;

	i = start - 1;
	j = 0;
	token->next_token->value = (char *)malloc(end - start + 2);
	while (i < end)
	{
		token->next_token->value[j++] = input->string[i++];
//		printf("%s, ", token->next_token->value);
	}
	token->next_token->value[j] = '\0'; //'\n';
	return (0);
}

Token_node *token_last(Token_node *tokens)
{
    Token_node *last_token;

    last_token = tokens;
    while (last_token->next_token != NULL)
        last_token = last_token->next_token;
    return (last_token);
}

int token_add(Token_node **tokens, Input *input)
{
    Token_node  *token_temp;

    if (*tokens == NULL) {
        *tokens = (Token_node *)malloc(sizeof(Token_node));
        (*tokens)->next_token = NULL;
        (*tokens)->type = input->current_token_type;
        (*tokens)->value = input->string;
    } else {
token_temp = token_last(*tokens);
token_temp->next_token = (Token_node *)malloc(sizeof(Token_node));
        token_temp->next_token->type = WORD;
		copy_substring(input, token_temp, 5, 6);
//        token_temp->next_token->value = "The end";
        token_temp->next_token->next_token = NULL;
        }
    return (0);
}