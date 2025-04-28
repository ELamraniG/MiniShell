
#include "../includes/minishell.h"

int	ft_isspace(int c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

void	put_syntax_error(void)
{
	write(2, "bash: syntax unclosed quote\n",
		ft_strlen("bash: syntax unclosed quote\n"));
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

static void	get_next_quote(char *str, int *i, char c, int *status)
{
	(*i)++;
	while (str[*i] && str[*i] != c)
		(*i)++;
	if (str[*i] != c)
	{
		*status = 130;
		put_syntax_error();
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

void	skip_spaces(char *str, int *i, int *j)
{
	while (ft_isspace(str[*i]))
	{
		(*i)++;
		(*j)++;
	}
}

void	handle_quote(char *str, t_vars *t, char q, int *status)
{
	if (q == '\'')
		t->quote = SQ;
	else
		t->quote = DQ;
	get_next_quote(str, &(t->i), q, status);
	if (*status != 0)
		return ;
	if (str[t->i] && ft_isspace(str[t->i + 1]))
		t->is_space = 1;
	t->i++;
}

static void	init_t_vars(t_vars *t, t_lex_list **tokens)
{
	*tokens = NULL;
	t->i = 0;
	t->is_space = 0;
	t->j = 0;
}

void	norm_lexing_the_thing(char *str, t_vars *t, int *status)
{
	if (str[t->i] == '\'' || str[t->i] == '"')
	{
		handle_quote(str, t, str[t->i], status);
		if (*status != 0)
			return ;
	}
	else if (str[t->i] == '(' || str[t->i] == ')')
		t->i++;
	else if (is_special(str[t->i]))
	{
		if ((str[t->i]) == str[t->i + 1])
			t->i++;
		t->i++;
	}
	else
	{
		while (str[t->i] && !ft_isspace(str[t->i]) && !is_special(str[t->i])
			&& !is_quote(str[t->i]))
			t->i++;
		if (str[t->i] && ft_isspace(str[t->i]))
			t->is_space = 1;
	}
}

t_lex_list	*lexing_the_thing(char *str, int *status)
{
	t_lex_list	*tokens;
	char		*s;
	t_vars		t;

	init_t_vars(&t, &tokens);
	while (str[t.i])
	{
		t.is_space = 0;
		t.quote = NQ;
		skip_spaces(str, &t.i, &t.j);
		norm_lexing_the_thing(str, &t, status);
		if (*status != 0)
		{
			free_lex_list(tokens);
			return (NULL);
		}
		s = ft_substr(str, t.j, t.i - t.j);
		if (s)
			add_to_list(&tokens, s, t.quote, t.is_space);
		t.j = t.i;
		skip_spaces(str, &t.i, &t.j);
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
