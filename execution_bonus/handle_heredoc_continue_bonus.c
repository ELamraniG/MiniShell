/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc_continue.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 22:45:36 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/28 22:45:58 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int	g_sigarette;

int	norm_herd_1(int pipe_fd)
{
	close(pipe_fd);
	g_sigarette = 130;
	return (-1);
}

char	*join_all_file_names(t_redirect *redir, int *flag)
{
	char	*s;
	char	*tmp_free;
	int		i;

	*flag = 0;
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

void	normeeker(char *s, int *pipe_fd)
{
	ft_putstr_fd(pipe_fd[1], s);
	ft_putstr_fd(pipe_fd[1], "\n");
	free(s);
}

void	do_the_heredoc(t_redirect *redir, int pipe_fd[2], t_env_list *env)
{
	char	*tmp;
	char	*s;
	char	*lim;
	int		exp_flag;

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
			s = expand_heredoc_line(tmp, env, g_sigarette);
		else
			s = tmp;
		normeeker(s, pipe_fd);
	}
	close(pipe_fd[1]);
	exit(0);
}

int	here_d_norm333(int pipe_fd, pid_t pid,
		struct termios *original_term)
{
	int	status;

	close(pipe_fd);
	ignore_signals();
	waitpid(pid, &status, 0);
	tcsetattr(STDIN_FILENO, TCSANOW, original_term);
	handle_main_sigs();
	return (status);
}
