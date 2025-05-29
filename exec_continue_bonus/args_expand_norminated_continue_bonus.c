/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_expand_norminated_continue_bonus.c            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:08:33 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 23:09:12 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

void	free_reallocc(t_relc *relc)
{
	free(relc->tmpint_free);
	free(relc->tmp_free);
}

void	ft_realloc2(t_expd_norm *expd, char *s)
{
	t_relc	relc;

	relc.i = 0;
	relc.new_is_space = malloc(sizeof(int) * (expd->size + 1));
	relc.new_q_type = malloc(sizeof(int) * (expd->size + 1));
	relc.new_args = malloc(sizeof(char *) * (expd->size + 2));
	relc.new_args[expd->size] = 0;
	while (relc.i < expd->size)
	{
		relc.new_q_type[relc.i] = expd->q_type[relc.i];
		relc.new_is_space[relc.i] = expd->is_space[relc.i];
		relc.new_args[relc.i] = expd->args[relc.i];
		relc.i++;
	}
	relc.new_args[relc.i] = (s);
	relc.new_args[expd->size + 1] = 0;
	(expd->size)++;
	relc.tmp_free = expd->args;
	expd->args = relc.new_args;
	relc.tmpint_free = expd->is_space;
	expd->is_space = relc.new_is_space;
	free(relc.tmpint_free);
	relc.tmpint_free = expd->q_type;
	expd->q_type = relc.new_q_type;
	return (free_reallocc(&relc));
}

void	trim_the_args(t_expd_norm *expd)
{
	char	*tmp_free;

	tmp_free = expd->args[expd->size - 1];
	expd->args[expd->size - 1] = ft_strtrim(expd->args[expd->size - 1], " ");
	free(tmp_free);
	return ;
}

char	*get_key_2(char *str, t_expd2 *expd2)
{
	int	len;

	(expd2->i)++;
	expd2->tmp = NULL;
	len = 0;
	if (str[expd2->i] == '?')
	{
		(expd2->i)++;
		return (ft_strdup("?"));
	}
	if (!ft_isalpha(str[expd2->i]))
		return (ft_strdup("$"));
	while (str[expd2->i])
	{
		if (str[expd2->i] == '$' || (!ft_isalnum(str[expd2->i])
				&& str[expd2->i] != '_'))
			return (ft_substr(str, expd2->prev_pos + 1, expd2->i
					- expd2->prev_pos - 1));
		len++;
		(expd2->i)++;
	}
	if (len == 0)
		return (ft_strdup("$"));
	return (ft_strdup(str + expd2->prev_pos + 1));
}

int	has_space_at_the_end_2(char *s)
{
	int	strl;

	if (!s)
		return (0);
	strl = ft_strlen(s);
	if (strl == 0)
		return (0);
	if (s[strl - 1] == ' ')
		return (1);
	return (0);
}
