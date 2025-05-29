/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:46:42 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 23:12:31 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

static int	is_set(char c, char const *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	begn;
	size_t	end;

	if (s1 == NULL)
		return (NULL);
	if (set == NULL)
		return (ft_strdup(s1));
	begn = 0;
	end = ft_strlen(s1) - 1;
	while (is_set(s1[begn], set) == 1)
		begn++;
	if (begn == ft_strlen(s1))
		return (ft_strdup(""));
	while (is_set(s1[end], set) == 1)
		end--;
	return (ft_substr(s1, begn, end - begn + 1));
}
