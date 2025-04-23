#include "includes/minishell.h"

int			ft_isspace(int c);
char		*ft_substr(char const *s, unsigned int start, size_t len);

size_t		ft_strlen(const char *s);

size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);

char		*ft_strdup(const char *s1);

int			ft_strcmp(const char *s1, const char *s2);

t_lex_list	*new_node(char *str, t_q_flags quote, int is_space);
t_ast_tree	*new_ast_node(void);

void		add_to_list(t_lex_list **ll, char *str, t_q_flags quote,
				int is_space);
void		put_syntax_error(void);

void		arg_type(t_lex_list *token);

void		get_next_quote(char *str, int *i, char c);
int			is_special(char c);

int			is_quote(char c);
t_lex_list	*lexing_the_thing(char *str);

void		set_the_arg_type(t_lex_list *token);

void		set_precedence_value(t_lex_list *token);

int			is_special_opperand(int n);

int			is_special_for_pipe(int n);
int			is_special_for_specials(int n);

int			check_valid_parenthesis(t_lex_list *token);

void		handle_syntax_errors(t_lex_list *token);

t_lex_list	*find_last_pipe(t_lex_list *start);

void		create_tree(t_lex_list *token);

t_lex_list	*find_matching_closed_parenthesis(t_lex_list *token)
{
	int	i;

	i = 0;
	while (token)
	{
		if (token->a_type == OP_PAREN)
			i++;
		else if (token->a_type == CL_PAREN)
			i--;
		
		if (i == 0)
			break;
		token = token->next;
	}
	return (token);
}
int	get_precedence(t_type_arg type)
{
	if (type == WORD)
		return (1);
	else if (type == PIPE)
		return (2);
	else if (type == AND || type == OR)
		return (3);
	return (0);
}
t_lex_list	*highest_prec(t_lex_list *first, t_lex_list *last)
{
	t_lex_list	*h_prec;

	if (!first)
		return (NULL);
	h_prec = first;
	while (first != last)
	{
		if(first->a_type == OP_PAREN)
		{
			t_lex_list *tmp = find_matching_closed_parenthesis(first);
			while (first != tmp)
				first = first->next;
		}
		if (get_precedence(first->a_type) >= get_precedence(h_prec->a_type))
			h_prec = first;
		first = first->next;
	}
	return (h_prec);
}

t_ast_tree	*create_ast_node(t_lex_list *token)
{
	t_ast_tree	*ast_node;

	if (!token)
		return (NULL);
	ast_node = malloc(sizeof(t_ast_tree));
	ast_node->content = token->s;
	ast_node->type = token->a_type;
	ast_node->right = NULL;
	ast_node->left = NULL;
	return (ast_node);
}

t_ast_tree	*create_ast_tree(t_lex_list *current, t_lex_list *last)
{
	t_lex_list	*h_precedence;
	t_ast_tree	*root;
	t_ast_tree	*right;
	t_ast_tree	*left;

	if (!current)
		return (NULL);
	root = NULL;
	right = NULL;
	left = NULL;
	if (current->a_type == OP_PAREN)
	{
		if (current->next && current->next->next && current->next->next == last)
			return (create_ast_node(current->next));
		else
		{
			h_precedence = highest_prec(current->next,
					find_matching_closed_parenthesis(current));
			printf("|%s|\n", h_precedence->s);
			left = create_ast_tree(current->next, h_precedence);
			right = create_ast_tree(h_precedence->next,
					find_matching_closed_parenthesis(current));
			root = create_ast_node(h_precedence);
			root->left = left;
			root->right = right;
		}
		return (root);
	}
	if (current->next == last)
		return (create_ast_node(current));
	h_precedence = highest_prec(current, last);
	root = create_ast_node(h_precedence);
	left = create_ast_tree(current, h_precedence);
	right = create_ast_tree(h_precedence->next, last);
	root->left = left;
	root->right = right;
	return (root);
}

t_ast_tree	*we_tree(t_lex_list *current, t_lex_list *last)
{
	t_ast_tree	*root;

	root = create_ast_tree(current, last);
	return (root);
}

void	print_tabs(int depth)
{
	while (depth--)
		write(1, "   ", 3);
}

