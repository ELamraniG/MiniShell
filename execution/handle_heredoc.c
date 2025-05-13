#include "../includes/minishell.h"

char	*join_all_names(char **file_name, int count)
{
	int		i;
	char	*s;
	char	*tmp_free;

	i = 0;
	s = ft_strdup("");
	while (i < count)
	{
		tmp_free = s;
		s = ft_strjoin(s, file_name[i]);
		free(tmp_free);
		i++;
	}
	return (s);
}

static void	create_a_heredoc(t_redirect *redir)
{
	char	*input;
	int		pipes[2];

	redir->LAST_DAMN_FILE_NAME = join_all_names(redir->file_name,
			redir->file_str_count);
	if (pipe(pipes) == -1)
	{
		perror("pipe");
		return ;
	}
	while (1337)
	{
		input = readline("> ");
		if (!input)
		{
			ft_putstr_fd(2,
				"minishell: warning: here-document delimited by end-of-file (wanted ");
			ft_putstr_fd(2, redir->LAST_DAMN_FILE_NAME);
			ft_putstr_fd(2, ")\n");
			break ;
		}
		if (!ft_strcmp(redir->LAST_DAMN_FILE_NAME, input))
		{
			free(input);
			break ;
		}
		ft_putstr_fd(pipes[1], input);
		ft_putstr_fd(pipes[1], "\n");
		free(input);
	}
	close(pipes[1]);
	redir->heredoc = pipes[0];
}

void	handle_heredoc(t_ast_tree *node)
{
	t_redirect	*redir;

	if (!node)
		return ;
	if (node->redirect)
	{
		redir = node->redirect;
		while (redir)
		{
			if (redir->type == HEREDOC)
				create_a_heredoc(redir);
			redir = redir->next;
		}
	}
	handle_heredoc(node->left);
	handle_heredoc(node->right);
}
