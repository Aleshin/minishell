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
#include "proto.h"

int	main(int argc, char **argv)
{
	char			*buf;
	t_Input			*input;
	t_Token_node	*token;
	t_Token_node	*current_token;
	t_ast_node		*ast_root;

	(void)argc;
	(void)argv;
	buf = readline("$> "); // Prompt for input command
	if (buf == NULL || ft_strncmp(buf, "exit", ft_strlen(buf)) == 0)
	// If user enters exit or closes input (Ctrl+D), exit the loop
	{
		free(buf);
		return (0);
	}
	input = (t_Input *)malloc(sizeof(t_Input));
	input->token_start = 0;
	input->current_char = 0;
	input->current_token_type = commandLine;
	input->string = buf;
	token = NULL;
	while (input->string[input->current_char] != '\0')
	{
		if (rule_terminals (&input, &token))
			if (rule_ws(&input, &token))
				rule_lexem(&input, &token);
	}
	current_token = token;
//	print_tokens(token);
	ast_root = create_ast_node(commandLine, buf);
	ast_root = rule_command_line(&current_token, ast_root);
	print_ast_tree(ast_root, 0);
	free_ast(&ast_root);
	free_tokens(&token);
	free (buf);
	free (input);
	return (0);
}