void	print_ast_tree(t_ast_tree *node, int position, int depth)
{
	if (!node)
		return ;
	print_tabs(depth);
	if (position == 0)
		printf("root: ");
	else if (position == 1)
		printf("left: ");
	else
		printf("right: ");
	printf("%s\n", node->content);
	print_ast_tree(node->left, 1, depth + 1);
	print_ast_tree(node->right, 2, depth + 1);
}

// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh

// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
#include <stdbool.h>

int	main(void)
{
	t_lex_list	*token;
	t_lex_list	*token2;
	t_ast_tree	*lopo;

	token = lexing_the_thing("(ls | cat)");
	token2 = token;
	set_the_arg_type(token);
	// handle_syntax_errors(token);
	while (token)
	{
		printf("Token: /%s\\ Type: %d\n", token->s, token->a_type);
		token = token->next;
	}
	lopo = create_ast_tree(token2, NULL);
	print_ast_tree(lopo, 0, 0);
}

// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh
// ghrw'khkrwh;'krwhkrwh

int	ft_isspace(int c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ss;

	if (!s)
		return (NULL);
	if (len == 0)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len + start > ft_strlen(s))
		len = ft_strlen(s) - start;
	ss = malloc(sizeof(char) * (len + 1));
	if (!ss)
		return (NULL);
	ft_strlcpy(ss, s + start, len + 1);
	return (ss);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	slen;

	if (!dst || !src)
		return (0);
	i = 0;
	slen = ft_strlen(src);
	if (dstsize == 0)
		return (slen);
	while (i < dstsize - 1 && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
	return (slen);
}

