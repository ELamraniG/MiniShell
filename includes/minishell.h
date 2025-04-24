#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef enum e_type_arg
{
	IN_REDIR,
	OUT_REDIR,
	APPEND,
	HEREDOC,
	PIPE,
	AND,
	OR,
	OP_PAREN,
	CL_PAREN,
	WORD,
}						t_type_arg;

typedef enum e_q_flags
{
	NQ,
	SQ,
	DQ
}						t_q_flags;

typedef struct s_redirect
{
	char				*file_name;
	int					type;
	struct s_redirect	*next;
}						t_redirect;

typedef struct s_ast_tree
{
	t_type_arg			type;
	char				*content;
	struct s_ast_tree	*left;
	struct s_ast_tree	*right;

	char				**args;
	t_q_flags			q_type;
	int					is_space;
	t_redirect			*redirect;
	int					arg_count;
}						t_ast_tree;

typedef struct s_lex_list
{
	char				*s;
	struct s_lex_list	*next;
	t_q_flags			q_type;
	t_type_arg			a_type;
	int					is_space;
}						t_lex_list;

void					handle_words(t_ast_tree *node, t_lex_list **token);
char					*ft_strdup(const char *s1);
int						ft_isspace(int c);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
void					get_next_quote(char *str, int *i, char c);
void					add_to_list(t_lex_list **ll, char *str, t_q_flags quote,
							int is_space);
int						ft_strcmp(const char *s1, const char *s2);
size_t					ft_strlen(const char *s);
size_t					ft_strlcpy(char *dst, const char *src, size_t dstsize);
t_lex_list				*new_node(char *str, t_q_flags quote, int is_space);
t_lex_list				*lexing_the_thing(char *str);

t_ast_tree				*new_ast_node(t_lex_list *node);
t_ast_tree				*parse_and_or(t_lex_list **token);
int						is_special_opperand(int n);
t_ast_tree				*parse_pipe(t_lex_list **token);

t_ast_tree				*parse_parenthesis_cmds(t_lex_list **token);

t_ast_tree				*create_ast_tree(t_lex_list *token);
t_redirect				*new_node_redir(char *str, int type);
void					add_to_list_redir(t_redirect **ll, char *str, int type);

#endif