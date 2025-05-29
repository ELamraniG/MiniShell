/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 21:52:18 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 19:32:52 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

typedef struct s_norm_m
{
	int	here_doc;
	int	status;
}	t_norm_m;

typedef struct s_sp_norm
{
	int	i;
	int	j;
	int	k;
	int	wc;
}	t_sp_norm;

typedef struct s_wild_relc
{
	int		i;
	int		*new_is_space;
	char	**new_args;
	int		*tmpint_free;
	char	**tmp_free;
	int		*new_q_type;
}t_wild_relc;

typedef struct s_file_relc
{
	int		i;
	int		*new_is_space;
	char	**new_file_name;
	int		*tmpint_free;
	char	**tmp_free;
	int		*new_q_types;
}t_file_relc;

typedef struct s_file_expd {
	int		k;
	char	**file_name;
	int		size;
	int		*is_space;
	int		*q_types;
		int status;
}t_file_expd;

typedef struct s_hrdc_norm {
	int		i;
	int		prev_pos;
	int		flag;
	char	*str;
	char	*tmp;
	char	*tmp_free;
	char	*key;
	char	*tmp2;
}t_hrdc_norm;

typedef struct s_file_wild_norm {
	int		k;
	char	**args;
	int		size;
	int		*is_space;
	int		*q_types;
}t_file_wild_norm;

typedef struct s_wild_norm {
	int		k;
	char	**args;
	int		size;
	int		*is_space;
	int		*q_type;
}t_wild_norm;

typedef struct s_exp_norm
{
	char	**args;
	int		*is_space;
	int		*q_type;
	int		k;
	int		size;	
	int status;
}t_expd_norm;

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

typedef struct s_stdindo
{
	int					stdinn;
	int					stdoutt;
}						t_stdindo;

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
typedef struct s_expd2 {
	int			i;
	int			j;
	int			flag;
	int			prev_pos;
	char		**dble;
	char		*tmp;
	char		*tmp2;
	char		*tmp3;
	t_env_list	*t;
}t_expd2;

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
typedef struct s_h_word
{
	char		**args;
	int			malc;
	t_redirect	*getting_redirs;
	int			i;
	int			*q_type;
	int			*is_space;
	int			r_type;
}	t_h_word;

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

typedef struct s_relc
{
	int		i;
	int		*new_is_space;
	char	**new_args;
	int		*tmpint_free;
	char	**tmp_free;
	int		*new_q_type;
}			t_relc;

char		*ft_strjoin(char *s1, char *s2);
char		*ft_itoa(int n);
void		free_lex_list(t_lex_list *token);
void		ft_putstr_fd(int fd, char *str);
void		set_the_arg_type(t_lex_list *token);
void		handle_words(t_ast_tree *node, t_lex_list **token);
char		*ft_strdup(const char *s1);
int			ft_isspace(int c);
char		*ft_substr(char const *s, unsigned int start, size_t len);
void		add_to_list(t_lex_list **ll, char *str, t_q_flags quote,
				int is_space);
int			ft_strcmp(const char *s1, const char *s2);
size_t		ft_strlen(const char *s);
int			ft_atoi(const char *s);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
t_lex_list	*new_node(char *str, t_q_flags quote, int is_space);
t_lex_list	*lexing_the_thing(char *str, int *status);
t_ast_tree	*new_ast_node(void);
t_ast_tree	*parse_and_or(t_lex_list **token);
t_ast_tree	*parse_pipe(t_lex_list **token);
t_ast_tree	*parse_parenthesis_cmds(t_lex_list **token);
t_ast_tree	*create_ast_tree(t_lex_list *token);
void		remove_quotes(t_lex_list *token);
int			handle_syntax_errors(t_lex_list *token, int *status);
t_env_list	*new_env_node(void);
void		set_up_env(char **env, t_env_list **env_list);
void		delete_node(t_env_list **env_list, char *key);
void		insert_node(t_env_list **d, char *key, char *value, int flag);
int			exec_pwd(int n);
int			change_dir(char **args, t_env_list *env);
int			echo(char **args);
int			excute_redirs(t_ast_tree *astree);
t_env_list	*get_env_value(t_env_list *env_list, char *key);
char		**ft_split(char const *s, char c);
char		*ft_strchr(const char *s, int c);
void		free_env_list(t_env_list *env_list);
void		free_tree(t_ast_tree *root);
void		free_args(t_ast_tree *root);
void		free_reds(t_redirect *red);
int			handle_path(char **args, t_env_list *env);
void		excute_the_damn_tree(t_ast_tree *astree, int *status,
				t_env_list **env, int in_pipe);
void		dup3(int new, int original);
char		**turn_env_to_chars(t_env_list *env);
int			exec_export(t_env_list **env, char **args);
int			exec_unset(t_env_list **env, char **args);
int			exit_shell(char **args, int status);
int			handle_file_wildcard(t_ast_tree *node);
int			print_env(t_env_list *env_list);
int			handle_heredoc(t_ast_tree *node, int n, t_env_list *env);
char		**join_args_without_spaces(t_ast_tree *node);
int			ft_isalnum(char c);
int			ft_isalpha(char c);
void		add_to_list_redir(t_redirect **ll, t_redirect *t);
void		handlectrlc(int n);
void		ft_handle_sigint(int sig);
void		handle_sig_exec_ve(int sig);
void		heredoc_child_signal(int sig);
void		handle_main_sigs(void);
void		reset_signals(void);
void		ignore_signals(void);
void		print_export(t_env_list *env_list);
char		**ft_split_for_expand(char const *s, char c);
void		i_hate_expanding(t_ast_tree *node, t_env_list *env, int status);
int			expand_file_name(t_ast_tree *node, t_env_list *env, int status);
char		*ft_strtrim(char const *s1, char const *set);
char		*expand_heredoc_line(char *line, t_env_list *env, int status);
void		handle_wild_card(t_ast_tree *node);
void		handle_pipe(t_ast_tree *astree, int *pipes, int *status,
				t_env_list **env);
