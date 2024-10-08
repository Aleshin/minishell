CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -L./libft -lft -lreadline
INCLUDES = -I./libft -I.

SRC = minishell.c inits.c ./lexer/token_functions.c ./lexer/token_functions1.c ./lexer/tokenizer_functions.c ./lexer/lexer_functions.c\
 ./lexer/removers.c ./lexer/expanders.c ./lexer/tokenizer_substring.c ./lexer/quotes_functions.c ./lexer/tokenizer_variables.c\
 ./parser/ast_structure.c ./parser/parser_functions.c ./parser/command_functions.c\
 free.c ./executor/pipes.c ./executor/ft_exit.c ./executor/builtiner.c ./executor/exit_status.c \
 signals.c ./executor/envvar.c ./executor/ft_echo.c ./executor/error_ft.c ./executor/redirections.c ./executor/redir_heredoc.c  \
 ./executor/exec_helpers.c ./executor/envvar_helpers.c ./executor/ft_export.c ./executor/ft_sort_lst.c ./executor/list_helpers.c
OBJ = $(SRC:.c=.o)
NAME = minishell

all: libs $(NAME)

libs:
	make -C ./libft

$(NAME): $(OBJ) ./libft/libft.a
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)

%.o: %.c Makefile minishell.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	make -C ./libft clean
	$(RM) $(OBJ)

fclean: clean
	make -C ./libft fclean
	$(RM) $(NAME)

re: fclean all


.PHONY: all clean fclean re libs
