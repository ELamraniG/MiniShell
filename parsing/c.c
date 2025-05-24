#include "../includes/minishell.h"

int	ft_isspace(int c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

void	put_syntax_error(int *status)
{
	write(2, "minishell: syntax unclosed quote\n",
		ft_strlen("minishell: syntax unclosed quote\n"));
	*status = 2;
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

static int	get_next_quote(char *str, int *i, char c, int *status)
{
	(*i)++;
	while (str[*i] && str[*i] != c)
		(*i)++;
	if (str[*i] != c)
	{
		*status = 2;
		put_syntax_error(status);
		return (0);
	}
	return (1);
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

int	handle_quote(char *str, t_vars *t, char q, int *status)
{
	if (q == '\'')
		t->quote = SQ;
	else
		t->quote = DQ;
	if (!get_next_quote(str, &(t->i), q, status))
		return (0); 
	if (str[t->i] && ft_isspace(str[t->i + 1]))
		t->is_space = 1;
	t->i++;
	return 1;
}

static void	init_t_vars(t_vars *t, t_lex_list **tokens)
{
	*tokens = NULL;
	t->i = 0;
	t->is_space = 0;
	t->j = 0;
}

int	norm_lexing_the_thing(char *str, t_vars *t, int *status)
{
	if (str[t->i] == '\'' || str[t->i] == '"')
	{
		if (!handle_quote(str, t, str[t->i], status))
			return 0;
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
	return 1;
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
		
		if (!norm_lexing_the_thing(str, &t, status))
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
