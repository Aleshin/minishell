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

//parse legal name of var or detect "?" var
int	rule_var_word(t_Input **input)
{
	int	i;

	i = (*input)->current_char;
	while ((*input)->string[i] != '\0')
	{
		if (((*input)->string[i] >= '0' && (*input)->string[i] <= '9')
			|| ((*input)->string[i] >= 'A' && (*input)->string[i] <= 'Z')
			|| ((*input)->string[i] >= 'a' && (*input)->string[i] <= 'z')
			|| (*input)->string[i] == '_')
			(*input)->current_char = ++i;
		else if ((*input)->string[i] == '?')
		{
			(*input)->current_char = ++i;
			break ;
		}
		else
			break ;
	}
	return (0);
}
//close and add previous lexem-token
//parse the var's name
//If name isn't empty add var-token

int	detect_var(t_Input **input, t_Token_node **token)
{
	if ((*input)->string[(*input)->current_char] == '$')
	{
		if ((*input)->current_token_type == lexem)
			if (token_add(token, input) == 1)
				return (1);
		(*input)->current_char++;
		rule_var_word(input);
		if ((*input)->current_char > (*input)->token_start + 1)
		{
			(*input)->token_start++;
			(*input)->current_token_type = var;
			if (token_add(token, input) == 1)
				return (1);
			(*input)->token_start = (*input)->current_char;
			return (0);
		}
		else
		{
			(*input)->current_token_type = lexem;
			(*input)->current_char--;
		}
	}
	return (1);
}
//find var name in env-list end return it's value
//if no return NULL

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
//change name in var-token to it's value
//If no set var-token value to ""

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
//assembly all previous functions to detect and expand var

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

/* Version with no var no token

if there is no var remove var token

int	expand_var(t_Input *input, t_Token_node **token)
{
	char	*value_temp;

	value_temp = (*token)->value;
	if (ft_getenv(input->env, (*token)->value) == NULL)
	{
		delete_token(token);
	}
	else
	{
		(*token)->value = ft_strdup(ft_getenv(input->env, (*token)->value));
		(*token)->type = SINGLE_QUOTED_STRING;
		free(value_temp);
	}
	return (0);

int	rule_var(t_Input **input, t_Token_node **token)
{
	t_Token_node	*token_temp;

	if (detect_var(input, token))
		return (1);
	token_temp = token_last(token);
	expand_var(*input, &token_temp);
	if (!token_temp)
		*token = NULL;
	return (0);
}
*/
