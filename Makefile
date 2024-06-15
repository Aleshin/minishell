CC = gcc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address
LDFLAGS = -L./libft -lft -lreadline
INCLUDES = -I./libft -I.

SRC = minishell.c ./lexer/token_functions.c ./lexer/lexer_functions.c\
 ./parser/ast_structure.c ./parser/parser_functions.c\
./executor/pipes.c
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
