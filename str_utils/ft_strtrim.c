/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:55:04 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/24 17:53:35 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_set(char c, char const *str)
{
	int i = 0;

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
