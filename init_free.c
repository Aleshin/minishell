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

int	init_start(t_main *main_str, char **envp)
{
	setup_signal_handlers();
	disable_ctrl_backslash();
	main_str->environment_list = envp_to_linked_list(envp);
	if (!main_str->environment_list)
		return (1);
	set_exit_code(&main_str->environment_list, 0);
	return (0);
}

int	init_lexer(t_main *main_str)
{
	add_history(main_str->buf);
	main_str->token = token_init(&main_str->buf);
	main_str->input = input_init(&main_str->token);
	main_str->input->env = main_str->environment_list;
	return (lexer(&(main_str->input), &(main_str->token)));
}

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
