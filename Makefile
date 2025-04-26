NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

# SRC =	src/c.c \
# 		src/list_utils.c \
# 		src/string_utils.c \
# 		src/tree_parsing.c \
# 		src/handle_redirs.c \
# 		src/quotes.c

SRC =	parsing_new/c.c \
		parsing_new/list_utils.c \
		parsing_new/string_utils.c \
		parsing_new/tree_parsing.c \
		parsing_new/handle_redirs.c \
		parsing_new/quotes.c \
		parsing_new/handle_syntax.c \
		parsing_new/main.c 

OBJ = $(SRC:.c=.o)

all: $(NAME)
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -lreadline -o $(NAME)

%.o: %.c includes/minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
