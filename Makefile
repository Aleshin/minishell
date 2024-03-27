CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -L./libft -lft -lreadline
INCLUDES = -I./libft -I.

SRC = minishell.c
OBJ = $(SRC:.c=.o)
NAME = minishell

all: $(NAME)

$(NAME): $(OBJ)
	make -C libft
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	make -C libft clean
	$(RM) $(OBJ)

fclean: clean
	make -C libft fclean
	$(RM) $(NAME)

re: fclean all


.PHONY: all clean fclean re
