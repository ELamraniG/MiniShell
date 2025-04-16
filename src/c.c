#include "../includes/minishell.h"

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
		// free_all();
		write(2, "syntax error\n", 13);
	exit(-1);
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

// void	set_precedence_value(t_lex_list *token)
// {
// 	while (token)
// 	{
// 		if ()
// 		token = token->next;
// 	}
// }
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
void	put_syntax_error(void)
{
	write(2, "bash: syntax error near unexpected token\n",
		ft_strlen("bash: syntax error near unexpected token\n"));
	// free_all();
	exit(258);
}

int check_valid_parenthesis(t_lex_list *token)
{
	int f = 0;
	while(token)
	{
		if (token->a_type == OP_PAREN)
			f++;
		else if (token->a_type == CL_PAREN)
			f--;
		if (f < 0)
			return 0;
	}
	if (f == 0)
		return 1;
	return 0;
}

void	handle_syntax_errors(t_lex_list *token)
{
	if (check_valid_parenthesis(token) == 0)
		put_syntax_error();
	if (token->a_type == PIPE || token->a_type == AND || token->a_type == OR)
		put_syntax_error();
	if (token && is_special_opperand(token->a_type) && token->next
		&& is_special_opperand(token->next))
		put_syntax_error();
	while (token)
	{
		if (token->a_type == PIPE && !token->next)
			put_syntax_error();
		if (token->a_type == PIPE && token->next
			&& is_special_for_pipe(token->next))
			put_syntax_error();
		if (token->a_type <= 3 && !token->next)
			put_syntax_error();
		if (token->a_type <= 3 && token->next
			&& is_special_opperand(token->next))
			put_syntax_error();
		token = token->next;
	}
}



int	main(void)
{
	t_lex_list *token = lexing_the_thing("echo test | cat file | echo word");
	while (token)
	{
		printf("`%s` `%d` `%d`\n", token->s, token->q_type, token->is_space);
		arg_type(token);
		printf("arg type is: `%d`\n\n\n", token->a_type);
		token = token->next;
	}
}