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

int	copy_substring(t_Input **input, t_Token_node *token)
{
	int	start;
	int	end;
	int	i;
	int	j;

	start = (*input)->token_start;
	end = (*input)->current_char;
	i = start;
	j = 0;
	token->value = (char *)malloc(end - start + 1);
	if (!token->value)
		return (1);
	while (i < end)
	{
		token->value[j++] = (*input)->string[i++];
	}
	token->value[j] = '\0';
	(*input)->token_start = (*input)->current_char;
	return (0);
}

t_Token_node	*token_last(t_Token_node **token)
{
	t_Token_node	*last_token;

	last_token = *token;
	while (last_token->next_token != NULL)
		last_token = last_token->next_token;
	return (last_token);
}

int	token_add(t_Token_node **token, t_Input **input)
{
	t_Token_node	*token_temp;

	if (*token == NULL)
	{
		*token = (t_Token_node *)malloc(sizeof(t_Token_node));
		if (!(*token))
			return (1);
		(*token)->next_token = NULL;
		(*token)->type = (*input)->current_token_type;
		if (copy_substring(input, *token) == 1)
			return (1);
	}
	else
	{
		token_temp = token_last(token);
		token_temp->next_token = (t_Token_node *)malloc(sizeof(t_Token_node));
		if (!token_temp->next_token)
			return (1);
		token_temp = token_temp->next_token;
		token_temp->type = (*input)->current_token_type;
		if (copy_substring(input, token_temp) == 1)
			return (1);
		token_temp->next_token = NULL;
	}
	return (0);
}

void	free_tokens(t_Token_node **head)
{
	t_Token_node	*current;
	t_Token_node	*next;

	current = *head;
	while (current != NULL)
	{
		next = current->next_token;
		free(current->value);
		free(current);
		current = next;
	}
}
