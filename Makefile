NAME = minishell
CC = cc
CFLAGS = 
# -Wall -Wextra -Werror

SRC =	src/c.c \
		src/list_utils.c \
		src/string_utils.c \
		src/tree_parsing.c \
		src/handle_redirs.c \
		src/quotes.c

OBJ = $(SRC:.c=.o)

all: $(NAME)
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c includes/minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
