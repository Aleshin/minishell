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
/*
int	init_lexer(t_main main_str)
{
	return (lexer(main_str.input, main_str.token));
}
*/
int	main(int argc, char **argv, char **envp)
{
	t_env			*environment_list;
	int				err_no;
	t_main			main_str;

	(void)argc;
	(void)argv;
	setup_signal_handlers();
	disable_ctrl_backslash();
	environment_list = envp_to_linked_list(envp);
	if (!environment_list)
		return (1);
	set_exit_code(&environment_list, 0);
	while (1)
	{
		main_str.buf = readline("$> ");
		if (main_str.buf == NULL || ft_strcmp(main_str.buf, "exit") == 0)
		{
			free(main_str.buf);
			lst_dealloc(&environment_list);
			write (1, "exit\n", 5);
			return (0);
		}
		add_history(main_str.buf);
		main_str.token = token_init(&main_str.buf);
		main_str.input = input_init(&main_str.token);
		main_str.input->env = environment_list;
//		err_no = init_lexer(&main_str);
		err_no = lexer(&main_str.input, &main_str.token);
		if (err_no == 0)
		{
//			print_tokens(token);
			main_str.ast_root = create_ast_node(commandLine, main_str.input->string);
			main_str.current_token = main_str.token;
			main_str.ast_root = rule_command_line(&main_str.current_token, main_str.ast_root);
			print_ast_tree(main_str.ast_root, 0);
			if (ft_handle_builtin(main_str.ast_root, &main_str.input->env) == 0)
				ft_executor(main_str.ast_root, &main_str.input->env);
			free_all(&main_str.ast_root, &main_str.token, &main_str.input, &main_str.buf);
		}
		else
		{
//			free_all(&ast_root, &token, &input, &buf);
			free_tokens(&main_str.token);
			free(main_str.input);
			if (err_no != -1)
				free(main_str.buf);
		}
	}
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
// examples for testing
// du ./ | sort -n | tail -10
// ls -l | sort -k 5 -n | tail -10
// cat minishell.c | tr -s ' ' '\n' | sort | uniq -c | sort -nr | head -10
// ps aux | awk '{print $1}' | sort | uniq -c | sort -nr
