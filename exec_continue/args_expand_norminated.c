/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_expand_norminated.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:51:54 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 19:26:25 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	expand_norm5(t_expd_norm *expd, t_expd2 *expd2)
{
	expd2->dble = ft_split_for_expand(expd2->tmp3, ' ');
	free(expd2->tmp3);
	while (expd2->dble[expd2->j])
	{
		ft_realloc2(expd, expd2->dble[expd2->j]);
		expd->is_space[expd->size - 1] = 1;
		expd->q_type[expd->size - 1] = NQ;
		if (expd2->j == 0)
		{
			if (has_space_at_the_beginning_2(expd->args[expd->size - 1])
				&& expd->size > 1)
				expd->is_space[expd->size - 2] = 1;
		}
		expd2->j++;
		if (expd2->dble[expd2->j] == NULL)
		{
			if (has_space_at_the_end_2(expd->args[expd->size - 1]))
				expd->is_space[expd->size - 1] = 1;
			else
				expd->is_space[expd->size - 1] = 0;
		}
		trim_the_args(expd);
	}
	free(expd2->dble);
}

static void	expand_norm2(t_expd_norm *expd, t_expd2 *expd2, t_ast_tree *astree,
		t_env_list *env)
{
	int	kk;

	expd2->flag = 1;
	expand_norminette_3(expd, expd2, astree);
	expd2->prev_pos = expd2->i;
	kk = expand_norm4(expd, expd2, astree, env);
	if (kk == 0)
	{
		if (expd2->t && expd2->t->value)
		{
			expd2->j = 0;
			expand_norm5(expd, expd2);
		}
	}
	else
	{
		ft_realloc2(expd, expd2->tmp3);
		expd->is_space[expd->size - 1] = 0;
		expd->q_type[expd->size - 1] = astree->is_space[expd->k];
	}
	expd2->prev_pos = expd2->i;
}

static void	expand_norm6(t_expd_norm *expd, t_expd2 *expd2, t_ast_tree *astree)
{
	if (expd2->flag == 0)
	{
		expd2->tmp = ft_substr(astree->args[expd->k], expd2->prev_pos,
				ft_strlen(astree->args[expd->k]) - expd2->prev_pos);
		ft_realloc2(expd, expd2->tmp);
		expd->is_space[expd->size - 1] = astree->is_space[expd->k];
		expd->q_type[expd->size - 1] = astree->q_type[expd->k];
	}
	expd->is_space[expd->size - 1] = astree->is_space[expd->k];
}

static void	expanded_for_single_word(t_expd_norm *expd, t_env_list *env, t_ast_tree *astree)
{
	t_expd2	expd2;

	expd2.i = 0;
	expd2.t = NULL;
	if (astree->q_type[expd->k] == SQ)
	{
		expand_norm1(expd, astree);
		return ;
	}
	expd2.flag = 0;
	expd2.prev_pos = 0;
	while (astree->args[expd->k][expd2.i])
	{
		if (astree->args[expd->k][expd2.i] == '\'')
			expd2.i++;
		else if (astree->args[expd->k][expd2.i] == '$')
			expand_norm2(expd, &expd2, astree, env);
		else
			expd2.i++;
	}
	expand_norm6(expd, &expd2, astree);
}

void	i_hate_expanding(t_ast_tree *astree, t_env_list *env, int status)
{
	t_expd_norm	expd;

	expd.k = 0;
	expd.args = NULL;
	expd.size = 0;
	expd.is_space = NULL;
	expd.k = 0;
	expd.q_type = NULL;
	expd.status = status;
	while (astree->args[expd.k])
	{
		expanded_for_single_word(&expd, env, astree);
		expd.k++;
	}
	expd.k = 0;
	while (astree->args[expd.k])
		free(astree->args[expd.k++]);
	free(astree->args[expd.k]);
	free(astree->args);
	astree->args = expd.args;
	astree->arg_counter = expd.size;
	free(astree->is_space);
	astree->is_space = expd.is_space;
	free(astree->q_type);
	astree->q_type = expd.q_type;
}
