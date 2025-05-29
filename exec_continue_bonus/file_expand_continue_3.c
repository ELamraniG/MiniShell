/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_expand_continue_3.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:41:08 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 21:41:08 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	file_expand_norm5(t_file_expd *fxpd, t_expd2 *expd2)
{
	expd2->dble = ft_split_for_expand(expd2->tmp3, ' ');
	expd2->j = 0;
	free(expd2->tmp3);
	while (expd2->dble[expd2->j])
	{
		ft_realloc_file(fxpd, expd2->dble[expd2->j]);
		fxpd->is_space[fxpd->size - 1] = 1;
		fxpd->q_types[fxpd->size - 1] = NQ;
		if (expd2->j == 0)
		{
			if (has_space_beginning(fxpd->file_name[fxpd->size - 1])
				&& fxpd->size > 1)
				fxpd->is_space[fxpd->size - 2] = 1;
		}
		expd2->j++;
		if (expd2->dble[expd2->j] == NULL)
		{
			if (has_space_at_the_end2(fxpd->file_name[fxpd->size - 1]))
				fxpd->is_space[fxpd->size - 1] = 1;
			else
				fxpd->is_space[fxpd->size - 1] = 0;
		}
		trim_the_file_names(fxpd);
	}
	free(expd2->dble);
}

void	file_expand_norm2(t_file_expd *fxpd, t_expd2 *expd2, t_redirect *redir,
		t_env_list *env)
{
	int	kk;

	expd2->flag = 1;
	file_expand_norm3(fxpd, expd2, redir);
	expd2->prev_pos = expd2->i;
	kk = file_expand_norm4(fxpd, expd2, redir, env);
	if (kk == 0)
	{
		if (expd2->t && expd2->t->value)
			file_expand_norm5(fxpd, expd2);
	}
	else
	{
		ft_realloc_file(fxpd, expd2->tmp3);
		fxpd->is_space[fxpd->size - 1] = 0;
		fxpd->q_types[fxpd->size - 1] = redir->is_space[fxpd->k];
	}
	expd2->prev_pos = expd2->i;
}

void	file_expand_norm6(t_file_expd *fxpd, t_expd2 *expd2, t_redirect *redir)
{
	if (expd2->flag == 0)
	{
		expd2->tmp = ft_substr(redir->file_name[fxpd->k], expd2->prev_pos,
				ft_strlen(redir->file_name[fxpd->k]) - expd2->prev_pos);
		ft_realloc_file(fxpd, expd2->tmp);
		fxpd->is_space[fxpd->size - 1] = redir->is_space[fxpd->k];
		fxpd->q_types[fxpd->size - 1] = redir->q_types[fxpd->k];
	}
	fxpd->is_space[fxpd->size - 1] = redir->is_space[fxpd->k];
}

void	expanded_for_single_file(t_file_expd *fxpd, t_env_list *env,
		t_redirect *redir)
{
	t_expd2	expd2;

	expd2.i = 0;
	expd2.t = NULL;
	if (redir->q_types[fxpd->k] == SQ)
	{
		file_expandnorm1(fxpd, redir);
		return ;
	}
	expd2.flag = 0;
	expd2.prev_pos = 0;
	while (redir->file_name[fxpd->k][expd2.i])
	{
		if (redir->file_name[fxpd->k][expd2.i] == '\'')
			expd2.i++;
		else if (redir->file_name[fxpd->k][expd2.i] == '$')
			file_expand_norm2(fxpd, &expd2, redir, env);
		else
			expd2.i++;
	}
	file_expand_norm6(fxpd, &expd2, redir);
}

void	check_abg_1(t_redirect *redir, int *abg, int old)
{
	int	i;
	int	count_empty;

	count_empty = 0;
	i = 0;
	*abg = 0;
	if (old == redir->file_str_count)
		*abg = 0;
	i = 0;
	while (i < redir->file_str_count)
	{
		if (redir->file_name[i][0] == 0)
			count_empty++;
		i++;
	}
	if (count_empty == redir->file_str_count)
		*abg = -1;
}
