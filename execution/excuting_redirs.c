#include "../includes/minishell.h"

static int	handle_out_redir(t_redirect *redir)
{
	int	fd;
	struct stat l;

	if (stat(redir->final_file_name, &l) == 0 && S_ISDIR(l.st_mode))
	{
		ft_putstr_fd(2, "minishell: ");
		ft_putstr_fd(2, redir->final_file_name);
		ft_putstr_fd(2, ": Is a directory\n");
		return (-1);
	}

	fd = open(redir->final_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd == -1)
	{
		ft_putstr_fd(2, "minishell: ");
		ft_putstr_fd(2, redir->final_file_name);
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
	struct stat l;

	if (stat(redir->final_file_name, &l) == 0 && S_ISDIR(l.st_mode))
	{
		ft_putstr_fd(2, "minishell: ");
		ft_putstr_fd(2, redir->final_file_name);
		ft_putstr_fd(2, ": Is a directory\n");
		return (-1);
	}

	fd = open(redir->final_file_name, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd(2, "minishell: ");
		ft_putstr_fd(2, redir->final_file_name);
		ft_putstr_fd(2, " ");
		perror(NULL);
		return (-1);
	}
	dup3(fd, STDIN_FILENO);
	return (0);
}

static int	handle_append_redir(t_redirect *redir)
{
	int	fd;
	struct stat l;

	if (stat(redir->final_file_name, &l) == 0 && S_ISDIR(l.st_mode))
	{
		ft_putstr_fd(2, "minishell: ");
		ft_putstr_fd(2, redir->final_file_name);
		ft_putstr_fd(2, ": Is a directory\n");
		return (-1);
	}

	fd = open(redir->final_file_name, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (fd == -1)
	{
		ft_putstr_fd(2, "minishell: ");
		ft_putstr_fd(2, redir->final_file_name);
		ft_putstr_fd(2, " ");
		perror(NULL);
		return (-1);
	}
	dup3(fd, STDOUT_FILENO);
	return (0);
}

static int	handle_the_here_dawg(t_redirect *redir)
{
	int	fd;

	fd = redir->heredoc;
	if (fd == -1)
	{
		perror(NULL);
		return (-1);
	}
	dup3(fd, STDIN_FILENO);
	return (0);
}

static int	norminete_exc_redir(int stdinn, int stdoutt)
{
	dup3(stdinn, STDIN_FILENO);
	dup3(stdoutt, STDOUT_FILENO);
	return (-1);
}

int	excute_redirs(t_ast_tree *astree)
{
	int	stdinn;
	int	stdoutt;

	stdinn = dup(STDIN_FILENO);
	stdoutt = dup(STDOUT_FILENO);
	t_redirect	*tmp;
	tmp = astree->redirect;
	while (tmp) 
	{
		if (tmp->type == OUT_REDIR) 
		{
			if (handle_out_redir(tmp) == -1) 
				return (norminete_exc_redir(stdinn, stdoutt));
		}
		if (tmp->type == IN_REDIR) 
		{
			if (handle_in_redir(tmp) == -1)
				return (norminete_exc_redir(stdinn, stdoutt));
		}
		if (tmp->type == APPEND) 
		{
			if (handle_append_redir(tmp) == -1) 
				return (norminete_exc_redir(stdinn, stdoutt));
		}
		if (tmp->type == HEREDOC) 
		{
			if (handle_the_here_dawg(tmp) == -1) 
				return (norminete_exc_redir(stdinn, stdoutt));
		}
		tmp = tmp->next; 
	}
	close(stdinn);
	close(stdoutt);
	return (0);
}
