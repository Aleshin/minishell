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

#ifndef PROTO_H
# define PROTO_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <dirent.h>
# include <signal.h>

typedef enum SymbolType {
	commandLine,
	command,
	executable,
	arguments,
	argument,
	WORD,
	SINGLE_QUOTED_STRING,
	DOUBLE_QUOTED_STRING,
	PIPE,
	WS,
	SYMBOL_UNKNOWN
}	t_SymbolType;

typedef struct Input {
	int				token_start;
	int				current_char;
	t_SymbolType	current_token_type;
	char			*string;
}	t_Input;

typedef struct Token_node {
	t_SymbolType		type;
	char				*value;
	struct Token_node	*next_token;
}	t_Token_node;

typedef struct ast_node{
	t_SymbolType	type;
	char			*value; // string for symbol
	struct ast_node	*first_child; // Child' pointer
	struct ast_node	*next_sibling; // Sibling pointer
}	t_ast_node;

t_Token_node	*token_last(t_Token_node **tokens);
int				token_add(t_Token_node **tokens, t_Input **input);
int				rule_word(t_Input **input, t_Token_node **token);
int				rule_ws(t_Input **input);
int				rule_symbol_unknown(t_Input **input, t_Token_node **token);
t_ast_node		*create_ast_node(t_SymbolType type, const char *value);
void			add_child_node(t_ast_node *parent, t_ast_node *child);
void			free_ast(t_ast_node *node);
#endif