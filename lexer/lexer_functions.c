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
//first pass of lexer create low-level token list
//second pass remove ws, terminals, quotes and parse "-tokens

int	lexer(t_Input **input, t_Token_node **token)
{
	t_Token_node	**token_temp;

	token_temp = token;
	if (tokenizer(input, token_temp) == -1)
		return (-1);
	token_temp = token;
	printf("+++after tokenizer+++\n");
	print_tokens(*token);
	while (*token_temp != NULL)
	{
		if (ws_remover(token_temp) == 1)
			break ;
		twin_redirects(token_temp);
		if (quotes_remover(token_temp) && *token_temp != NULL)
			token_temp = &(*token_temp)->next_token;
	}
	token_temp = token;
	printf("+++after ws remover, twin redirects and quotes remover+++\n");
	print_tokens(*token);
	if (expander(input, token_temp))
		return (1);
	printf("+++after redirect- and heredoc-expanders, empty-lexem remover and checker+++\n");
	print_tokens(*token);
	return (0);
}
