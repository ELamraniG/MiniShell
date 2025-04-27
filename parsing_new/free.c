#include "../includes/minishell.h"

void	free_lex_list(t_lex_list *token)
{
	t_lex_list	*tmp;

	while (token)
	{
		tmp = token;
		free(token->s);
		token = token->next;
		free(tmp);
	}
}