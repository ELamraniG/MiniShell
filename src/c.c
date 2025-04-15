#include "../includes/minishell.h"

void	get_next_quote(char *str, int *i, char c)
{
	(*i)++;
	while (str[*i] && str[*i] != c)
		(*i)++;
}
int	is_special(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	if (c == '&' || c == '(' || c == ')')
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
			printf("before|%c| |i=%d|\n", str[i], i);
			while (str[i] && !ft_isspace(str[i]) && is_special(str[i]))
				i++;
			if (str[i] && ft_isspace(str[i]))
				is_space = 1;
			printf("after|%c| |i=%d|\n", str[i], i);
		}
		else if (str[i] == '"')
		{
			quote = DQ;
			get_next_quote(str, &i, '"');
			while (str[i] && !ft_isspace(str[i]) && is_special(str[i]))
				i++;
			if (str[i] && ft_isspace(str[i]))
				is_space = 1;
		}
		else if (is_special(str[i]))
		{
			i++;
			if (is_special(str[i]))
				i++;
		}
		else
		{
			quote = NQ;
			while (str[i] && !ft_isspace(str[i]))
				i++;
			if (str[i] && ft_isspace(str[i]))
				is_space = 1;
		}
		if (!str[j])
			break ;
		s = ft_substr(str, j, i - j);
		add_to_list(&tokens, s, quote, is_space);
		if (!str[i])
			break ;
		j = i;
		j++;
		i++;
	}
	return (tokens);
}

int	main(void)
{
	t_lex_list *a = lexing_the_thing("echo  \"abcd\"\"8\">1|ls>>fe&&ll	d 'e' f");
	while (a)
	{
		printf("|%s| |q_type = %d| |is_space =%d|--\n", a->s, a->q_type,
			a->is_space);
		a = a->next;
	}
}