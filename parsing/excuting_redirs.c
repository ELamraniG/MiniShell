#include "../includes/minishell.h"

static int	handle_out_redir(t_redirect *redir, int *status)
{
	int	fd;

	fd = open(redir->file_name, O_WRONLY | O_CREAT | 0644);
	if (fd == -1)
	{
		ft_putstr_fd(2, "minishell: ");
		ft_putstr_fd(2, redir->file_name);
		ft_putstr_fd(2, " ");
		perror(NULL);
		return (-1);
	}
	dup3(fd, STDOUT_FILENO);
	return (0);
}

void	excute_redirs(t_ast_tree *astree, int *status)
{
	int stdinn = -1;
	int stdoutt = -1;
	while (astree->redirect)
	{
		if (astree->redirect->type == OUT_REDIR)
		{
			if (handle_out_redir(astree->redirect, status) == -1)
			{
			}
		}
	}
}