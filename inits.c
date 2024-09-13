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

int	init_start(t_main *main_str, char **envp)
{
	setup_signal_handlers();
//	disable_ctrl_backslash();
	main_str->environment_list = envp_to_linked_list(envp);
	if (!main_str->environment_list)
		return (1);
	set_exit_code(&main_str->environment_list, 0);
	return (0);
}

t_Input	*input_init(t_Token_node **token)
{
	t_Input		*input;

	input = (t_Input *)malloc(sizeof(t_Input));
	if (!input)
		return (NULL);
	input->current_char = 0;
	input->token_start = 0;
	input->current_token_type = (*token)->type;
	input->string = (*token)->value;
	input->token = *token;
	return (input);
}

t_Token_node	*token_init(char **buf)
{
	t_Token_node	*token;

	token = (t_Token_node *)malloc(sizeof(t_Token_node));
	if (!token)
		return (NULL);
	token->next_token = NULL;
	token->prev_token = NULL;
	token->type = commandLine;
	token->value = *buf;
	return (token);
}

int	init_lexer(t_main *main_str)
{
	add_history(main_str->buf);
	main_str->token = token_init(&main_str->buf);
	main_str->input = input_init(&main_str->token);
	main_str->input->env = main_str->environment_list;
	return (lexer(&(main_str->input), &(main_str->token)));
}
