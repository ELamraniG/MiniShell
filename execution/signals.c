/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:33:38 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 16:36:01 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int	g_sigarette;

void	ft_handle_sigint(int sig)
{
	(void)sig;
	ft_putstr_fd(1, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_sigarette = 130;
}

void	handle_sig_exec_ve(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd(1, "\n");
		exit(130);
	}
}

void	heredoc_child_signal(int sig)
{
	(void)sig;
	ft_putstr_fd(1, "\n");
	rl_clear_history();
	exit(1);
}

void	handle_main_sigs(void)
{
	rl_catch_signals = 0;
	signal(SIGINT, ft_handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
