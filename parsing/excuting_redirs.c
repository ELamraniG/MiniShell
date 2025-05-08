#include "../includes/minishell.h"

static void handle_out_redir(t_redirect *redir,int *status)
{
	char *file = 
}

void excute_redirs(t_ast_tree *astree,int *status)
{
	while(astree->redirect)
	{
		if (astree->redirect->type == OUT_REDIR)
		{
			handle_out_redir(astree->redirect,status);
		}
	}
}