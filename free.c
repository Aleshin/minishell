/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 20:49:38 by saleshin          #+#    #+#             */
/*   Updated: 2024/05/09 20:50:28 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	free_noerr(t_main *main_str, int err_no)
{
	free_tokens(&main_str->token);
	free(main_str->input);
	if (err_no != -1)
		free(main_str->buf);
	return (0);
}

int	free_all(t_ast_node **ast_root, t_Token_node **token,
			t_Input **input, char **buf)
{
	(void)token;
	(void)ast_root;
	(void)buf;
	(void)input;
	if (ast_root)
		free_ast(ast_root);
	if (token)
		free_tokens(token);
	if (buf)
		free(*buf);
	if (input)
		free(*input);
	return (1);
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
