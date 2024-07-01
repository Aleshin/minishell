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
	line = readline("$>");
	while (line != NULL)
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
	t_env			*environment_list;

	(void)argc;
	(void)argv;
	setup_signal_handlers();
	environment_list = envp_to_linked_list(envp);
	if (!environment_list)
		return (1);
	while (1)
	{
		buf = readline("$> "); // Prompt for input command
		if (buf == NULL || ft_strcmp(buf, "exit") == 0)
		// If user enters exit or closes input (Ctrl+D), exit the loop
		{
			lst_dealloc(&environment_list);
			free(buf);
			return (0);
		}
		add_history(buf);
		token = token_init(&buf);
		input = input_init(&token);
		input->env = environment_list;
		if (lexer(&input, &token) == 1)
		{
			free_all(&ast_root, &token, &input, &buf);
			return (1);
		}
		//	print_tokens(token);
		ast_root = create_ast_node(commandLine, input->string);
		current_token = token;
		ast_root = rule_command_line(&current_token, ast_root);
		//print_ast_tree(ast_root, 0);

		if (ft_handle_builtin(ast_root, &input->env) == 0)
			ft_executor(ast_root, &input->env);
		free_all(&ast_root, &token, &input, &buf);
	}
	return (0);
}

int	free_all(t_ast_node **ast_root, t_Token_node **token,
			t_Input **input, char **buf)
{
	if (*ast_root)
		free_ast(ast_root);
	free(*buf);
	free_tokens(token);
	free(*input);
	return (1);
}
// examples for testing
// du ./ | sort -n | tail -10
// ls -l | sort -k 5 -n | tail -10
// cat minishell.c | tr -s ' ' '\n' | sort | uniq -c | sort -nr | head -10
// ps aux | awk '{print $1}' | sort | uniq -c | sort -nr
