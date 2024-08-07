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

int	rule_var_word(t_Input **input)
{
	int	i;

	i = (*input)->token_start;
	while ((*input)->string[i] != '\0')
	{
		if (((*input)->string[i] >= '0' && (*input)->string[i] <= '9')
			|| ((*input)->string[i] >= 'A' && (*input)->string[i] <= 'Z')
			|| ((*input)->string[i] >= 'a' && (*input)->string[i] <= 'z')
			|| (*input)->string[i] == '_')
			(*input)->current_char = ++i;
		else if ((*input)->string[i] >= '?')
		{
			(*input)->current_char = ++i;
			break ;
		}
	}
	return (0);
}

int	detect_var(t_Input **input, t_Token_node **token)
{
	if ((*input)->string[(*input)->current_char] == '$')
	{
		if ((*input)->current_token_type == lexem)
			if (token_add(token, input) == 1)
				return (1);
		(*input)->current_char++;
		(*input)->token_start = (*input)->current_char;
		rule_var_word(input);
		if ((*input)->token_start != (*input)->current_char)
		{
			(*input)->current_token_type = var;
			if (token_add(token, input) == 1)
				return (1);
			return (0);
		}
	}
	return (1);
}

char	*ft_getenv(t_env *env, char *value)
{
	while (env != NULL)
	{
		if (!ft_strcmp(env->name, value))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	expand_var(t_Input *input, t_Token_node **token)
{
	char	*value_temp;

	value_temp = (*token)->value;
	if (ft_getenv(input->env, (*token)->value) == NULL)
		(*token)->value = ft_strdup("");
	else
		(*token)->value = ft_strdup(ft_getenv(input->env, (*token)->value));
	free(value_temp);
	return (0);
}

int	rule_var(t_Input **input, t_Token_node **token)
{
	t_Token_node	*token_temp;

	if (detect_var(input, token))
		return (1);
	token_temp = token_last(token);
	expand_var(*input, &token_temp);
	token_temp->type = SINGLE_QUOTED_STRING;
	return (0);
}
