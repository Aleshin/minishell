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

int	lexer(t_Input **input, t_Token_node **token)
{
	t_Token_node	**token_temp;

	token_temp = token;
	if (tokenizer(input, token_temp) == -1)
		return (-1);
//	print_tokens(*token);
	token_temp = token;
	while (*token_temp != NULL)
	{
		if (ws_remover(token_temp) == 1)
			break ;
		twin_redirects(token_temp);
		if (quotes_remover(token_temp) && *token_temp != NULL)
			token_temp = &(*token_temp)->next_token;
	}
	token_temp = token;
	if (expander(input, token_temp))
		return (1);
	return (0);
}
