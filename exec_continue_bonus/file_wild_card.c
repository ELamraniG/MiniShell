/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_wild_card.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:44:10 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 22:33:59 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	file_wild_norm1(t_wild_norm *wild, char *current_arg,
		int old_q_type, int point_flag)
{
	char	**all_dirs;
	int		reads_counter;

	reads_counter = file_wild_norm2(current_arg);
	if (reads_counter == 0)
	{
		ft_realloc_filew(wild, current_arg);
		wild->is_space[wild->size - 1] = 1;
		wild->q_type[wild->size - 1] = old_q_type;
	}
	else
	{
		all_dirs = file_wild_norm3(current_arg, reads_counter);
		sort_all_direction(&all_dirs, reads_counter);
		file_wild_norm4(wild, all_dirs, reads_counter, point_flag);
		free(all_dirs);
	}
}

void	handle_single_wild_card(t_wild_norm *wild, char *current_arg,
		int old_is_space, int old_q_type)
{
	int	point_flag;

	point_flag = 0;
	if (current_arg[0] == '.')
		point_flag = 1;
	if (it_has_star(current_arg) && old_q_type == NQ)
		file_wild_norm1(wild, current_arg, old_q_type, point_flag);
	else
		file_wild_norm5(wild, current_arg, old_is_space, old_q_type);
}

void	handle_file_cards(t_redirect *redir)
{
	t_wild_norm	wild;

	wild.k = 0;
	wild.args = NULL;
	wild.size = 0;
	wild.is_space = NULL;
	wild.q_type = NULL;
	while (redir->file_name[wild.k])
	{
		handle_single_wild_card(&wild, redir->file_name[wild.k],
			redir->is_space[wild.k], redir->q_types[wild.k]);
		wild.k++;
	}
	wild.k = 0;
	while (redir->file_name[wild.k])
		free(redir->file_name[wild.k++]);
	free(redir->file_name);
	redir->file_name = wild.args;
	redir->file_str_count = wild.size;
	free(redir->is_space);
	redir->is_space = wild.is_space;
	free(redir->q_types);
	redir->q_types = wild.q_type;
}

static int	check_abg(t_redirect *redir, int old)
{
	int	i;

	i = 0;
	while (i < redir->file_str_count)
		i++;
	if (redir->file_str_count > old)
	{
		ft_putstr_fd(2, "minishell: ");
		ft_putstr_fd(2, redir->file_name[0]);
		ft_putstr_fd(2, ": ambiguous redirect\n");
		return (-1);
	}
	return (1);
}

int	handle_file_wildcard(t_ast_tree *node)
{
	t_redirect	*redir;
	int			old;

	redir = node->redirect;
	while (redir)
	{
		if (redir->type != HEREDOC)
		{
			old = redir->file_str_count;
			handle_file_cards(redir);
			if (check_abg(redir, old) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (1);
}
