/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:48:26 by saleshin          #+#    #+#             */
/*   Updated: 2024/05/14 18:48:36 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*				AST structure
commands
ast_tree
	command 1
	ast_tree.first_child
		redirects (num of rdr): ast_tree.first_child.first_child
			redirect 1: ast_tree.first_child.first_child.first_child
			redirect 2: ast_tree.first_child.first_child.first_child.next_sibling
			...
		exec: ast_tree.first_child.first_child.next_sibling
		args (num of args): ast_tree.first_child.first_child.next_sibling.next_sibling
			arg 1: ast_tree.first_child.first_child.next_sibling..next_sibling.first_child
			arg 2: ast_tree.first_child.first_child.next_sibling..next_sibling.first_child.next_sibling
			...
	command 2: ast_tree.first_child.next_sibling
	...
*/
#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <sys/types.h>
# include <dirent.h>
# include <signal.h>
# include <termios.h>
# include <string.h>

typedef enum SymbolType
{
	commandLine,
	command,
	executable,
	arguments,
	argument,
	WORD,
	SINGLE_QUOTED_STRING,
	DOUBLE_QUOTED_STRING,
	PIPE,
	redirects,
	redirect_in,
	heredoc,
	redirect_out,
	redirect_out_add,
	var,
	WS,
	lexem,
	terminals,
	SYMBOL_UNKNOWN
}	t_SymbolType;

#define READ_END 0
#define WRITE_END 1

//декларируем структуру списка переменых среды
typedef struct t_list 
{
  char *name;
  char *value;
  //int export; //1 or 0, when you call export it indicates that env is exported to child process
  struct t_list *next;
} t_env;


typedef struct Input
{
	int					token_start;
	int					current_char;
	t_SymbolType		current_token_type;
	char				*string;
	struct Token_node	*token;
	t_env				*env;
}	t_Input;

typedef struct Token_node
{
	t_SymbolType		type;
	char				*value;
	struct Token_node	*next_token;
	struct Token_node	*prev_token;
}	t_Token_node;

typedef struct ast_node
{
	t_SymbolType	type;
	char			*value; // string for symbol
	int				param; // number of pipes or args
	struct ast_node	*first_child; // Child' pointer
	struct ast_node	*next_sibling; // Sibling pointer
}	t_ast_node;

typedef struct ast_keys
{
//	t_ast_node	*head;
	t_ast_node	*command;
	t_ast_node	*redirects;
	int			redirects_num;
	t_ast_node	*arguments;
	int			arguments_num;
//	t_ast_node	*reverse_node;
}	t_ast_keys;

typedef int (*t_function_pointer)(t_ast_node *);

// signals
void			setup_signal_handlers();
void disable_ctrl_backslash();
// token structure functions
t_Input			*input_init(t_Token_node **token);
t_Token_node 	*token_init(char **buf);
t_Token_node	*token_last(t_Token_node **tokens);
t_Token_node	*token_first(t_Token_node **token);
int				token_add(t_Token_node **tokens, t_Input **input);
int				append_to_token(t_Token_node **token, t_Input **input);
int				join_next_token(t_Token_node **token);
int				delete_token(t_Token_node **token);
void			free_tokens(t_Token_node **head);
int				print_tokens(t_Token_node *token_temp);
char			*ft_getenv(t_Input *input, char *value);
// lexer functions
char			*heredoc_stdin(char *delimiter);
int				rule_terminals(t_Input **input, t_Token_node **token);
//int				rule_word(t_Input **input, t_Token_node **token);
int				rule_ws(t_Input **input, t_Token_node **token);
int				rule_lexem(t_Input **input, t_Token_node **token);
int				rule_quotes(t_Input **input, t_Token_node **token);
//int				rule_symbol_unknown(t_Input **input, t_Token_node **token);
int				tokenizer(t_Input **input, t_Token_node **token);
int				tokenizer_double_quotes(t_Token_node **token);
//int				expand_var(t_Token_node **token);
int				rule_var(t_Input **input, t_Token_node **token);
int				lexer(t_Input **input, t_Token_node **token);
//ast structure functions
t_ast_node		*create_ast_node(t_SymbolType type, const char *value);
void			add_child_node(t_ast_node *parent, t_ast_node *child);
void			free_ast(t_ast_node **node);
// parser functions
t_ast_node		*rule_command_line(t_Token_node **token, t_ast_node *ast_node);
t_ast_node		*rule_command(t_Token_node **token);
t_ast_node		*rule_executable(t_Token_node **token);
t_ast_node		*rule_argument_recursive(t_Token_node **token,
					t_ast_keys **ast_keys);
t_ast_node		*rule_redirect_recursive(t_Token_node **token,
					t_ast_keys **ast_keys);
void			redirects_arguments(t_Token_node **token,
					t_ast_keys **ast_keys);
int				print_ast_tree(t_ast_node *ast_node, int level);
//executer functions
void			ft_executor(t_ast_node *commands, t_env **env);
int 			builtiner(t_ast_node *command, t_env **env);

//redirections
int input_redir(t_ast_node *commands);
int output_redir(t_ast_node *commands);
void handle_dup_and_close(int old_fd, int new_fd);

//builtins
int ft_handle_builtin(t_ast_node *ast_tree, t_env **env_list);
int 			is_builtin(t_ast_node *command);
int 			ft_echo(t_ast_node *command);
t_env	*ft_lstnew_env(char *name, char *value);
void ft_unset(t_env **list, t_ast_node *command);

//exec_helpers
void			free_arr(char **arr);
char	*ft_find_abs_path(char *command);
char			**cmd_to_argv(t_ast_node *cmd);
void ft_exec_command(t_ast_node *commands, t_env **env_var);

//envvar_helpers FULL
char **split_env(char *env, char c);
int check_varname(char *str);
char 			**linked_list_to_envp(t_env **env);
t_env 			*envp_to_linked_list(char **envp);
void free_env_node(t_env *node);
void lst_dealloc(t_env **head);

//envvar FULL
t_env	*ft_lstnew_env(char *name, char *value);
void	ft_lstadd_back_env(t_env **lst, t_env *new);
void print_env(t_env **env);
void remove_node(t_env **lst, char *name);
void ft_export(t_env **lst, char *str);
int list_len(t_env *env);

int list_len(t_env *env);

//helper functions
int				ft_strcmp(const char *s1, const char *s2);

void 			lst_dealloc(t_env **head);
int				free_all(t_ast_node **ast_root, t_Token_node **token, t_Input **input, char **buf);
void 			print_env(t_env **env);

//errors
void	print_error(char *command);
void ft_perror(char *str);
void ft_shell_error(char *cmd, char *error);
void ft_sintax_error(char *cmd);

#endif