#include "../includes/minishell.h"

void	change_dir(t_lex_list *d)
{
	if (!d || !d->s || ft_strcmp(d->s, "cd") != 0)
		return ;
	chdir(d->next->s);
}
