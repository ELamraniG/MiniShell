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
void	handle_sig_doc(int n)
{
	sigarette = 130;
	exit(1);
}

static void	create_a_heredoc(t_redirect *redir, int *sss)
{
	char	*input;
	int		pipes[2];
	int		pid;
	int		status;

	status = 0;
	if (pipe(pipes) == -1)
	{
		perror("pipe");
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		close(pipes[0]);
		close(pipes[1]);
	}
	if (pid == 0)
	{
		signal(SIGINT, handle_sig_doc);
		redir->LAST_DAMN_FILE_NAME = join_all_names(redir->file_name,
				redir->file_str_count);
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
			if (sigarette == 130)
			{
				close(pipes[0]);
				close(pipes[1]);
				exit(130);
			}
		}
		close(pipes[0]);
		close(pipes[1]);
		exit(0);
	}
	close(pipes[1]);
	waitpid(pid, NULL, 0);
	if (WIFEXITED(status))
		*sss = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*sss = 128 + WTERMSIG(status);
	if (*sss != 0)
		close(pipes[0]);
	redir->heredoc = pipes[0];
}

int	handle_heredoc(t_ast_tree *node, int sss)
{
	t_redirect	*redir;

	if (!node)
		return (0);
	if (sss != 0)
	{
		// printf("goingback\n\n\n\n\n\n\n\n\n");
		return (-1);
	}
	if (node->redirect)
	{
		redir = node->redirect;
		while (redir)
		{
			if (redir->type == HEREDOC)
				create_a_heredoc(redir, &sss);
			if (sss != 0)
				return (-1);
			redir = redir->next;
		}
	}
	handle_heredoc(node->left, sss);
	handle_heredoc(node->right, sss);
	return (0);
}
