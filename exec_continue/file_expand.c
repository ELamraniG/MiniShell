#include "../includes/minishell.h"



static void	file(t_redirect *redir, t_file_expd *fxpd)
{
	redir->file_name = fxpd->file_name;
	redir->file_str_count = fxpd->size;
	free(redir->is_space);
	redir->is_space = fxpd->is_space;
	free(redir->q_types);
	redir->q_types = fxpd->q_types;
}

static int	i_hate_expanding_file(t_redirect *redir, t_env_list *env,
		int status)
{
	t_file_expd fxpd;
	int old;
	int abg = 0;
	char **old_fl = redir->file_name;
	fxpd.k = 0;
	fxpd.file_name = NULL;
	fxpd.size = 0;
	fxpd.is_space = NULL;
	fxpd.q_types = NULL;
	fxpd.status = status;
	while (fxpd.k < redir->file_str_count)
	{
		old = redir->file_str_count;
		expanded_for_single_file(&fxpd, env, redir);
		fxpd.k++;
	}
	fxpd.k = 0;
	file(redir, &fxpd);
	check_abg_1(redir, &abg,old);
	if (abg != 0)
	{
		ft_putstr_fd(2,"minishell: ");
		ft_putstr_fd(2,old_fl[0]);
		ft_putstr_fd(2,": ambiguous redirect\n");
	}
	while (fxpd.k < old)
		free(old_fl[fxpd.k++]);
	return (free(old_fl), abg);
}

int	expand_file_name(t_ast_tree *node, t_env_list *env, int status)
{
	t_redirect *redir = node->redirect;
	int abg = 0;
	while (redir)
	{
		if (redir->type != HEREDOC)
		{
			abg = i_hate_expanding_file(redir, env, status);
			if (abg != 0)
			
				return -1;
		}
		redir = redir->next;
	}
	return (0);
}
