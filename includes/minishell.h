#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
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

typedef struct s_env_list
{
	char				*key;
	char				*value;
	int					flag;
	struct s_env_list	*next;
}						t_env_list;

typedef struct s_redirect
{
	char				**file_name;
	int					*q_types;
	int					*is_space;
	int					file_str_count;
	char				*final_file_name;
	int					type;
	struct s_redirect	*next;
	int					heredoc;
}						t_redirect;

typedef struct s_vars
{
	int					i;
	int					j;
	int					is_space;
	t_q_flags			quote;
}						t_vars;

typedef struct s_ast_tree
{
	t_type_arg			type;
	struct s_ast_tree	*left;
	struct s_ast_tree	*right;
	char				**args;
	int					*q_type;
	int					*is_space;
	t_redirect			*redirect;
	int					arg_counter;
}						t_ast_tree;

typedef struct s_lex_list
{
	char				*s;
	struct s_lex_list	*next;
	t_q_flags			q_type;
	t_type_arg			a_type;
	int					is_space;
}						t_lex_list;

void					dup3(int new, int original);
char					*ft_strjoin(char *s1, char *s2);
char					*ft_itoa(int n);
void					free_lex_list(t_lex_list *token);
void					ft_putstr_fd(int fd, char *str);
void					set_the_arg_type(t_lex_list *token);
void					handle_words(t_ast_tree *node, t_lex_list **token);
char					*ft_strdup(const char *s1);
int						ft_isspace(int c);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
void					add_to_list(t_lex_list **ll, char *str, t_q_flags quote,
							int is_space);
int						ft_strcmp(const char *s1, const char *s2);
size_t					ft_strlen(const char *s);
size_t					ft_strlcpy(char *dst, const char *src, size_t dstsize);

t_lex_list				*new_node(char *str, t_q_flags quote, int is_space);
t_lex_list				*lexing_the_thing(char *str, int *status);

t_ast_tree				*new_ast_node(void);
t_ast_tree				*parse_and_or(t_lex_list **token);
t_ast_tree				*parse_pipe(t_lex_list **token);
t_ast_tree				*parse_parenthesis_cmds(t_lex_list **token);
t_ast_tree				*create_ast_tree(t_lex_list *token);
void					remove_quotes(t_lex_list *token);
int						handle_syntax_errors(t_lex_list *token, int *status);

t_env_list				*new_env_node(void);
void					set_up_env(char **env, t_env_list **env_list);
void					delete_node(t_env_list **env_list, char *key);
void					insert_node_last(t_env_list **d, char *key, char *value,
							int flag);
int						exec_pwd(int n);
int						change_dir(char **args, t_env_list *env);
int						echo(char **args);
int						excute_redirs(t_ast_tree *astree);
t_env_list				*get_env_value(t_env_list *env_list, char *key);
char					**ft_split(char const *s, char c);
char					*ft_strchr(const char *s, int c);
void					free_env_list(t_env_list *env_list);
void					free_tree(t_ast_tree *root);
void					free_args(t_ast_tree *root);
void					free_reds(t_redirect *red);
int						handle_path(char **args, t_env_list *env);
void					excute_the_damn_tree(t_ast_tree *astree, int *status,
							t_env_list **env);
void					dup3(int new, int original);
char					**turn_env_to_chars(t_env_list *env);
int						exec_export(t_env_list **env, char **args);
int						exec_unset(t_env_list **env, char **args);
int						exit_shell(char **args);
int						print_env(t_env_list *env_list);
int						handle_heredoc(t_ast_tree *node, int n,
							t_env_list *env);
char					**join_args_without_spaces(t_ast_tree *node);
void					expand_variables(t_ast_tree *node, t_env_list *env,
							int *status);
int						ft_isalnum(char c);
int						ft_isalpha(char c);
void					add_to_list_redir(t_redirect **ll, t_redirect *t);
t_redirect				*new_node_redir(char *str, int type, int q_type);
void					handlectrlc(int n);
void					ft_handle_sigint(int sig);
void					handle_sig_exec_ve(int sig);
void					heredoc_child_signal(int sig);
void					handle_main_sigs(void);
void					reset_signals(void);
void					ignore_signals(void);
void					print_export(t_env_list *env_list);
void					expanddd(t_ast_tree *node, t_env_list *env, int status);
char					**split_expanded_args(char *expanded_str);
char					**ft_split_for_expand(char const *s, char c);
void					I_HATE_EXPANDING(t_ast_tree *node, t_env_list *env,
							int status);
int						expand_file_name(t_ast_tree *node, t_env_list *env,
							int status);
char					*ft_strtrim(char const *s1, char const *set);
char					*expand_heredoc_line(char *line, t_env_list *env,
							int status);
void					handle_wild_card(t_ast_tree *node);
#endif