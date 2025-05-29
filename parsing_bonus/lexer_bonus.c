/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 21:33:29 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/28 21:43:55 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	skip_spaces(char *str, int *i, int *j)
{
	while (ft_isspace(str[*i]))
	{
		(*i)++;
		(*j)++;
	}
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
			return (0);
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
	return (1);
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
