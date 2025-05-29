LIB_RL = -L/goinfre/moel-amr/homebrew/opt/readline/lib -lreadline
INCLUDE_RL = -I/goinfre/moel-amr/homebrew/opt/readline/include
NAME = minishell
NAME_BONUS = minishell_bonus
CC = cc 
CFLAGS = -Wall -Wextra -Werror
SRC =	str_utils/ft_split.c \
		str_utils/ft_strchr.c \
		str_utils/ft_split_for_expand.c \
		str_utils/ft_split_for_expand_continue.c \
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
		main_m/xminishell.c \
		main_m/xminishell_continue_1.c \
		main_m/xminishell_continue_2.c \
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
		exec_continue/args_expand_norminated.c \
		exec_continue/args_expand_norminated_continue.c \
		exec_continue/args_expand_norminated_continue_2.c \
		exec_continue/file_expand.c \
		exec_continue/file_expand_continue_1.c \
		exec_continue/file_expand_continue_2.c \
		exec_continue/file_expand_continue_3.c \
		exec_continue/here_doc_expand.c \
		execution/signals.c \
		execution/signals_continue.c \
		str_utils/ft_strtrim.c \
		exec_continue/handle_wild_cards.c \
		exec_continue/handle_wild_cards_continue.c \
		exec_continue/handle_wild_cards_continue_2.c \
		str_utils/ft_atoi.c \
		exec_continue/file_wild_card.c \
		exec_continue/file_wild_card_continue.c \
		exec_continue/file_wild_card_continue_2.c \
		execution/built_in_calling.c \
		execution/gluing_stuff.c \
		execution/handle_pipes.c \
		execution/norminette_word1.c \
		execution/norminette_word1_continue.c \
		execution/norminette_word2.c \

SRC_BONUS =	str_utils_bonus/ft_split_bonus.c \
		str_utils_bonus/ft_strchr_bonus.c \
		str_utils_bonus/ft_split_for_expand_bonus.c \
		str_utils_bonus/ft_split_for_expand_continue_bonus.c \
		str_utils_bonus/put_str_bonus.c \
		str_utils_bonus/string_utils_bonus.c \
		str_utils_bonus/ft_strjoin_bonus.c \
		str_utils_bonus/ft_itoa_bonus.c \
		parsing_bonus/lexer_bonus.c \
		parsing_bonus/lexer_continue_1_bonus.c \
		parsing_bonus/lexer_continue_2_bonus.c \
		parsing_bonus/free_bonus.c \
		parsing_bonus/list_utils_bonus.c \
		parsing_bonus/tree_parsing_bonus.c \
		parsing_bonus/handle_redirs_continue_bonus.c \
		parsing_bonus/handle_redirs_bonus.c \
		parsing_bonus/quotes_bonus.c \
		parsing_bonus/handle_syntax_continue_1_bonus.c \
		parsing_bonus/handle_syntax_continue_2_bonus.c \
		parsing_bonus/handle_syntax_continue_3_bonus.c \
		parsing_bonus/handle_syntax_bonus.c \
		main_bonus/xminishell_bonus.c \
		main_bonus/xminishell_continue_1_bonus.c \
		main_bonus/xminishell_continue_2_bonus.c \
		built_ins_bonus/helper_env_bonus.c \
		built_ins_bonus/env_bonus.c \
		built_ins_bonus/list_utils_bonus.c \
		built_ins_bonus/cd_bonus.c \
		built_ins_bonus/pwd_bonus.c \
		built_ins_bonus/echo_bonus.c \
		built_ins_bonus/helper_exp_bonus.c \
		built_ins_bonus/export_bonus.c \
		built_ins_bonus/unset_bonus.c \
		built_ins_bonus/exit_bonus.c \
		execution_bonus/norminette_exec_tree_continue_bonus.c \
		execution_bonus/norminette_exec_tree_bonus.c \
		execution_bonus/excuting_redirs_continue_bonus.c \
		execution_bonus/excuting_redirs_bonus.c \
		execution_bonus/handle_heredoc_continue_bonus.c \
		execution_bonus/handle_heredoc_bonus.c \
		execution_bonus/norminette_word_msgs_bonus.c \
		exec_continue_bonus/args_expand_norminated_continue_2_bonus.c \
		exec_continue_bonus/args_expand_norminated_continue_bonus.c \
		exec_continue_bonus/args_expand_norminated_bonus.c \
		exec_continue_bonus/file_expand_bonus.c \
		exec_continue_bonus/file_expand_continue_1_bonus.c \
		exec_continue_bonus/file_expand_continue_2_bonus.c \
		exec_continue_bonus/file_expand_continue_3_bonus.c \
		exec_continue_bonus/here_doc_expand_bonus.c \
		execution_bonus/signals_bonus.c \
		execution_bonus/signals_continue_bonus.c \
		str_utils_bonus/ft_strtrim_bonus.c \
		exec_continue_bonus/handle_wild_cards_bonus.c \
		exec_continue_bonus/handle_wild_cards_continue_bonus.c \
		exec_continue_bonus/handle_wild_cards_continue_2_bonus.c \
		str_utils_bonus/ft_atoi_bonus.c \
		exec_continue_bonus/file_wild_card_bonus.c \
		exec_continue_bonus/file_wild_card_continue_bonus.c \
		exec_continue_bonus/file_wild_card_continue_2_bonus.c \
		execution_bonus/built_in_calling_bonus.c \
		execution_bonus/gluing_stuff_bonus.c \
		execution_bonus/handle_pipes_bonus.c \
		execution_bonus/norminette_word1_bonus.c \
		execution_bonus/norminette_word1_continue_bonus.c \
		execution_bonus/norminette_word2_bonus.c \


OBJ = $(SRC:.c=.o)
OBJ_BONUS = $(SRC_BONUS:.c=.o)
all: $(NAME)
$(NAME): $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) $(LIB_RL) -o $(NAME)

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJ_BONUS) 
	$(CC) $(CFLAGS) $(OBJ_BONUS) $(LIB_RL) -o $(NAME_BONUS)
%_bonus.o: %_bonus.c includes_bonus/minishell_bonus.h
	$(CC) $(CFLAGS) $(INCLUDE_RL) -c $< -o $@
%.o: %.c includes/minishell.h
	$(CC) $(CFLAGS) $(INCLUDE_RL) -c $< -o $@


clean:
	rm -rf $(OBJ)
	rm -rf $(OBJ_BONUS)

fclean: clean
	rm -rf $(NAME)
	rm -rf $(NAME_BONUS)

re: fclean all
