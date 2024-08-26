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
#include "./minishell.h"

int	join_next_token(t_Token_node **token)
{
	char	*new_value;

	new_value = ft_strjoin((*token)->value, (*token)->next_token->value, "");
	if (!new_value)
		return (1);
	free((*token)->value);
	(*token)->value = new_value;
	delete_token(&(*token)->next_token);
	return (0);
}

int	delete_token(t_Token_node **token)
{
	t_Token_node	*prev_token;
	t_Token_node	*next_token;

	prev_token = (*token)->prev_token;
	next_token = (*token)->next_token;
	free((*token)->value);
	free(*token);
	*token = NULL;
	if (prev_token != NULL)
	{
		prev_token->next_token = next_token;
	}
	if (next_token != NULL)
	{
		next_token->prev_token = prev_token;
		*token = next_token;
	}
	return (0);
}

int	print_tokens(t_Token_node *token_temp)
{
	while (token_temp != NULL)
	{
		printf("%d, \"%s\"\n",
			token_temp->type, token_temp->value);
		token_temp = token_temp->next_token;
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
