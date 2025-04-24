#include "../includes/minishell.h"

void	remove_quotes(t_lex_list *token)
{
	while(token && token->a_type == WORD && token->q_type != NQ)
	{
		token->s = ft_substr(token->s, 1, ft_strlen(token->s) - 1);
		token = token->s;
	}
}

