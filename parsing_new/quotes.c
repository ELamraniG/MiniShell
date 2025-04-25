# include "../includes/minishell.h"

void	remove_quotes(t_lex_list *token)
{
	while (token)
	{
		if (token->a_type == WORD && token->q_type != NQ)
		{
			char *tmp = token->s;
			token->s = ft_substr(token->s, 1, ft_strlen(token->s) - 2);
			free(tmp);
		}
		token = token->next;
	}
}
