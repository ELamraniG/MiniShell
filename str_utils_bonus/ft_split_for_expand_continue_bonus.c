/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_for_expand_continue.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:39:00 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 22:39:04 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	do_i_j(int *i, int *k)
{
	(*i)++;
	(*k)++;
}

void	ft_free_split(char **s, int i)
{
	while (i >= 0)
	{
		free(s[i]);
		i--;
	}
	free(s);
}

int	ft_word_count_expand(const char *str, char c)
{
	int	i;
	int	wc;

	wc = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == c))
			i++;
		if (str[i])
			wc++;
		while (str[i] && !(str[i] == c))
			i++;
	}
	if (wc == 0 && str[0] == ' ')
		return (1);
	return (wc);
}
