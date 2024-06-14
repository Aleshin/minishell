CC = gcc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address
LDFLAGS = -L../libft -lft -lreadline
INCLUDES = -I../libft -I.

SRC = proto.c token_functions.c lexer_functions.c ast_structure.c parser_functions.c pipes.c
OBJ = $(SRC:.c=.o)
NAME = proto

all: libs $(NAME)

libs:
	make -C ../libft

$(NAME): $(OBJ) ../libft/libft.a
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)

%.o: %.c Makefile proto.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	make -C ../libft clean
	$(RM) $(OBJ)

fclean: clean
	make -C ../libft fclean
	$(RM) $(NAME)

re: fclean all


.PHONY: all clean fclean re libs
