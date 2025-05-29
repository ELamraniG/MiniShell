/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 22:17:04 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/28 22:46:06 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	create_the_dawg(t_redirect *redir, t_env_list *env)
{
	int				pipe_fd[2];
	struct termios	original_term;
	pid_t			pid;
	int				status;

	tcgetattr(STDIN_FILENO, &original_term);
	if (pipe(pipe_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	if (pid == 0)
		do_the_heredoc(redir, pipe_fd, env);
	status = here_d_norm333(pipe_fd[1], pid, &original_term);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (norm_herd_1(pipe_fd[0]));
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return (norm_herd_1(pipe_fd[0]));
	redir->heredoc = pipe_fd[0];
	return (0);
}

int	handle_heredoc(t_ast_tree *node, int n, t_env_list *env)
{
	t_redirect	*redir;

	if (!node)
		return (0);
	if (node->left && handle_heredoc(node->left, n, env) == -1)
		return (-1);
	if (node->right && handle_heredoc(node->right, n, env) == -1)
		return (-1);
	redir = node->redirect;
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			if (create_the_dawg(redir, env) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}
