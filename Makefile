CC = gcc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address
LDFLAGS = -L./libft -lft -lreadline
INCLUDES = -I./libft -I.

SRC = minishell.c ./lexer/token_functions.c ./lexer/token_functions1.c ./lexer/tokenizer_functions.c ./lexer/lexer_functions.c\
 ./lexer/removers.c ./lexer/expanders.c ./lexer/tokenizer_substring.c ./lexer/quotes_functions.c ./lexer/tokenizer_variables.c\
 ./parser/ast_structure.c ./parser/parser_functions.c ./parser/command_functions.c\
 init_free.c ./executor/pipes.c ./executor/builtiner.c\
  signals.c ./executor/envvar.c ./executor/ft_echo.c ./executor/error_ft.c ./executor/redirections.c\
  ./executor/exec_helpers.c ./executor/envvar_helpers.c ./executor/ft_export.c ./executor/ft_sort_lst.c
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
