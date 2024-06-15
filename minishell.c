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
	token = (t_Token_node *)malloc(sizeof(t_Token_node));
	if (!token)
		return (1);
	token->next_token = NULL;
	token->prev_token = NULL;
	token->type = commandLine;
	token->value = NULL;
	lexer(&input, &token);
//	print_tokens(token);
	ast_root = create_ast_node(commandLine, input->string);
	current_token = token;
	ast_root = rule_command_line(&current_token, ast_root);
//	print_ast_tree(ast_root, 0);
	ft_pipes(ast_root);
// examples for testing
// du ./ | sort -n | tail -10
// ls -l | sort -k 5 -n | tail -10
// cat proto.c | tr -s ' ' '\n' | sort | uniq -c | sort -nr | head -10
// ps aux | awk '{print $1}' | sort | uniq -c | sort -nr
	free_ast(&ast_root);
	free_tokens(&token);
	free (buf);
	free (input);
	return (0);
}
