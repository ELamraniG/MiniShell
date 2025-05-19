/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:55:04 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/19 14:29:11 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	int		start;
	int		end;

	if (s1 == NULL || set == NULL)
		(NULL);
	start = 0;
	end = 0;
	if ((*s1 == 0 && *set == 0) || *s1 == 0)
		return (ft_strdup(""));
	end = ft_strlen(s1) - 1;
	while (ft_strchr(set, s1[start]) != NULL)
		start++;
	while (ft_strchr(set, s1[end]) != NULL && end >= 0)
		end--;
	str = ft_substr(s1, start, end - start + 1);
	return (str);
}
