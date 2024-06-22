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

char	*heredoc_stdin(char *delimiter)
{
	char	*line;
	char	*buf;
	char	*buf_temp;

	buf = (char *)malloc(sizeof(char));
	*buf = '\0';
	while ((line = readline("$>")) != NULL)
	{
		if (ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
		{
			free(line);
			return (buf);
		}
		buf_temp = buf;
		buf = ft_strjoin(buf_temp, line, "\n");
		free(buf_temp);
		free(line);
	}
return (NULL);
}

int	main(int argc, char **argv, char **envp)
{
	char			*buf;
	t_Input			*input;
	t_Token_node	*token;
	t_Token_node	*current_token;
	t_ast_node		*ast_root;

	(void)argc;
	(void)argv;
	(void)envp;
	buf = readline("$> "); // Prompt for input command
	if (buf == NULL || ft_strncmp(buf, "exit", ft_strlen(buf)) == 0)
	// If user enters exit or closes input (Ctrl+D), exit the loop
	{
		free(buf);
		return (0);
	}
	token = (t_Token_node *)malloc(sizeof(t_Token_node));
	if (!token)
		return (1);
	token->next_token = NULL;
	token->prev_token = NULL;
	token->type = commandLine;
	token->value = buf;
	input = input_init(&token);
	if (lexer(&input, &token) == 1)
	{
		free(buf);
		free(input);
		free_tokens(&token);
		return (1);
	}
//	print_tokens(token);
	ast_root = create_ast_node(commandLine, input->string);
	current_token = token;
	ast_root = rule_command_line(&current_token, ast_root);
	//print_ast_tree(ast_root, 0);
	ft_executor(ast_root, envp);
// examples for testing
// du ./ | sort -n | tail -10
// ls -l | sort -k 5 -n | tail -10
// cat minishell.c | tr -s ' ' '\n' | sort | uniq -c | sort -nr | head -10
// ps aux | awk '{print $1}' | sort | uniq -c | sort -nr
	free_ast(&ast_root);
	free_tokens(&token);
	free (buf);
	free (input);
	return (0);
}
