/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excuting_redirs_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 21:06:09 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 23:09:49 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

static int	norminete_redir(t_redirect *tmp, int stdinn, int stdoutt)
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
	return (0);
}

int	excute_redirs(t_ast_tree *astree)
{
	int			stdinn;
	int			stdoutt;
	t_redirect	*tmp;
	int			i;

	stdinn = dup(STDIN_FILENO);
	stdoutt = dup(STDOUT_FILENO);
	tmp = astree->redirect;
	while (tmp)
	{
		i = norminete_redir(tmp, stdinn, stdoutt);
		if (i == -1)
			return (i);
		tmp = tmp->next;
	}
	close(stdinn);
	close(stdoutt);
	return (0);
}
