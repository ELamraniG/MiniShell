LIB_RL = -L/goinfre/moel-amr/homebrew/opt/readline/lib -lreadline
INCLUDE_RL = -I/goinfre/moel-amr/homebrew/opt/readline/include
NAME = minishell
CC = cc 
CFLAGS =  -fsanitize=address 

SRC =	str_utils/ft_split.c \
		str_utils/ft_strchr.c \
		str_utils/put_str.c \
		str_utils/string_utils.c \
		str_utils/ft_strjoin.c \
		str_utils/ft_itoa.c \
		parsing/c.c \
		parsing/free.c \
		parsing/list_utils.c \
		parsing/tree_parsing.c \
		parsing/handle_redirs.c \
		parsing/quotes.c \
		parsing/handle_syntax.c \
		parsing/expand_vars.c \
		xminishell.c \
		built_ins/env.c \
		built_ins/list_utils.c \
		built_ins/cd.c \
		built_ins/pwd.c \
		built_ins/echo.c \
		built_ins/export.c \
		built_ins/unset.c \
		built_ins/exit.c \
		execution/excuting_redirs.c \
		execution/exec_tree.c \
		execution/handle_heredoc.c \
		utils/signals.c \
		# expandings_test.c

OBJ = $(SRC:.c=.o)

all: $(NAME)
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -L/goinfre/moel-amr/homebrew/opt/readline/lib -lreadline -o $(NAME)

%.o: %.c includes/minishell.h
	$(CC) $(CFLAGS) -I/goinfre/moel-amr/homebrew/opt/readline/include -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
