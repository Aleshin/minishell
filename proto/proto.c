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
	t_ast_node		*ast_root;

	(void)argc;
	(void)argv;
	(void)ast_root;
	buf = readline("$> "); // Prompt for input command./
	if (buf == NULL || strcmp(buf, "exit") == 0)
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
		{
			if (rule_ws(&input, &token))
				rule_lexem(&input, &token);
		}
	}
	print_tokens(token);
	ast_root = create_ast_node(commandLine, buf);
    ast_root = rule_command_line(token, ast_root);
/*
	ast_root->first_child = create_ast_node(token->next_token->type, token->next_token->value);
	ast_root->first_child->first_child = create_ast_node(token->next_token->next_token->type, token->next_token->next_token->value);
	ast_root->first_child->first_child->first_child = create_ast_node(token->next_token->next_token->next_token->type, token->next_token->next_token->next_token->value);
	ast_root->next_sibling = create_ast_node(commandLine, buf);
	ast_root->first_child->next_sibling = create_ast_node(commandLine, buf);
	ast_root->first_child->first_child->next_sibling = create_ast_node(commandLine, buf);
	ast_root->first_child->first_child->next_sibling->next_sibling = create_ast_node(commandLine, buf);
	ast_root->first_child->first_child->first_child->next_sibling = create_ast_node(commandLine, buf);
	ast_root->first_child->first_child->first_child->next_sibling->next_sibling = create_ast_node(commandLine, buf);

*/
	print_ast_tree(ast_root, 0);
	return (0);
}
