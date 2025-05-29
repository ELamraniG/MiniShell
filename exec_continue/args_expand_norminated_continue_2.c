/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_expand_norminated_continue_2.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:35:29 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 19:09:32 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	has_space_at_the_beginning_2(char *s)
{
	int	strl;

	if (!s)
		return (0);
	strl = ft_strlen(s);
	if (strl == 0)
		return (0);
	if (s[0] == ' ')
		return (1);
	return (0);
}

void	expand_norm1(t_expd_norm *expd, t_ast_tree *astree)
{
	ft_realloc2(expd, ft_strdup(astree->args[expd->k]));
	expd->is_space[expd->size - 1] = astree->is_space[expd->k];
	expd->q_type[expd->size - 1] = astree->q_type[expd->k];
}

void	expand_norminette_3(t_expd_norm *expd, t_expd2 *expd2,
							t_ast_tree *astree)
{
	expd2->tmp = ft_substr(astree->args[expd->k], expd2->prev_pos, expd2->i
			- expd2->prev_pos);
	if (expd2->tmp[0] != 0)
	{
		ft_realloc2(expd, expd2->tmp);
		expd->is_space[expd->size - 1] = 0;
		expd->q_type[expd->size - 1] = astree->q_type[expd->k];
	}
	else
		free(expd2->tmp);
}

void	expand_norm7(t_expd_norm *expd, t_expd2 *expd2)
{
	expd2->tmp3 = ft_strdup("");
	ft_realloc2(expd, expd2->tmp3);
	free(expd2->tmp2);
	expd->is_space[expd->size - 1] = 0;
	expd->q_type[expd->size - 1] = DQ;
	expd2->prev_pos = expd2->i;
}

int	expand_norm4(t_expd_norm *expd, t_expd2 *expd2, t_ast_tree *astree,
		t_env_list *env, int status)
{
	expd2->tmp2 = get_key_2(astree->args[expd->k], expd2);
	if (astree->args[expd->k][expd2->i])
		expd2->flag = 0;
	if (!ft_strcmp(expd2->tmp2, "?"))
	{
		free(expd2->tmp2);
		expd2->tmp3 = ft_itoa(status);
		expd2->flag = 0;
		return (1);
	}
	else if (!ft_strcmp(expd2->tmp2, "$"))
	{
		free(expd2->tmp2);
		expd2->tmp3 = ft_strdup("$");
		expd2->flag = 0;
		return (1);
	}
	else
	{
		expd2->t = get_env_value(env, expd2->tmp2);
		if (!expd2->t)
			return (expand_norm7(expd, expd2), 0);
		else
			expd2->tmp3 = ft_strdup(expd2->t->value);
	}
	return (free(expd2->tmp2), 0);
}