char	*ft_strdup(const char *s1)
{
	char	*snew;
	int		slen;

	slen = ft_strlen(s1);
	snew = malloc(sizeof(char) * (slen + 1));
	if (snew == NULL)
		return (NULL);
	ft_strlcpy(snew, s1, slen + 1);
	return (snew);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (0);
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

t_lex_list	*new_node(char *str, t_q_flags quote, int is_space)
{
	t_lex_list	*p;

	p = malloc(sizeof(t_lex_list));
	if (!p)
		return (NULL);
	p->s = str;
	p->q_type = quote;
	p->is_space = is_space;
	p->next = NULL;
	return (p);
}

void	add_to_list(t_lex_list **ll, char *str, t_q_flags quote, int is_space)
{
	t_lex_list	*tmp;
	t_lex_list	*t;

	if (!ll)
		return ;
	tmp = *ll;
	while (tmp && tmp->next)
		tmp = (tmp)->next;
	t = new_node(str, quote, is_space);
	if (!t)
		return ;
	if (!tmp)
	{
		*ll = t;
		return ;
	}
	(tmp)->next = t;
}
void	put_syntax_error(void)
{
	write(2, "bash: syntax error near unexpected token\n",
		ft_strlen("bash: syntax error near unexpected token\n"));
	// free_all();
	exit(258);
}

void	arg_type(t_lex_list *token)
{
	if (ft_strcmp(token->s, "(") == 0)
		token->a_type = OP_PAREN;
	else if (ft_strcmp(token->s, ")") == 0)
		token->a_type = CL_PAREN;
	else if (ft_strcmp(token->s, "||") == 0)
		token->a_type = OR;
	else if (ft_strcmp(token->s, "&&") == 0)
		token->a_type = AND;
	else if (ft_strcmp(token->s, "|") == 0)
		token->a_type = PIPE;
	else if (ft_strcmp(token->s, "<<") == 0)
		token->a_type = HEREDOC;
	else if (ft_strcmp(token->s, ">>") == 0)
		token->a_type = APPEND;
	else if (ft_strcmp(token->s, ">") == 0)
		token->a_type = OUT_REDIR;
	else if (ft_strcmp(token->s, "<") == 0)
		token->a_type = IN_REDIR;
	else
		token->a_type = WORD;
}

void	get_next_quote(char *str, int *i, char c)
{
	(*i)++;
	while (str[*i] && str[*i] != c)
		(*i)++;
	if (str[*i] != c)
	{
		// free_all();
		put_syntax_error();
		exit(258);
	}
}
int	is_special(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	if (c == '&' || c == '(' || c == ')')
		return (1);
	return (0);
}

int	is_quote(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}
t_lex_list	*lexing_the_thing(char *str)
{
	int			i;
	int			j;
	t_lex_list	*tokens;
	char		*s;
	int			is_space;
	t_q_flags	quote;

	i = 0;
	is_space = 0;
	i = 0;
	j = 0;
	tokens = NULL;
	while (str[i])
	{
		is_space = 0;
		while (ft_isspace(str[i]))
		{
			j++;
			i++;
		}
		if (str[i] == '\'')
		{
			quote = SQ;
			get_next_quote(str, &i, '\'');
			if (str[i] && ft_isspace(str[i + 1]))
				is_space = 1;
			i++;
		}
		else if (str[i] == '"')
		{
			quote = DQ;
			get_next_quote(str, &i, '"');
			if (str[i] && ft_isspace(str[i + 1]))
				is_space = 1;
			i++;
		}
		else if (is_special(str[i]))
		{
			quote = NQ;
			if ((str[i]) == str[i + 1])
				i++;
			i++;
		}
		else
		{
			quote = NQ;
			while (str[i] && !ft_isspace(str[i]) && !is_special(str[i])
				&& !is_quote(str[i]))
				i++;
			if (str[i] && ft_isspace(str[i]))
				is_space = 1;
		}
		s = ft_substr(str, j, i - j);
		if (s)
			add_to_list(&tokens, s, quote, is_space);
		if (!str[i])
			break ;
		j = i;
	}
	return (tokens);
}

void	set_the_arg_type(t_lex_list *token)
{
	while (token)
	{
		arg_type(token);
		token = token->next;
	}
}

int	is_special_opperand(int n)
{
	if (n != WORD)
		return (1);
	return (0);
}

int	is_special_for_pipe(int n)
{
	if (n != WORD && n != OP_PAREN)
		return (1);
	return (0);
}

int	is_special_for_specials(int n)
{
	if (n != WORD && n != OP_PAREN && n != CL_PAREN)
		return (1);
	return (0);
}

int	check_valid_parenthesis(t_lex_list *token)
{
	int	f;

	f = 0;
	while (token)
	{
		if (token->a_type == OP_PAREN)
			f++;
		else if (token->a_type == CL_PAREN)
			f--;
		if (f < 0)
			return (0);
		token = token->next;
	}
	if (f == 0)
		return (1);
	return (0);
}

void	handle_syntax_errors(t_lex_list *token)
{
	if (!token)
		return ;
	if (check_valid_parenthesis(token) == 0)
		put_syntax_error();
	if (token->a_type == PIPE || token->a_type == AND || token->a_type == OR)
		put_syntax_error();
	if (token && is_special_opperand(token->a_type) && token->next
		&& is_special_opperand(token->next->a_type))
		put_syntax_error();
	while (token)
	{
		if (is_special_opperand(token->a_type) && token->next
			&& (token->next->a_type == HEREDOC
				|| token->next->a_type == IN_REDIR))
		{
			token = token->next;
			continue ;
		}
		else if (is_special_for_specials(token->a_type) && token->next
			&& is_special_for_specials(token->next->a_type))
			put_syntax_error();
		else if (token->a_type == PIPE && !token->next)
			put_syntax_error();
		else if (token->a_type == PIPE && token->next
			&& token->next->a_type != IN_REDIR
			&& is_special_for_pipe(token->next->a_type))
			put_syntax_error();
		else if (token->a_type <= 3 && !token->next)
			put_syntax_error();
		else if (token->a_type <= 3 && token->next
			&& is_special_opperand(token->next->a_type))
			put_syntax_error();
		else if (token->a_type <= 7 && !token->next)
			put_syntax_error();
		token = token->next;
	}
}

t_lex_list	*find_last_pipe(t_lex_list *start)
{
	t_lex_list *cur;
	t_lex_list *last_pipe;

	cur = start;
	last_pipe = NULL;
	while (cur)
	{
		if (cur->a_type == PIPE)
			last_pipe = cur;
		cur = cur->next;
	}
	return (last_pipe);
}