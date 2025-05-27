/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                             +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 10:00:00                     #+#    #+#             */
/*   Updated: 2025/05/14 10:00:00                     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int sigarette;
static char	*join_all_file_names(t_redirect *redir,int *flag)
{
	char	*s;
	char	*tmp_free;
	int		i;

	s = ft_strdup(redir->file_name[0]);
	i = 1;
	while (i < redir->file_str_count)
	{
		if (redir->q_types != NQ)
			*flag = 1;
		tmp_free = s;
		s = ft_strjoin(s, redir->file_name[i]);
		free(tmp_free);
		i++;
	}
	return (s);
}

static void	process_heredoc_content(t_redirect *redir, int pipe_fd[2], t_env_list *env)
{
	char	*tmp;
	char	*s;
	char	*lim;
	int exp_flag = 0;

	signal(SIGINT, heredoc_child_signal);
	close(pipe_fd[0]);
	redir->final_file_name = join_all_file_names(redir, &exp_flag);
	lim = redir->final_file_name;
	
	while (1337)
	{
		tmp = readline("> ");
		if (!tmp || !ft_strcmp(tmp, lim))
		{
			free(tmp);
			break ;
		}
		if (exp_flag == 0)
			s = expand_heredoc_line(tmp, env, sigarette);
		else
			s = tmp;
		ft_putstr_fd(pipe_fd[1], s);
		ft_putstr_fd(pipe_fd[1], "\n");
		free(s);
		free(tmp);
	}
	close(pipe_fd[1]);
	exit(0);
}

static int	create_the_dawg(t_redirect *redir, t_env_list *env)
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
		process_heredoc_content(redir, pipe_fd, env);
	close(pipe_fd[1]);
	ignore_signals();
	waitpid(pid, &status, 0);
	tcsetattr(STDIN_FILENO, TCSANOW, &original_term);
	handle_main_sigs();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipe_fd[0]);
		sigarette = 130;
		return (-1);
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
	{
		close(pipe_fd[0]);
		sigarette = 130;
		return (-1);
	}
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
			if (create_the_dawg(redir,env) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}
