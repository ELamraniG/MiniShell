#include "../includes/minishell.h"

// later on msa7 argcounter mmen list

static int	handle_out_redir(t_redirect *redir)
{
	int	fd;

	fd = open(redir->file_name, O_WRONLY | O_CREAT, 0664);
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

static int	handle_in_redir(t_redirect *redir)
{
	int	fd;

	fd = open(redir->file_name, O_RDONLY | 0644);
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

static int	handle_append_redir(t_redirect *redir)
{
	int	fd;

	fd = open(redir->file_name, O_CREAT, O_TRUNC | O_RDONLY | 0644);
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

int	excute_redirs(t_ast_tree *astree)
{
	int	stdinn;
	int	stdoutt;

	stdinn = -1;
	stdoutt = -1;
	stdinn = dup(STDIN_FILENO);
	stdoutt = dup(STDOUT_FILENO);
	while (astree->redirect)
	{
		if (astree->redirect->type == OUT_REDIR)
		{
			if (handle_out_redir(astree->redirect) == -1)
			{
				dup3(stdinn, STDIN_FILENO);
				dup3(stdoutt, STDOUT_FILENO);
				return (-1);
			}
		}
		if (astree->redirect->type == IN_REDIR)
		{
			if (handle_in_redir(astree->redirect) == -1)
			{
				dup3(stdinn, STDIN_FILENO);
				dup3(stdoutt, STDOUT_FILENO);
				return (-1);
			}
		}
		if (astree->redirect->type == APPEND)
		{
			if (handle_append_redir(astree->redirect) == -1)
			{
				dup3(stdinn, STDIN_FILENO);
				dup3(stdoutt, STDOUT_FILENO);
				return (-1);
			}
		}
		astree->redirect = astree->redirect->next;
	}
	close(stdinn);
	close(stdoutt);
	return (0);
}
