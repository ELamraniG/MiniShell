/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wild_cards_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:37:16 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 23:09:34 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

void	sort_all_dirs_wild(char ***all_dirs, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	j = 0;
	if (count == 1)
		return ;
	while (i < count)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(all_dirs[0][i], all_dirs[0][j]) > 0)
			{
				tmp = all_dirs[0][i];
				all_dirs[0][i] = all_dirs[0][j];
				all_dirs[0][j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	handle_single_wild_card2(t_wild_norm *wild, char *current_arg,
		int old_is_space, int old_q_type)
{
	int	point_flag;

	point_flag = 0;
	if (current_arg[0] == '.')
		point_flag = 1;
	if (it_has_etoil2(current_arg) && old_q_type == NQ)
		wild_card_norm1(wild, current_arg, old_q_type, point_flag);
	else
		wild_card_norm5(wild, current_arg, old_is_space, old_q_type);
}

void	handle_wild_card(t_ast_tree *node)
{
	t_wild_norm	wild;

	wild.k = 0;
	wild.args = NULL;
	wild.size = 0;
	wild.is_space = NULL;
	wild.q_type = NULL;
	while (node->args[wild.k])
	{
		handle_single_wild_card2(&wild, node->args[wild.k],
			node->is_space[wild.k], node->q_type[wild.k]);
		wild.k++;
	}
	wild.k = 0;
	while (node->args[wild.k])
		free(node->args[wild.k++]);
	free(node->args);
	node->args = wild.args;
	node->arg_counter = wild.size;
	free(node->is_space);
	node->is_space = wild.is_space;
	free(node->q_type);
	node->q_type = wild.q_type;
}