void		norminette_handle_word1(t_ast_tree *astree, t_env_list **env,
				int *status);
void		norminette_handle_word2(t_ast_tree *astree, t_env_list **env,
				int *status);
char		**join_args_without_spaces(t_ast_tree *astree);
int			is_built_in(char *cmd);
void		join_all_redir_files_without_spaces(t_ast_tree *astree);
int			execute_built_in(char **args, t_env_list **env, int *status);
int			validate_key(char *s);
void		print_msg(char *args);
void		print_export(t_env_list *env_list);
char		*get_value_env(t_env_list**d, char *key);
int			find_key(t_env_list *d, char *key);
void		print_msg1(char *s);
void		print_msg2(char *s);
void		print_msg3(char *s);
void		print_msg4(char *s);
void		free_node(t_env_list *current);
t_env_list	*find_key_node(t_env_list *current, char *key, t_env_list **prev);
int			is_redirection(t_type_arg type);
int			is_and_or(t_type_arg type);
int			check_paren(t_lex_list *token);
void		print_syntax(t_lex_list *token, int *status);
int			is_redir_good(t_lex_list *token, int *status);
int			wach_valid_tokens(t_lex_list *current, int *status);
int			check_empty_parentheses(t_lex_list *token, int *status);
int			word_paren(t_lex_list *token, int *status);
int			check_operator_before_close_paren(t_lex_list *token, int *status);
int			check_pipe_after_open_paren(t_lex_list *token, int *status);
int			check_parenth_problm(t_lex_list *token, int *status);
int			check_close_paren_word(t_lex_list *token, int *status);
int			validate_parentheses(t_lex_list *token, int *status);
int			check_redirs(t_lex_list *token, int *status);
int			chec_pipe(t_lex_list *current, int *status);
int			check_and_or(t_lex_list *current, int *status);
int			check_token(t_lex_list *current, int *status);
int			check_syntax(t_lex_list *token, int *status);
void		arg_type(t_lex_list *token);
int			get_next_quote(char *str, int *i, char c, int *status);
int			is_special(char c);
int			is_quote(char c);
void		skip_spaces(char *str, int *i, int *j);
int			ft_isspace(int c);
void		put_syntax_error(int *status);
int			handle_quote(char *str, t_vars *t, char q, int *status);
int			is_special_for_redirs(int a);
int			wach_skip(t_lex_list *token);
int			calculate_words(t_lex_list *token);
int			count_files(t_lex_list *token);
void		redir_files(t_lex_list **token, t_redirect *redir);
int			handle_out_redir(t_redirect *redir);
int			handle_in_redir(t_redirect *redir);
int			handle_append_redir(t_redirect *redir);
int			handle_the_here_dawg(t_redirect *redir);
int			norminete_exc_redir(int stdinn, int stdoutt);
int			norm_herd_1(int pipe_fd);
char		*join_all_file_names(t_redirect *redir, int *flag);
void		normeeker(char *s, int *pipe_fd);
void		normeee(t_env_list **t, char *tmp);
void		do_the_heredoc(t_redirect *redir, int pipe_fd[2], t_env_list *env);
int			here_d_norm333(int pipe_fd, pid_t pid,
				struct termios *original_term);
void		norm_word_166(t_ast_tree *astree, t_env_list **env,
				char ***env_char);
void		failed_exec_v(t_ast_tree *astree, char ***env_char);
void		ignore_signals(void);
int			has_space_at_the_beginning_2(char *s);
void		trim_the_args(t_expd_norm *expd);
void		expand_norminette_3(t_expd_norm *expd, t_expd2 *expd2,
							t_ast_tree *astree);
int			expand_norm4(t_expd_norm *expd, t_expd2 *expd2, t_ast_tree *astree,
		t_env_list *env);
void		expand_norm1(t_expd_norm *expd, t_ast_tree *astree);
int			has_space_at_the_end_2(char *s);
char		*get_key_2(char *str, t_expd2 *expd2);
void		ft_realloc2(t_expd_norm *expd, char *s);
int			has_space_at_the_end2(char *s);
char		*get_keyy2(char *str, t_expd2 *expd2);
void		trim_the_file_names(t_file_expd *fxpd);
void		ft_realloc_file(t_file_expd *fxpd, char *s);
void		free_relc(t_file_relc *relc);
int			has_space_beginning(char *s);
void		file_expandnorm1(t_file_expd *fxpd, t_redirect *redir);
void		file_expand_norm2(t_file_expd *fxpd, t_expd2 *expd2, t_redirect *redir, t_env_list *env);
void		file_expand_norm3(t_file_expd *fxpd, t_expd2 *expd2, t_redirect *redir);
int 		file_expand_norm4(t_file_expd *fxpd, t_expd2 *expd2, t_redirect *redir, t_env_list *env);
void		file_expand_norm5(t_file_expd *fxpd, t_expd2 *expd2);
void		file_expand_norm6(t_file_expd *fxpd, t_expd2 *expd2, t_redirect *redir);
void		file_expand_norm7(t_file_expd *fxpd, t_expd2 *expd2);
void		expanded_for_single_file(t_file_expd *fxpd, t_env_list *env, t_redirect *redir);
void		check_abg_1(t_redirect *redir, int *abg,int old);

#endif