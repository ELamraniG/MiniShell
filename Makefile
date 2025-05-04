NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

# SRC =	src/c.c \
# 		src/list_utils.c \
# 		src/string_utils.c \
# 		src/tree_parsing.c \
# 		src/handle_redirs.c \
# 		src/quotes.c

SRC =	parsing/c.c \
		parsing/free.c \
		parsing/list_utils.c \
		parsing/string_utils.c \
		parsing/tree_parsing.c \
		parsing/handle_redirs.c \
		parsing/quotes.c \
		parsing/handle_syntax.c \
		parsing/main.c \
		built_ins/set_up_env.c \
		built_ins/list_utils.c \
		built_ins/cd.c \
		built_ins/pwd.c \
		built_ins/echo.c \

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
