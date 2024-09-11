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
	if (!buf)
		return (NULL);
	*buf = '\0';
	line = readline("> ");
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
		line = readline("> ");
	}
	return (NULL);
}

int	parse_exec(t_main *main_str)
{
	main_str->ast_root = create_ast_node(commandLine, main_str->input->string);
	main_str->current_token = main_str->token;
	main_str->ast_root
		= rule_command_line(&main_str->current_token, main_str->ast_root);
	print_ast_tree(main_str->ast_root, 0);
	if (main_str->ast_root->first_child == NULL)
	{
	free_all(&main_str->ast_root, &main_str->token,
		&main_str->input, &main_str->buf);
		printf("Command '' not found\n");
		set_exit_code(&main_str->environment_list, 127);
		return (1);
	}
	if (ft_handle_builtin(main_str->ast_root, &main_str->environment_list) == 0)
		ft_executor(main_str->ast_root, &main_str->environment_list);
	free_all(&main_str->ast_root, &main_str->token,
		&main_str->input, &main_str->buf);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	int				err_no;
	t_main			main_str;

	(void)argc;
	(void)argv;
	if (init_start(&main_str, envp))
		return (1);
	rl_bind_key('\t', rl_insert);
	while (1)
	{
		main_str.buf = readline("$> ");
		if (main_str.buf == NULL)
		{
			free(main_str.buf);
			lst_dealloc(&main_str.environment_list);
			write (1, "exit\n", 5);
			return (0);
		}
		if (g_exit_code > 0)
		{
			set_exit_code(&main_str.environment_list, 128 + g_exit_code);
			g_exit_code = 0;
		}
		err_no = init_lexer(&main_str);
		if (err_no == 0)
			parse_exec(&main_str);
		else
			free_noerr(&main_str, err_no);
	}
	return (0);
}
// examples for testing
// du ./ | sort -n | tail -10
// ls -l | sort -k 5 -n | tail -10
// cat minishell.c | tr -s ' ' '\n' | sort | uniq -c | sort -nr | head -10
// ps aux | awk '{print $1}' | sort | uniq -c | sort -nr
// echo $HOME/$USER'$USER'$NOTHING"$USER"
