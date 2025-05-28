# LIB_RL = -lreadline
LIB_RL = -L/goinfre/jhamdaou/homebrew/opt/readline/lib -lreadline
INCLUDE_RL = -I/goinfre/jhamdaou/homebrew/opt/readline/include
NAME = minishell
CC = cc 
CFLAGS = #-fsanitize=address -g #later on add -Wall ....  		# args_expand.c \

SRC =	str_utils/ft_split.c \
		str_utils/ft_strchr.c \
		str_utils/ft_split_for_expand.c \
		str_utils/put_str.c \
		str_utils/string_utils.c \
		str_utils/ft_strjoin.c \
		str_utils/ft_itoa.c \
		parsing/lexer.c \
		parsing/lexer_continue_1.c \
		parsing/lexer_continue_2.c \
		parsing/free.c \
		parsing/list_utils.c \
		parsing/tree_parsing.c \
		parsing/handle_redirs_continue.c \
		parsing/handle_redirs.c \
		parsing/quotes.c \
		parsing/handle_syntax_continue_1.c \
		parsing/handle_syntax_continue_2.c \
		parsing/handle_syntax_continue_3.c \
		parsing/handle_syntax.c \
		xminishell_tester.c \
		built_ins/helper_env.c \
		built_ins/env.c \
		built_ins/list_utils.c \
		built_ins/cd.c \
		built_ins/pwd.c \
		built_ins/echo.c \
		built_ins/helper_exp.c \
		built_ins/export.c \
		built_ins/unset.c \
		built_ins/exit.c \
		execution/norminette_exec_tree_continue.c \
		execution/norminette_exec_tree.c \
		execution/excuting_redirs_continue.c \
		execution/excuting_redirs.c \
		execution/handle_heredoc_continue.c \
		execution/handle_heredoc.c \
		execution/norminette_word_msgs.c \
		args_expand_norminated.c \
		file_expand.c \
		here_doc_expand.c \
		execution/signals.c \
		str_utils/ft_strtrim.c \
		handle_wild_cards.c \
		str_utils/ft_atoi.c \
		file_wild_card.c \
		execution/built_in_calling.c \
		execution/gluing_stuff.c \
		execution/handle_pipes.c \
		execution/norminette_word1.c \
		execution/norminette_word2.c \

OBJ = $(SRC:.c=.o)

all: $(NAME)
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB_RL) -o $(NAME)

%.o: %.c includes/minishell.h
	$(CC) $(CFLAGS) $(INCLUDE_RL) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
