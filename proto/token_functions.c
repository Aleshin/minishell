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

char	*make_token_value(t_Input **input)
{
	int	start;
	int	end;
	int	i;
	int	j;
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

int	token_add(t_Token_node **token, t_Input **input)
{
	t_Token_node	*token_temp;

	if ((*token)->type == commandLine)
	{
//		*token = (t_Token_node *)malloc(sizeof(t_Token_node));
//		if (!(*token))
//			return (1);
		(*token)->next_token = NULL;
		(*token)->prev_token = NULL;
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

int	join_next_token(t_Token_node **token)
{
	char	*next_token_value;
	char	*new_value;

	next_token_value = ft_strdup((*token)->next_token->value);
	if (!next_token_value)
		return (1);
	new_value = ft_strjoin((*token)->value, next_token_value, "");
	if (!new_value)
		return (1);
	free((*token)->value);
	(*token)->value = new_value;
	(*token)->type = lexem;
	free(next_token_value);
	return (0);
}

int	delete_token(t_Token_node **token)
{
	t_Token_node	*prev_token;
	t_Token_node	*next_token;

	if ((*token)->next_token == NULL)
	{
		prev_token = (*token)->prev_token;
		free(*token);
		*token = NULL;
		prev_token->next_token = NULL;
	}
	else
	{
		prev_token = (*token)->prev_token;
		next_token = (*token)->next_token;
		free(*token);
		if (prev_token != NULL)
			prev_token->next_token = next_token;
		if (next_token != NULL)
		{
			next_token->prev_token = prev_token;
			*token = next_token;
		}
		else
		{
			*token = prev_token;
		}
	}
	return (0);
}


int	append_to_token(t_Token_node **token, t_Input **input)
{
	char	*add_value;
	char	*new_value;

	add_value = make_token_value(input);
	if (!add_value)
		return (1);
	new_value = ft_strjoin((*token)->value, add_value, "");
	if (!new_value)
		return (1);
	free((*token)->value);
	(*token)->value = new_value;
	free(add_value);
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
