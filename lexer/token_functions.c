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

char	*make_token_value(t_Input **input)
{
	int		start;
	int		end;
	int		i;
	int		j;
	char	*value;

	start = (*input)->token_start;
	end = (*input)->current_char;
	i = start;
	j = 0;
	value = (char *)malloc(end - start + 1);
	if (!value)
		return (NULL);
	while (i < end)
	{
		value[j++] = (*input)->string[i++];
	}
	value[j] = '\0';
	(*input)->token_start = (*input)->current_char;
	return (value);
}

int	copy_substring(t_Input **input, t_Token_node *token)
{
	char	*value;

	value = make_token_value(input);
	if (!value)
		return (1);
	token->value = value;
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

t_Token_node	*token_first(t_Token_node **token)
{
	t_Token_node	*first_token;

	first_token = *token;
	while (first_token->prev_token != NULL)
		first_token = first_token->prev_token;
	return (first_token);
}

int	token_add(t_Token_node **token, t_Input **input)
{
	t_Token_node	*token_temp;

	if ((*token)->type == commandLine)
	{
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
		token_temp->next_token->prev_token = token_temp;
		token_temp = token_temp->next_token;
		token_temp->type = (*input)->current_token_type;
		if (copy_substring(input, token_temp) == 1)
			return (1);
		token_temp->next_token = NULL;
	}
	return (0);
}
