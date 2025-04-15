#include "../includes/minishell.h"

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

int	main(void)
{
	t_lex_list *token = lexing_the_thing("echo \"|| > <");
	while (token)
	{
		printf("|%s| |%d| |%d|\n", token->s, token->q_type, token->is_space);
		token = token->next;
	}
}