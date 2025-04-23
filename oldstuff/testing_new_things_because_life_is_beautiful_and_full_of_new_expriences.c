#include "includes/minishell.h"

int			ft_isspace(int c);
char		*ft_substr(char const *s, unsigned int start, size_t len);

size_t		ft_strlen(const char *s);

size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);

char		*ft_strdup(const char *s1);

int			ft_strcmp(const char *s1, const char *s2);

t_lex_list	*new_node(char *str, t_q_flags quote, int is_space);

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

t_ast_tree *parse_pipe(t_lex_list **token);
t_ast_tree *parse_parenthesis_cmds(t_lex_list **token);

t_ast_tree	*new_ast_node(t_lex_list *node)
{
	t_ast_tree	*tree_node;

	tree_node = malloc(sizeof(t_ast_tree));
	tree_node->content = node->s;
	return (tree_node);
}
t_ast_tree *parse_and_or(t_lex_list **token)
{
	t_ast_tree *left = parse_pipe(token);
	while (*token && ((*token)->a_type == AND || (*token)->a_type == OR))
	{
		t_ast_tree *root = new_ast_node(*token);
		root->type = (*token)->a_type;
		(*token) = (*token)->next;
		t_ast_tree *right = parse_pipe(token);
		root->left = left;
		root->right = right;
		left = root;
	}
	return (left);
}

t_ast_tree *parse_pipe(t_lex_list **token)
{
	t_ast_tree *left = parse_parenthesis_cmds(token);
	while (*token && (*token)->a_type == PIPE)
	{
		t_ast_tree *root = new_ast_node(*token);
		root->type = (*token)->a_type;
		(*token) = (*token)->next;
		t_ast_tree *right = parse_parenthesis_cmds(token);
		root->left = left;
		root->right = right;
		left = root;
	}
	return (left);
}

t_ast_tree *parse_parenthesis_cmds(t_lex_list **token)
{
	if (*token == NULL)
		return NULL;
	if ((*token)->a_type == WORD)
	{
		t_ast_tree *root = new_ast_node(*token);
		root->type = (*token)->a_type;
		root->left = NULL;
		root->right = NULL;
		*token = (*token)->next;
		return (root);
	}
	if (*token && ((*token)->a_type == OP_PAREN))
	{
		*token = (*token)->next;
		t_ast_tree *root = parse_and_or(token);
		if (*token == NULL || (*token)->a_type != CL_PAREN)
			return root;
		*token = (*token)->next;
		return root;
	}
	return NULL;
}


t_ast_tree *create_ast_tree(t_lex_list *token)
{
	return (parse_and_or(&token));
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
char *tests[] = {
    // Test 1: Deeply nested parentheses with mixed operators
    "( (ls && cat) || (echo && grep) ) | wc && sort",

    // Test 2: Excessive pipe chaining with mixed && and ||
    "ls | cat | echo | grep && wc && sort || uniq || head",

    // Test 3: Multiple nested logical operators with a mix of precedence
    "(ls || cat) && (echo && ( (grep) || wc) ) | (sort || uniq)",

    // Test 4: Operator precedence and parentheses combined in an extreme case
    "(ls || (cat && echo) ) && ( (grep || wc) | sort) && head",

    // Test 5: Multiple nested parentheses with mixed &&, ||, and pipes
    "( ( (ls && cat) | (echo || grep) ) && (wc || sort) ) || head"
};

	t_lex_list	*token;
	t_ast_tree	*tree;
	int			i = 0;

	while (i < 5)
	{
		printf("\n=============================\n");
		printf("TEST %d: %s\n", i + 1, tests[i]);
		printf("=============================\n");
		token = lexing_the_thing(tests[i]);
		set_the_arg_type(token);
		handle_syntax_errors(token);
		tree = create_ast_tree(token);
		print_ast_tree(tree, 0, 0);
		i++;
	}
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
